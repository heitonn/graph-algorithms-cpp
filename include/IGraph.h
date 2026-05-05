#pragma once
#include <string>
#include<vector>

struct IGraph {
    virtual ~IGraph() = default;

    virtual void insert_edge(const std::string& node_a_label,
                             const std::string& edge_label,
                             const std::string& node_b_label) = 0;

    virtual void disconnect(const std::string& node_a_label,
                            const std::string& node_b_label) = 0;

    virtual void remove_node(const std::string& node_label) = 0;

    virtual void read_from_file(const std::string& filename) = 0;
    virtual void write_to_file(const std::string& filename) const = 0;

    // for traversal/implementation of tarjans scc
    virtual std::vector<std::string> get_nodes() const = 0;
    virtual std::vector<std::string> get_neighbors(const std::string& node_label) const = 0;

    // for diamond-shapes 
    virtual std::vector<std::string> get_neighbors_by_label(
    const std::string& node_label,
    const std::string& edge_label) const = 0;
};