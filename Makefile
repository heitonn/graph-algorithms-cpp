CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Iinclude

SRC = src/main.cpp \
      src/AdjacencyMatrixGraph.cpp \
      src/IncidenceListGraph.cpp \
      src/DiamondQuery.cpp \
      src/TarjanSCC.cpp

TARGET = program

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

# Strongly Connected Components (Tarjans SCC)
scc_demo: $(TARGET)
	@echo "Running Tarnajs SCC demo (incidence)..."
	./program scc incidence data/kb03a.dat silent
	@echo ""
	@echo "Running Tarjans SCC demo (matrix)..."
	./program scc matrix data/kb03a.dat silent

# Diamond query/query path 
diamond_demo: $(TARGET)
	@echo "Running Diamond demo (incidence)..."
	./program diamond incidence data/kb03a.dat data/qy03a.dat silent
	@echo ""
	@echo "Running Diamond demo (matrix)..."
	./program diamond matrix data/kb03a.dat data/qy03a.dat silent


# File input/output
io_demo: $(TARGET)
	@echo "Running file I/O demo (incidence)..."
	./program io incidence data/kb03a.dat out_io_incidence.dat
	@echo ""
	@echo "Running file I/O demo (matrix)..."
	./program io matrix data/kb03a.dat out_io_matrix.dat

# Delete edges and nodes
delete_demo: $(TARGET)
	@echo "Running delete demo (incidence)..."
	./program delete incidence data/kb03a.dat n0 n4 n3 out_delete_incidence.dat
	@echo ""
	@echo "Running delete demo (matrix)..."
	./program delete matrix data/kb03a.dat n0 n4 n3 out_delete_matrix.dat

BENCH_SRC = benchmark/benchmark_main.cpp \
            src/IncidenceListGraph.cpp \
            src/AdjacencyMatrixGraph.cpp \
            src/TarjanSCC.cpp \
            src/DiamondQuery.cpp
BENCH_TARGET = benchmark_program

$(BENCH_TARGET): $(BENCH_SRC)
	$(CXX) $(CXXFLAGS) $(BENCH_SRC) -o $(BENCH_TARGET)

clean:
	rm -f $(TARGET)