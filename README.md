# Trace-Driven CPU Pipeline Simulator

A cycle-based, in-order, superscalar five-stage processor simulator, originally developed for SFU CMPT 305 (Spring 2022) and maintained by Shengyu Jia. The simulator models instruction timing rather than architectural register values or instruction semantics. The original assignment specification is retained in [Project 2.pdf](Project%202.pdf).

## Build and test

Requirements: CMake 3.16 or newer and a C++17 compiler (GCC, Clang or MSVC). No third-party libraries or Xcode project files are required.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

On Unix-like single-configuration generators, the executable is `build/proj2`. With Visual Studio or other multi-configuration generators, use `build/Release/proj2.exe` (or the corresponding configuration directory).

## Usage

```sh
./build/proj2 path/to/trace 1 1000000 2
```

Arguments: trace path, one-based starting instruction, exact instruction count, and pipeline width (1–4). The trace must contain enough records to satisfy the requested range. Invalid input, missing files and premature EOF produce a nonzero exit status. The final report on stdout includes cycle count, retired count, and instruction counts and percentages; progress reports are written to stderr approximately every 100,000 retirements.

### Trace format

One comma-separated instruction per line: `PC,type[,dependency_PC...]`. Addresses are hexadecimal (optional `0x` prefix, case-insensitive); up to four dependencies are supported. A trailing comma, whitespace around fields, and CRLF line endings are accepted.

```text
100,1
104,2,100
108,4
10c,5,108
110,3
```

Types: 1 integer, 2 floating point, 3 branch, 4 load, 5 store. The original benchmark traces were not included in the repository; supply decompressed trace files separately. A small example is provided in `tests/sample.trace`.

## Timing model

- Five stages: IF, ID, EX, MEM, WB. Each instruction advances at most one stage per cycle, in program order. Each stage accepts at most W instructions per cycle, with backpressure and no bypass of a stalled head instruction.
- Separate single integer, floating-point and branch execution units; one L1 data read port and one write port. IF uses a separate instruction cache; all cache accesses hit. EX and MEM operations have one-cycle latency when admitted to the stage.
- Integer, floating-point and branch results become available in the cycle after entry into EX; load and store results become available in the cycle after entry into MEM. Dependencies bind to the latest preceding dynamic instance of each PC. Dependencies without a preceding instance in the selected window are treated as already satisfied (including pre-window dependencies).
- Fetch stops immediately upon fetching a branch, including the remainder of its fetch bundle. Fetch resumes the cycle after the branch enters EX. No branch prediction is modeled.
- Cycle 1 is the first fetch cycle; a single instruction retires in cycle 5. Retirement is counted at WB, even for repeated PCs. The last retirement determines total cycles.
- Instructions are streamed instead of preloading the full trace; memory use scales with distinct PCs in the selected window and in-flight dependencies.

## Project layout

- `include/simulator.hpp`: public trace and simulation API.
- `src/simulator.cpp`: parser, stage scheduling, dependency tracking and statistics.
- `src/main.cpp`: CLI and reporting.
- `tests/simulator_tests.cpp`: deterministic unit and regression tests covering pipeline timing, structural/data/control hazards, repeated PCs, offsets, malformed input, EOF and progress reporting.
- `tests/sample.trace`: synthetic example trace.

## Reproducing the original experiment

The assignment specifies widths 1–4, three workloads, and six one-million-instruction replications per configuration, starting at instruction 1, 5,000,000, 10,000,000, 15,000,000, 20,000,000 and 25,000,000 (72 runs total). Use the original workload traces and record the final cycle counts for the factorial analysis. No measured benchmark results are included because the original traces are unavailable in this repository. The included synthetic regression tests validate the documented timing model, not performance on the missing workloads.
