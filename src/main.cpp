#include "simulator.hpp"
#include <charconv>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string_view>

namespace {
pipeline::Count number(std::string_view s) {
    pipeline::Count n = 0;
    const auto result = std::from_chars(s.data(), s.data() + s.size(), n);
    if (result.ec != std::errc{} || result.ptr != s.data() + s.size() || !n)
        throw std::invalid_argument("expected a positive integer");
    return n;
}
void report(std::ostream& out, const pipeline::Stats& s) {
    const char* names[] = {"Integer", "Floating point", "Branch", "Load", "Store"};
    out << "Cycles: " << s.cycles << "\nRetired: " << s.retired << '\n';
    out << std::fixed << std::setprecision(2);
    for (unsigned i = 0; i < 5; ++i)
        out << names[i] << ": " << s.histogram[i] << " (" << (s.retired ? 100.0 * s.histogram[i] / s.retired : 0.0) << "%)\n";
}
}
int main(int argc, char** argv) {
    if (argc != 5) { std::cerr << "Usage: proj2 TRACE START COUNT WIDTH\n"; return 2; }
    try {
        const auto start = number(argv[2]), count = number(argv[3]), width = number(argv[4]);
        if (width > 4) throw std::invalid_argument("width must be 1-4");
        std::ifstream trace(argv[1]);
        if (!trace) throw std::runtime_error("cannot open trace file");
        const auto stats = pipeline::simulate(trace, start, count, static_cast<unsigned>(width), [](const auto& s){ report(std::cerr, s); });
        report(std::cout, stats);
        return 0;
    } catch (const std::exception& e) { std::cerr << "Error: " << e.what() << '\n'; return 1; }
}
