#include <iostream>
#include <memory>
#include <string>

#include "IGraph.h"
#include "IncidenceListGraph.h"
#include "AdjacencyMatrixGraph.h"
#include "TarjanSCC.h"
#include "DiamondQuery.h"

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cerr << "Usage:\n";
        std::cerr << "  ./program scc <incidence|matrix> <graphfile> <silent|verbose>\n";
        std::cerr << "  ./program diamond <incidence|matrix> <graphfile> <queryfile> <silent|verbose>\n";
        std::cerr << "  ./program io <incidence|matrix> <inputfile> <outputfile>\n";
        std::cerr << "  ./program delete <incidence|matrix> <graphfile> <nodeA> <nodeB> <removeNode> <outputfile>\n";
        return 1;
    }

    std::string mode_type = argv[1];
    std::string graph_type = argv[2];

    std::unique_ptr<IGraph> graph;

    if (graph_type == "incidence") {
        graph = std::make_unique<IncidenceListGraph>();
    } else if (graph_type == "matrix") {
        graph = std::make_unique<AdjacencyMatrixGraph>();
    } else {
        std::cerr << "Unknown graph type\n";
        return 1;
    }

    if (mode_type == "scc") {
        if (argc != 5) {
            std::cerr << "Usage: ./program scc <incidence|matrix> <graphfile> <silent|verbose>\n";
            return 1;
        }

        std::string graph_file = argv[3];
        bool verbose = (std::string(argv[4]) == "verbose");

        graph->read_from_file(graph_file);

        TarjanSCC tarjan(*graph, verbose);
        auto sccs = tarjan.run();

        if (!verbose) {
            for (const auto& comp : sccs) {
                for (const auto& node : comp) {
                    std::cout << node << " ";
                }
                std::cout << "\n";
            }
        }
    }
    else if (mode_type == "diamond") {
        if (argc != 6) {
            std::cerr << "Usage: ./program diamond <incidence|matrix> <graphfile> <queryfile> <silent|verbose>\n";
            return 1;
        }

        std::string graph_file = argv[3];
        std::string query_file = argv[4];
        bool verbose = (std::string(argv[5]) == "verbose");

        graph->read_from_file(graph_file);

        DiamondQuery dq(*graph, verbose);
        dq.read_query_file(query_file);
        auto pairs = dq.run();

        if (!verbose) {
            if (pairs.empty()) {
                std::cout << "No diamond pairs found\n";
            } else {
                for (const auto& p : pairs) {
                    std::cout << "(" << p.first << ", " << p.second << ")\n";
                }
            }
        }

    }
    else if (mode_type == "io") {
        if (argc != 5) {
            std::cerr << "Usage: ./program io <incidence|matrix> <inputfile> <outputfile>\n";
            return 1;
        }

        std::string input_file = argv[3];
        std::string output_file = argv[4];

        graph->read_from_file(input_file);
        graph->write_to_file(output_file);

        std::cout << "Graph read from " << input_file
                  << " and written to " << output_file << "\n";
    }
    else if (mode_type == "delete") {
        if (argc != 8) {
            std::cerr << "Usage: ./program delete <incidence|matrix> <graphfile> <nodeA> <nodeB> <removeNode> <outputfile>\n";
            return 1;
        }

        std::string graph_file = argv[3];
        std::string node_a = argv[4];
        std::string node_b = argv[5];
        std::string remove_node = argv[6];
        std::string output_file = argv[7];

        graph->read_from_file(graph_file);

        graph->disconnect(node_a, node_b);
        graph->remove_node(remove_node);

        graph->write_to_file(output_file);

        std::cout << "Applied disconnect(" << node_a << ", " << node_b << ")"
                  << " and remove_node(" << remove_node << ")\n";
        std::cout << "Result written to " << output_file << "\n";
    }
    else {
        std::cerr << "Unknown mode: use 'scc', 'diamond', 'io', or 'delete'\n";
        return 1;
    }

    return 0;
}