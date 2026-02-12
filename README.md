# Modern C++ Systems & Algorithms 

High-performance C++20 implementations of core algorithms, data structures, and iterator abstractions.

Built as a modular CMake-based monorepo emphasizing:

-   RAII & memory safety
-   Rule of Five / move semantics
-   Algorithmic complexity awareness
-   Custom STL-style iterators
-   Probabilistic data structures
-   Clean cross-platform builds

------------------------------------------------------------------------

## Advanced Algorithms

-   Hungarian (Munkres) Algorithm --- O(n³)
-   Bron--Kerbosch maximal clique detection
-   Bloom Filter with MurmurHash & false-positive analysis
-   Greedy range coverage optimization

### Dynamic Programming & Ownership

-   Needleman--Wunsch global alignment
-   RAII linked structure with deep copy & move semantics
-   Heterogeneous DataFrame with lambda-based transforms

### STL & Performance Engineering

-   Statistical simulation pipeline (MT RNG, t-tests)
-   n-skip k-mer hashing engine (performance constrained)
-   Custom reverse-column & diagonal iterators

------------------------------------------------------------------------

## Build

``` bash
cmake -S . -B build
cmake --build build -j
```
