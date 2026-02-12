# C++ Programming

Modern C++ implementations of core algorithms and data structures, with emphasis on memory safety, ownership semantics, performance, and modular build design (CMake).

Built as a multi-module monorepo with reproducible builds.

------------------------------------------------------------------------

## Core Implementations

### Algorithms

**Hungarian (Munkres) Algorithm --- O(n³)**\
Optimal assignment solver using matrix cost transformations and state
tracking.

**Bron--Kerbosch Algorithm**\
Maximal clique enumeration with recursive backtracking and pivoting.

**Needleman--Wunsch (Dynamic Programming)**\
Global sequence alignment with matrix traceback reconstruction.

------------------------------------------------------------------------

### Data Structures

**Bloom Filter** - Bit-vector backed - MurmurHash integration -
False-positive analysis - Approximate cardinality estimation

**Custom DataFrame** - Heterogeneous typed columns - Lambda-based
filtering - STL-style iteration

**RAII Tree Structure** - `std::unique_ptr` ownership model - Rule of
Five - Deep copy & move semantics

------------------------------------------------------------------------

## Engineering Focus

-   Modern C++20
-   Rule of Five / Move semantics
-   Const-correctness
-   Custom iterators
-   Template usage
-   Cross-platform CMake
-   Static vs shared linking
-   Strong compiler warning configuration

------------------------------------------------------------------------

## Build

``` bash
cmake -S . -B build
cmake --build build -j
```

Example targets:

``` bash
build/assignment4/a4_task3   # Hungarian algorithm
build/assignment4/a4_task4   # Bloom filter
build/assignment2/a2_task4   # Needleman–Wunsch
```
------------------------------------------------------------------------

## Quick Review Pointers

Start here:

-   `assignment4/task3.cpp` --- Hungarian algorithm\
-   `assignment4/task4.cpp` --- Bloom filter\
-   `assignment2/task4.cpp` --- Dynamic programming alignment\
-   `assignment3/task4.cpp` --- Custom matrix iterators
