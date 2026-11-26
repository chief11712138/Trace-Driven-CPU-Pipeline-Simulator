#include "simulator.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
using namespace pipeline;
void check(bool ok, const char* message) { if (!ok) throw std::runtime_error(message); }
Stats run(const std::string& trace, Count count, unsigned width = 1, Count start = 1) {
    std::istringstream in(trace); return simulate(in, start, count, width);
}
template<class F> void rejects(F fn) {
    try { fn(); } catch (const std::exception&) { return; }
    throw std::runtime_error("expected rejection");
}
int main() {
    try {
        auto parsed = parse_instruction(" 0x00Ab , 2 , 00F , 0X10\r");
        check(parsed.pc == "ab" && parsed.type == Type::Floating && parsed.dependencies == std::vector<std::string>{"f", "10"}, "parser normalization");
        check(parse_instruction("0,1,").dependencies.empty(), "trailing comma");
        for (const auto* s : {"", "g,1", "1,6", "1,2x", "1,1,,2", "1,1,2,3,4,5,6"}) rejects([&]{ parse_instruction(s); });
        check(run("a,1\n", 1).cycles == 5, "single instruction latency");
        check(run("a,1\nb,1\n", 2).cycles == 6, "scalar throughput");
        check(run("a,1\nb,2\n", 2, 2).cycles == 5, "parallel independent ALU/FPU");
        check(run("a,1\nb,1\n", 2, 2).cycles == 6, "integer structural hazard");
        check(run("a,2\nb,2\n", 2, 2).cycles == 6, "floating structural hazard");
        check(run("a,4\nb,4\n", 2, 2).cycles == 6, "read port contention");
        check(run("a,5\nb,5\n", 2, 2).cycles == 6, "write port contention");
        check(run("a,4\nb,5\n", 2, 2).cycles == 5, "separate data ports");
        check(run("a,1\nb,2,a\n", 2, 2).cycles == 6, "EX forwarding");
        check(run("a,4\nb,1,a\n", 2, 2).cycles == 7, "load forwarding");
        check(run("a,5\nb,1,a\n", 2, 2).cycles == 7, "store forwarding");
        check(run("a,3\nb,1\n", 2, 4).cycles == 8, "branch stalls fetch until next cycle");
        check(run("a,3\nb,3\nc,1\n", 3, 4).cycles == 11, "consecutive branches");
        check(run("a,4\nb,1,a\nc,2\n", 3, 3).cycles == 7, "in-order blocking");
        check(run("a,1\na,4\nb,2,a\n", 3, 3).cycles == 7, "latest dynamic producer");
        check(run("a,1\na,1,a\n", 2, 2).retired == 2, "repeated PC retires twice");
        auto offset = run("a,1\nb,2,a\nc,4\n", 1, 1, 2);
        check(offset.cycles == 5 && offset.histogram[1] == 1, "offset ignores pre-window dependencies");
        auto all = run("a,1\nb,2\nc,3\nd,4\ne,5\n", 5, 4);
        check(all.retired == 5 && all.histogram == std::array<Count,5>{1,1,1,1,1}, "histogram");
        std::string many; for (int i = 0; i < 120; ++i) many += "a,1\n";
        check(run(many, 120, 4).cycles == 124, "long trace and repeated PC");
        std::istringstream progress_trace(many); Count reports = 0;
        simulate(progress_trace, 1, 120, 1, [&](const Stats& s){ check(s.retired > 0, "progress counts"); ++reports; }, 20);
        check(reports == 6, "periodic progress");
        rejects([]{ run("a,1\n", 2); }); rejects([]{ run("a,1\n", 1, 1, 2); });
        rejects([]{ run("a,1\n", 1, 0); }); rejects([]{ run("a,1\n", 1, 5); });
        rejects([]{ run("a,1\n", 0); }); rejects([]{ run("a,1\n", 1, 1, 0); });
        rejects([]{ run("invalid\n", 1); });
        std::cout << "All pipeline tests passed\n";
    } catch (const std::exception& e) { std::cerr << e.what() << '\n'; return 1; }
}
