#include <iostream>
#include <memory>
#include <chrono>
#include <string>

#include "../IGraph.h"
#include "../IncidenceListGraph.h"
#include "../AdjacencyMatrixGraph.h"
#include "../TarjanSCC.h"
#include "../DiamondQuery.h"

int main(int argc, char** argv) {
    if (argc < 5) {
        std::cerr << "Usage:\n";
        std::cerr << "  ./benchmark_program scc <incidence|matrix> <graphfile> <runs>\n";
        std::cerr << "  ./benchmark_program diamond <incidence|matrix> <graphfile> <queryfile> <runs>\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string graph_type = argv[2];

    long long total_time = 0;

    if (mode == "scc") {
        if (argc != 5) {
            std::cerr << "Usage: ./benchmark_program scc <incidence|matrix> <graphfile> <runs>\n";
            return 1;
        }

        std::string graph_file = argv[3];
        int runs = std::stoi(argv[4]);

        for (int i = 0; i < runs; i++) {
            std::unique_ptr<IGraph> graph;

            if (graph_type == "incidence") {
                graph = std::make_unique<IncidenceListGraph>();
            } else if (graph_type == "matrix") {
                graph = std::make_unique<AdjacencyMatrixGraph>();
            } else {
                std::cerr << "Unknown graph type\n";
                return 1;
            }

            graph->read_from_file(graph_file);

            auto start = std::chrono::high_resolution_clock::now();

            TarjanSCC tarjan(*graph, false);
            auto sccs = tarjan.run();

            auto end = std::chrono::high_resolution_clock::now();

            long long duration =
                std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            total_time += duration;
        }

        double avg_time = static_cast<double>(total_time) / runs;
        std::cout << "Average SCC time (microseconds): " << avg_time << "\n";
    }
    else if (mode == "diamond") {
        if (argc != 6) {
            std::cerr << "Usage: ./benchmark_program diamond <incidence|matrix> <graphfile> <queryfile> <runs>\n";
            return 1;
        }

        std::string graph_file = argv[3];
        std::string query_file = argv[4];
        int runs = std::stoi(argv[5]);

        for (int i = 0; i < runs; i++) {
            std::unique_ptr<IGraph> graph;

            if (graph_type == "incidence") {
                graph = std::make_unique<IncidenceListGraph>();
            } else if (graph_type == "matrix") {
                graph = std::make_unique<AdjacencyMatrixGraph>();
            } else {
                std::cerr << "Unknown graph type\n";
                return 1;
            }

            graph->read_from_file(graph_file);

            DiamondQuery dq(*graph, false);
            dq.read_query_file(query_file);

            auto start = std::chrono::high_resolution_clock::now();

            auto pairs = dq.run();

            auto end = std::chrono::high_resolution_clock::now();

            long long duration =
                std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            total_time += duration;
        }

        double avg_time = static_cast<double>(total_time) / runs;
        std::cout << "Average Diamond time (microseconds): " << avg_time << "\n";
    }
    else {
        std::cerr << "Unknown mode: use 'scc' or 'diamond'\n";
        return 1;
    }

    return 0;
}