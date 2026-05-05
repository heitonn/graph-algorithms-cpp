#include "AdjacencyMatrixGraph.h"
#include <fstream>
#include <stdexcept>
#include <vector>

// Returns the index of the node with the given label,
// or -1 if the node does not exist.
int AdjacencyMatrixGraph::find_index_(const std::string& label) const {
    for (int i = 0; i < (int)nodes_.size(); i++) {
        if (nodes_[i].label == label) return i;
    }
    return -1;
}


// Returns the index of an existing node.
// If the node does not exist, it is created and the adjacency matrix is resized
int AdjacencyMatrixGraph::get_or_create_index_(const std::string& label) {
    int idx = find_index_(label);
    if (idx != -1) return idx;

    nodes_.emplace_back(label);

    // grow matrix to NxN
    const int n = (int)nodes_.size();
    adj_.resize(n);
    for (int i = 0; i < n; i++) adj_[i].resize(n);

    return n - 1;
}


// Inserts a directed edge from node_a_label to node_b_label
// with the given edge label.
void AdjacencyMatrixGraph::insert_edge(const std::string& node_a_label,
                                       const std::string& edge_label,
                                       const std::string& node_b_label) {
    int i = get_or_create_index_(node_a_label);
    int j = get_or_create_index_(node_b_label);
    adj_[i][j].push_back(edge_label);
}

// Removes all edges from node_a_label to node_b_label.
// If this creates isolated nodes, they are removed.
void AdjacencyMatrixGraph::disconnect(const std::string& node_a_label,
                                      const std::string& node_b_label) {
    int i = find_index_(node_a_label);
    int j = find_index_(node_b_label);
    if (i == -1 || j == -1) return;

    adj_[i][j].clear();
    remove_isolated_nodes_();
}

// Removes a node completely from the graph by deleting its
// row and column in the adjacency matrix.
void AdjacencyMatrixGraph::remove_node(const std::string& node_label) {
    int k = find_index_(node_label);
    if (k == -1) return;

    // erase node
    nodes_.erase(nodes_.begin() + k);

    // erase row k
    adj_.erase(adj_.begin() + k);
    // erase col k
    for (auto& row : adj_) {
        row.erase(row.begin() + k);
    }

    remove_isolated_nodes_();
}

// Reads graph data from file in triple notation:
// node_a edge_label node_b.
void AdjacencyMatrixGraph::read_from_file(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        throw std::runtime_error("Could not open file for reading: " + filename);
    }

    std::string a, e, b;

    while (in >> a >> e >> b) {
        if (!b.empty() && b.back() == '.') {
            b.pop_back();
        }
        insert_edge(a, e, b);
    }
}


// Writes the graph back to file in triple notation.
void AdjacencyMatrixGraph::write_to_file(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out) throw std::runtime_error("Could not open file for writing: " + filename);

    const int n = (int)nodes_.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (const auto& edge_label : adj_[i][j]) {
                out << nodes_[i].label << " " << edge_label << " " << nodes_[j].label << ".\n";
            }
        }
    }
}

// Returns all node labels in the graph.
// For Tarjans SCC algorithm.
std::vector<std::string> AdjacencyMatrixGraph::get_nodes() const {
    std::vector<std::string> labels;

    for (const auto& node : nodes_) {
        labels.push_back(node.label);
    }

    return labels;
}

// Returns all neighbors reachable by at least one outgoing edge,
// regardless of edge label.
// For Tarjans SCC algorithm.
std::vector<std::string> AdjacencyMatrixGraph::get_neighbors(const std::string& node_label) const {
    std::vector<std::string> neighbors;

    int i = find_index_(node_label);
    if (i == -1) {
        return neighbors;
    }

    for (int j = 0; j < (int)nodes_.size(); j++) {
        if (!adj_[i][j].empty()) {
            neighbors.push_back(nodes_[j].label);
        }
    }

    return neighbors;
}

// Returns all neighbors reachable from node_label by an edge
// with exactly the given edge label.
// Used for finding Diamond-shapes
std::vector<std::string> AdjacencyMatrixGraph::get_neighbors_by_label(
    const std::string& node_label,
    const std::string& edge_label) const {

    std::vector<std::string> neighbors;

    int i = find_index_(node_label);
    if (i == -1) {
        return neighbors;
    }

    for (int j = 0; j < (int)nodes_.size(); j++) {
        for (const auto& label : adj_[i][j]) {
            if (label == edge_label) {
                neighbors.push_back(nodes_[j].label);
                break;
            }
        }
    }

    return neighbors;
}

// Returns true if node k has no incoming and no outgoing edges.
bool AdjacencyMatrixGraph::is_isolated_index_(int k) const {
    const int n = (int)nodes_.size();
    // any outgoing?
    for (int j = 0; j < n; j++) {
        if (!adj_[k][j].empty()) return false;
    }
    // any incoming?
    for (int i = 0; i < n; i++) {
        if (!adj_[i][k].empty()) return false;
    }
    return true;
}


// Removes all isolated nodes from the graph.
// Repeats until no isolated nodes remain.
void AdjacencyMatrixGraph::remove_isolated_nodes_() {
    bool removed = true;
    while (removed) {
        removed = false;
        for (int k = 0; k < (int)nodes_.size(); ) {
            if (is_isolated_index_(k)) {
                // remove node k
                nodes_.erase(nodes_.begin() + k);
                adj_.erase(adj_.begin() + k);
                for (auto& row : adj_) row.erase(row.begin() + k);
                removed = true;
            } else {
                ++k;
            }
        }
    }
}