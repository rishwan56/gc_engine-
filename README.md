# Custom C Garbage Collection Engine (gc_engine)

A custom, low-level memory management and garbage collection engine written in C. This project implements and compares two primary memory reclamation paradigms from scratch: **Reference Counting** (real-time, incremental) and **Mark-and-Sweep** (tracing, generational).

Designed as a systems programming project to analyze memory layout, allocation latency, and performance trade-offs under different GC strategies.

---

## 🚀 Key Features

* **Dual-Paradigm Collection:**
  * **Reference Counting:** Implements deterministic, real-time memory reclamation using manual pointer management (`refcount_inc` / `refcount_dec`). Tracks heap allocations globally using a doubly-linked allocation list.
  * **Mark-and-Sweep:** Employs a simulated Virtual Machine (`vm_t`) and call stack frames (`frame_t`). Performs mark, trace, and sweep passes over roots to reclaim dead object cycles in bulk.
* **Heap Visualizer:** A text-based diagnostics tool that dumps active memory layouts, printing object types, memory addresses, data payloads, and refcount/reachability statuses.
* **Automated Benchmarking Suite:** A comparative suite (C + Python) that compiles both modes, runs uniform high-intensity workloads, and generates a side-by-side performance report (Execution Time vs. Peak Memory Footprint).
* **Cycle & Leak Detection:** Identifies unreachable circular references in the reference counting module to flag memory leaks.

---

## 📁 Repository Structure

* `reference_counting/` - Reference counting memory management, refcount headers, and global list tracking.
* `mark_and_sweep/` - VM implementation, marking/tracing/sweeping algorithms, and root stack frames.
* `stack/` - General stack helper data structure used by the VM.
* `visualizer.c` - Visualizer and leak detection driver program.
* `benchmark.c` - Uniform workload runner for execution profiling.
* `run_benchmarks.py` - Orchestration script to compile, execute, and generate the comparative analysis report.
* `CMakeLists.txt` - Build system configuration with conditional compilation flags.

---

## 🛠️ How to Build & Run

Ensure you have `cmake` and a C compiler (e.g., `gcc`) installed.

### 1. Build and Run the Visualizer
You can compile either the Reference Counting or Mark-and-Sweep visualizer using the `USE_MARK_SWEEP` flag.

#### **For Reference Counting Visualizer:**
```bash
mkdir -p build && cd build
cmake -DUSE_MARK_SWEEP=OFF ..
make gc_visualizer
./gc_visualizer
```

#### **For Mark-and-Sweep Visualizer:**
```bash
mkdir -p build && cd build
cmake -DUSE_MARK_SWEEP=ON ..
make gc_visualizer
./gc_visualizer
```

### 2. Run the Benchmark Suite
The Python script automates the compilation of both versions, runs the benchmark workload, and generates a comparison report. From the project root, run:
```bash
python3 run_benchmarks.py
```

---

## 📊 Reference Counting vs. Mark-and-Sweep Trade-offs

| Metric | Reference Counting | Mark-and-Sweep |
| :--- | :--- | :--- |
| **Reclamation Latency** | Immediate / Real-time (drops to 0) | Deferred (runs when memory fills up) |
| **Pause Time** | Distributed incrementally | "Stop-the-World" sweep pauses |
| **Circular References** | Leaks memory cycles unless manually broken | Reclaims cycles naturally |
| **Memory Overhead** | Higher (per-object refcount header + global list pointers) | Lower (uses temporary trace bits) |
| **Implementation Complexity**| Moderate (heavy pointer bookkeeping) | High (root tracing, stack traversal) |
