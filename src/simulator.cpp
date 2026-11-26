#include "simulator.hpp"
#include <algorithm>
#include <cctype>
#include <deque>
#include <limits>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <utility>

namespace pipeline {
namespace {
std::string trim(std::string s) {
    const auto first = s.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return {};
    return s.substr(first, s.find_last_not_of(" \t\r\n") - first + 1);
}
std::string pc_key(std::string s) {
    s = trim(std::move(s));
    if (s.size() >= 2 && s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) s.erase(0, 2);
    if (s.empty() || !std::all_of(s.begin(), s.end(), [](unsigned char c){ return std::isxdigit(c); }))
        throw std::invalid_argument("invalid hexadecimal PC");
    for (char& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    const auto first = s.find_first_not_of('0');
    return first == std::string::npos ? "0" : s.substr(first);
}
constexpr Count pending = std::numeric_limits<Count>::max();
struct Dynamic {
    Type type;
    std::shared_ptr<Count> ready;
    std::vector<std::shared_ptr<Count>> dependencies;
};
using Stage = std::deque<Dynamic>;
void move_front(Stage& from, Stage& to) {
    to.push_back(std::move(from.front()));
    from.pop_front();
}
}

Instruction parse_instruction(const std::string& line) {
    std::vector<std::string> fields;
    std::size_t begin = 0;
    for (;;) {
        auto end = line.find(',', begin);
        fields.push_back(trim(line.substr(begin, end == std::string::npos ? end : end - begin)));
        if (end == std::string::npos) break;
        begin = end + 1;
    }
    if (fields.size() > 2 && fields.back().empty()) fields.pop_back();
    if (fields.size() < 2 || fields.size() > 6 || fields[1].size() != 1 || fields[1][0] < '1' || fields[1][0] > '5')
        throw std::invalid_argument("expected PC,type[,dependency...] (type 1-5, at most four dependencies)");
    Instruction result{pc_key(fields[0]), static_cast<Type>(fields[1][0] - '0'), {}};
    for (std::size_t i = 2; i < fields.size(); ++i) result.dependencies.push_back(pc_key(fields[i]));
    return result;
}

Stats simulate(std::istream& trace, Count start, Count count, unsigned width,
               const std::function<void(const Stats&)>& progress, Count interval) {
    if (!start || !count || width < 1 || width > 4 || count - 1 > pending - start)
        throw std::invalid_argument("start/count must be positive, width 1-4, and range must not overflow");
    std::string line;
    for (Count i = 1; i < start; ++i)
        if (!std::getline(trace, line)) throw std::runtime_error("trace ended before start instruction");
    Stage fetch, decode, execute, memory;
    std::unordered_map<std::string, std::shared_ptr<Count>> latest;
    Count fetched = 0;
    bool branch_pending = false;
    Stats stats;
    Count next_report = interval;
    while (stats.retired < count) {
        if (stats.cycles >= pending - 1) throw std::overflow_error("cycle counter overflow");
        const Count cycle = ++stats.cycles;
        bool changed = false;
        const bool fetch_blocked = branch_pending;
        // Reverse stage order: an instruction can enter at most one stage per cycle.
        // WB is the terminal stage: retire at the end of this cycle.
        while (!memory.empty()) {
            ++stats.histogram[static_cast<unsigned>(memory.front().type) - 1];
            ++stats.retired;
            memory.pop_front();
            changed = true;
        }
        bool read_used = false, write_used = false;
        while (!execute.empty() && memory.size() < width) {
            auto& inst = execute.front();
            if ((inst.type == Type::Load && read_used) || (inst.type == Type::Store && write_used)) break;
            if (inst.type == Type::Load || inst.type == Type::Store) {
                read_used |= inst.type == Type::Load;
                write_used |= inst.type == Type::Store;
                *inst.ready = cycle + 1;
            }
            move_front(execute, memory); changed = true;
        }
        std::array<bool, 3> unit_used{};
        while (!decode.empty() && execute.size() < width) {
            auto& inst = decode.front();
            if (std::any_of(inst.dependencies.begin(), inst.dependencies.end(), [cycle](const auto& p){ return *p > cycle; })) break;
            const auto type = static_cast<unsigned>(inst.type);
            if (type <= 3) {
                if (unit_used[type - 1]) break;
                unit_used[type - 1] = true;
                *inst.ready = cycle + 1;
                if (inst.type == Type::Branch) branch_pending = false;
            }
            move_front(decode, execute); changed = true;
        }
        while (!fetch.empty() && decode.size() < width) {
            move_front(fetch, decode); changed = true;
        }
        while (!fetch_blocked && !branch_pending && fetched < count && fetch.size() < width) {
            if (!std::getline(trace, line)) throw std::runtime_error("trace ended before requested instruction count");
            Instruction parsed;
            try { parsed = parse_instruction(line); }
            catch (const std::invalid_argument& e) { throw std::runtime_error("trace line " + std::to_string(start + fetched) + ": " + e.what()); }
            Dynamic inst{parsed.type, std::make_shared<Count>(pending), {}};
            for (const auto& dep : parsed.dependencies) {
                auto it = latest.find(dep);
                if (it != latest.end()) inst.dependencies.push_back(it->second);
            }
            latest[parsed.pc] = inst.ready;
            branch_pending = parsed.type == Type::Branch;
            fetch.push_back(std::move(inst)); ++fetched; changed = true;
        }
        if (progress && interval && stats.retired >= next_report) {
            progress(stats);
            next_report = next_report > pending - interval ? pending : next_report + interval;
            if (next_report <= stats.retired && next_report != pending) {
                const Count remainder = stats.retired % interval;
                const Count delta = interval - remainder;
                next_report = stats.retired > pending - delta ? pending : stats.retired + delta;
            }
        }
        if (!changed) throw std::logic_error("pipeline made no progress");
    }
    return stats;
}
}
