#pragma once
#include "IGraph.h"

#include <string>
#include <vector>

// Graph implementation of Adjacency MAtrix.
// Nodes stored in a vector, edges stored in a 3D structure:
// adj_[i][j] contains all edge labels for edges from node i to node j.
class AdjacencyMatrixGraph : public IGraph {
public:
    AdjacencyMatrixGraph() = default;
    ~AdjacencyMatrixGraph() override = default;

    // IGraph interface (parent class)
    void insert_edge(const std::string& node_a_label,
                     const std::string& edge_label,
                     const std::string& node_b_label) override;

    void disconnect(const std::string& node_a_label,
                    const std::string& node_b_label) override;

    void remove_node(const std::string& node_label) override;

    // Read and write graph in triple notation:
    // node_a edge_label node_b.
    void read_from_file(const std::string& filename) override;
    void write_to_file(const std::string& filename) const override;

    // Traversal for Tarjans SCC algorithm 
    std::vector<std::string> get_nodes() const override;
    std::vector<std::string> get_neighbors(const std::string& node_label) const override;

    // Traversdal for finding Diamond-shapes
    // returns all neighbors reachable by an edge with a specific label
    std::vector<std::string> get_neighbors_by_label(
    const std::string& node_label,
    const std::string& edge_label) const override;

private:
    struct Node {
        std::string label;
        explicit Node(std::string lbl) : label(std::move(lbl)) {}
    };

    // List of nodes in the graph
    std::vector<Node> nodes_;

    // adj_[i][j] = list of edge labels from i -> j (multi-edges)
    std::vector<std::vector<std::vector<std::string>>> adj_;

    // Returns the index of a node label, or -1 if not found
    int find_index_(const std::string& label) const;

    // Returns the index of an existing node, or creates it if missing
    int get_or_create_index_(const std::string& label);

    // Removes nodes with no incoming or outgoing edges
    void remove_isolated_nodes_();

    // Checks whether node k has neither incoming nor outgoing edges
    bool is_isolated_index_(int k) const;
};