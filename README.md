# C++ Graph Algorithms

A modular C++ implementation of directed graph data structures and graph algorithms, with support for multiple representations and performance benchmarking.

---

## Overview

This project implements graph data structures and algorithms in C++, focusing on:

- clean abstraction via a shared graph interface
- multiple underlying representations
- reusable algorithm implementations
- performance comparison between representations

The system is designed so that algorithms operate on a common interface (`IGraph`), allowing the same logic to run on different graph implementations.

---

## Graph Representations

Two graph representations are implemented:

### Incidence List Graph
- Nodes store outgoing edges
- Explicit edge objects
- Uses dynamic memory (manual management)
- Suitable for sparse graphs

### Adjacency Matrix Graph
- Matrix-based structure
- Supports multiple labeled edges between nodes
- Uses standard containers for memory management
- Suitable for dense graphs

---

## Interface Design

All graph implementations inherit from a shared abstract interface:

- `insert_edge`
- `disconnect`
- `remove_node`
- `read_from_file`
- `write_to_file`
- `get_nodes`
- `get_neighbors`
- `get_neighbors_by_label`

This enables algorithm implementations to remain independent of the underlying data structure.

---

## Implemented Algorithms

### Tarjan’s Strongly Connected Components (SCC)
- Linear-time algorithm: **O(V + E)**
- Depth-first search with lowlink values
- Identifies strongly connected components in directed graphs

### Diamond Query
- Finds node pairs connected through two distinct labeled paths
- Path traversal based on edge labels
- Useful for structured graph queries

---

## Benchmarking

The project includes tools for comparing performance between graph representations.

### Benchmarks included:
- SCC computation (Tarjan)
- Diamond query execution

### Example:

```bash
./graph-benchmark-scc 1000 3000 scc1000.dat
```

Benchmarks can be repeated and averaged to compare runtime across implementations.

---

## Analysis

A Jupyter notebook is included for analyzing benchmark results:

`benchmark/plot.ipynb`

This allows visualization of:
- runtime scaling
- differences between adjacency matrix and incidence list
- algorithm performance trends

---

## File Structure

.
├── include/        # Header files (interfaces and class definitions)  
├── src/            # Implementation files  
├── benchmark/      # Benchmark programs and analysis  
├── data/           # Example graph and query data  
├── docs/           # Diagrams and documentation  
├── Makefile  
└── README.md  

---

## Build and Run
### Compile
Run the build using your environment:

**Linux / macOS:**
```bash
make
```

**On Windows (MSYS2 / MinGW):**
```bash
mingw32-make
```

### Run examples
#### Strongly Connected Components

```bash
./program scc incidence data/kb03a.dat silent
./program scc matrix data/kb03a.dat silent
```

#### Diamond Query

```bash
./program diamond incidence data/kb03a.dat data/qy03a.dat verbose
```
---

## Design Highlights
- Separation of interface and implementation
- Support for multiple graph representations
- Algorithm reuse across implementations
- Manual memory management (incidence list) vs STL-based (matrix)
- Benchmark-driven comparison of performance

## Possible Extensions
- Additional graph algorithms (shortest paths, BFS/DFS variants)
- Weighted edges
- Parallel or optimized implementations
- Improved benchmarking and visualization

---- 
**OLD**
---
Compilation:
Run the program using the provided Makefile:
    mingw32-make

Demo targets:
    mingw32-make pro31  (SCC)
    mingw32-make pro32  (Diamond query)
    mingw32-make pro22  (file I/O)    
    mingw32-make pro25  (delete edges and nodes)

Benchmarking:

Graph data is generated using the provided generator programs:
    g++ -std=c++17 -O2 benchmark/graph-benchmark-scc.cpp -o generate_scc
    g++ -std=c++17 -O2 benchmark/graph-benchmark-diamond.cpp -o generate_graph

Example usage:
    ./generate_scc 1000 3000 scc1000.dat
    ./generate_graph 1000 3 kb1000.dat qy1000.dat

The benchmark program can then be run as:
    ./benchmark_program scc incidence scc1000.dat 10
    ./benchmark_program diamond incidence kb1000.dat qy1000.dat 10

Note:
.dat files are not included in the submission.