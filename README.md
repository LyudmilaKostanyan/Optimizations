# Optimizations Analysis

## Overview

This repository contains a set of performance tests designed to demonstrate and analyze various compiler and programming optimizations. Each test illustrates specific optimization techniques, measures their impact on performance, and provides insights into efficient programming practices.

---

## Problem Description

Understanding the performance impact of different optimization techniques is crucial for writing efficient and high-performance code. This repository includes practical examples demonstrating how compiler optimizations, memory management strategies, and coding patterns affect execution speed and resource usage.

---

## Detailed Explanation of Each Optimization

### 1. Compiler Optimization Levels (-O0, -O1, -O2, -O3)

This test demonstrates how various compiler optimization levels influence the execution speed and generated assembly code. It shows:

* Reduced execution time at higher optimization levels.
* Significant reductions in assembly instructions.
* Improved CPU pipeline utilization.

### 2. Function Inlining

This test highlights the performance benefit of inlining frequently called small functions:

* Reduces function call overhead.
* Improves cache locality and reduces execution time.
* Assembly inspection shows direct inclusion of function bodies, eliminating call instructions.

### 3. Loop Unrolling

Loop unrolling reduces the overhead associated with loop control structures by processing multiple elements per iteration:

* Decreases the number of conditional checks and jumps.
* Improves CPU pipeline and instruction-level parallelism.
* Can increase cache pressure if overused, negatively affecting performance.

### 4. Short-String Optimization (SSO)

Demonstrates how SSO avoids heap allocation for short strings by storing them directly within the `std::string` object:

* Reduces heap allocations and deallocations, minimizing overhead.
* Improves performance by enhancing memory locality and cache efficiency.

### 5. Devirtualization Techniques

Shows performance improvements by eliminating virtual function calls:

* Using the `final` keyword allows the compiler to resolve virtual calls at compile-time.
* Reduces runtime overhead associated with indirect function calls through vtables.
* Results in direct function calls, evident in optimized assembly output.

### 6. Algebraic Reductions and Vectorization

Demonstrates compiler capabilities in algebraic simplifications and vectorization:

* Simplified expressions and constant folding optimize computational workload.
* Auto-vectorization utilizes SIMD instructions for parallel processing.
* Performance improvements verified through assembly analysis.

### 7. Register Usage and Pointer Elimination

Highlights how minimizing pointer dereferences and arithmetic improves performance:

* Temporary references and direct indexing optimize register usage.
* Reduces instruction count and memory access overhead.
* Enhances CPU cache efficiency, resulting in faster execution.

### 8. Memory Fragmentation and Cache Efficiency

Illustrates the importance of memory management strategies to minimize fragmentation:

* Random dynamic allocations demonstrate memory fragmentation and performance degradation.
* Fixed-size pooling and contiguous memory strategies (`std::vector`) enhance cache utilization.
* Improved execution speed and reduced memory overhead.

---

## Example Output

### With -O0 optimization

```
===========================================================
               Performance Optimization Tests
===========================================================
Sum: 100000000
Elapsed time: 0.227469 seconds

===========================================================
                 Inline vs No Inline Tests
===========================================================
No inline time: 0.308708 s
Inline time: 0.229215 s

===========================================================
                    Loop Unrolling Tests
===========================================================
Normal copy time: 0.337662 seconds
Unrolled copy time: 0.292422 seconds

===========================================================
              Short-String Optimization Tests
===========================================================
Short string time: 0.0564597 seconds
Long string time: 0.174635 seconds

Short string data pointer: 0x6ffffe170020
Long string data pointer: 0xa00026970

===========================================================
                Virtual vs Direct Call Tests
===========================================================
Virtual call time: 0.0196069
Direct call time: 0.0238042

===========================================================
               Algorithmic Optimization Tests
===========================================================
Original time: 0.0533538 seconds
Simplified time: 0.0567377 seconds

===========================================================
          Pointer Dereference vs Elimination Tests
===========================================================
Pointer dereference time: 0.0021382 s
Pointer elimination time: 0.0033901 s

===========================================================
      Memory Fragmentation and Cache Efficiency Tests
===========================================================
Fragmentation test done in 8.27237 seconds
Contiguous allocation test done in 2.30058 seconds
===========================================================
```

### With -O3 optimization

```
===========================================================
               Performance Optimization Tests
===========================================================
Sum: 100000000
Elapsed time: 0.0319167 seconds

===========================================================
                 Inline vs No Inline Tests
===========================================================
No inline time: 0.0994968 s
Inline time: 0.0427508 s

===========================================================
                    Loop Unrolling Tests
===========================================================
Normal copy time: 0.0346232 seconds
Unrolled copy time: 0.0498944 seconds

===========================================================
              Short-String Optimization Tests
===========================================================
Short string time: 0.0105344 seconds
Long string time: 0.0705498 seconds

Short string data pointer: 0x6ffffe170020
Long string data pointer: 0xa00026970

===========================================================
                Virtual vs Direct Call Tests
===========================================================
Virtual call time: 1e-07
Direct call time: 2e-07

===========================================================
               Algorithmic Optimization Tests
===========================================================
Original time: 0.0082508 seconds
Simplified time: 0.0083517 seconds

===========================================================
          Pointer Dereference vs Elimination Tests
===========================================================
Pointer dereference time: 1e-07 s
Pointer elimination time: 1e-07 s

===========================================================
      Memory Fragmentation and Cache Efficiency Tests
===========================================================
Fragmentation test done in 8.29983 seconds
Contiguous allocation test done in 0.211229 seconds
===========================================================
```

---

## Explanation of Output

* Results clearly indicate execution times and performance differences.
* Structured output facilitates easy comparison between different optimization strategies.

---

## How to Compile and Run

### Clone the Repository

```bash
git clone https://github.com/LyudmilaKostanyan/optimizations.git
cd optimizations
```

### Build

```bash
cmake -S . -B build
cmake --build build
```

### Run

```bash
cd build
./main
```
