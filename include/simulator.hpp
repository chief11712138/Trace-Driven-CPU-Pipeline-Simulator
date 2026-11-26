#pragma once
#include <array>
#include <cstdint>
#include <functional>
#include <istream>
#include <string>
#include <vector>

namespace pipeline {
using Count = std::uint64_t;
enum class Type { Integer = 1, Floating, Branch, Load, Store };
struct Instruction { std::string pc; Type type; std::vector<std::string> dependencies; };
struct Stats { Count cycles = 0, retired = 0; std::array<Count, 5> histogram{}; };
Instruction parse_instruction(const std::string& line);
// start is one-based; count is the exact number of instructions to retire.
Stats simulate(std::istream& trace, Count start, Count count, unsigned width,
               const std::function<void(const Stats&)>& progress = {}, Count interval = 100000);
}
