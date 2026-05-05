# C++ Graph Algorithms

A modular C++ implementation of directed graph data structures and graph algorithms, with support for multiple representations and runtime benchmarking.

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
- Depth-first search with lowlink values
- Identifies strongly connected components in directed graphs
- Theoretical runtime is `O(V + E)`.

### Diamond Query
- Finds node pairs connected through two distinct labeled paths
- Path traversal based on edge labels
- Useful for structured graph queries

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
Build:

**On Windows (MSYS2 / MinGW):**
```bash
mingw32-make
```

**Linux / macOS:**
```bash
make
```

The build produces:
```bash
program
```

### Run demos
On Linux/macOS replace `mingw32-make` with `make`

```bash
mingw32-make scc_demo
mingw32-make diamond_demo
mingw32-make io_demo
mingw32-make delete_demo
```


### Usage
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

## Benchmarking

The project includes benchmark code for comparing the incidence list and adjacency matrix representations.

Benchmarks are used to compare:
- Tarjan SCC runtime
- Diamond query runtime
- scaling behavior for different graph sizes


Benchmarks can be repeated and averaged to compare runtime across implementations.

A Jupyter notebook is included for plotting benchmark results

```bash
benchmark/plot.ipynb
```

---

## Documentation

A detailed project report is available in:

docs/report.pdf

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

