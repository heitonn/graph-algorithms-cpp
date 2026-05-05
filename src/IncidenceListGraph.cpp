#include "IncidenceListGraph.h"
#include <vector>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <unordered_map>

// Destructor: releases all dynamically allocated nodes and edges.
IncidenceListGraph::~IncidenceListGraph() {
    clear_();
}

// Copy constructor: performs a deep copy of the other graph.
IncidenceListGraph::IncidenceListGraph(const IncidenceListGraph& other) {
    deep_copy_from_(other);
}

// Copy assignment: clears current content and deep-copies from other.
IncidenceListGraph& IncidenceListGraph::operator=(const IncidenceListGraph& other) {
    if (this == &other) return *this;
    clear_();
    deep_copy_from_(other);
    return *this;
}

// Move constructor: takes ownership of the other graph's resources.
IncidenceListGraph::IncidenceListGraph(IncidenceListGraph&& other) noexcept {
    nodes_ = std::move(other.nodes_);
    edges_ = std::move(other.edges_);
    other.nodes_.clear();
    other.edges_.clear();
}

// Move assignment: clears current content and takes ownership from other.
IncidenceListGraph& IncidenceListGraph::operator=(IncidenceListGraph&& other) noexcept {
    if (this == &other) return *this;
    clear_();
    nodes_ = std::move(other.nodes_);
    edges_ = std::move(other.edges_);
    other.nodes_.clear();
    other.edges_.clear();
    return *this;
}

// Inserts a directed edge from node_a_label to node_b_label
// with the given edge label.
void IncidenceListGraph::insert_edge(const std::string& node_a_label,
                                     const std::string& edge_label,
                                     const std::string& node_b_label) {
    Node* a = get_or_create_node_(node_a_label);
    Node* b = get_or_create_node_(node_b_label);

    Edge* e = new Edge(edge_label, a, b);
    edges_.push_back(e);
    a->outgoing.push_back(e);
}

// Removes all edges from node_a_label to node_b_label.
// If this creates isolated nodes, they are removed.
void IncidenceListGraph::disconnect(const std::string& node_a_label,
                                    const std::string& node_b_label) {
    Node* a = find_node_(node_a_label);
    Node* b = find_node_(node_b_label);
    if (!a || !b) return;

    std::vector<Edge*> to_delete;

    // Remove matching outgoing edges from a
    auto& out = a->outgoing;
    out.erase(std::remove_if(out.begin(), out.end(),
        [&](Edge* e) {
            if (e && e->to == b) {
                to_delete.push_back(e);
                return true;
            }
            return false;
        }), out.end());

    // Remove the same edges from graph ownership and delete them
    for (Edge* e : to_delete) {
        edges_.erase(std::remove(edges_.begin(), edges_.end(), e), edges_.end());
        delete e;
    }

    remove_isolated_nodes_();
}

// Removes a node completely from the graph.
// All incident edges are removed before the node itself is deleted.
void IncidenceListGraph::remove_node(const std::string& node_label) {
    Node* n = find_node_(node_label);
    if (!n) return;

    remove_edges_incident_to_(n);

    nodes_.erase(std::remove(nodes_.begin(), nodes_.end(), n), nodes_.end());
    delete n;

    remove_isolated_nodes_();
}

// Reads graph data from file in triple notation:
// node_a edge_label node_b.
void IncidenceListGraph::read_from_file(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        throw std::runtime_error("Could not open file for reading: " + filename);
    }

    std::string a, e, b;

    while (in >> a >> e >> b) {
        // Remove trailing '.' from the last token
        if (!b.empty() && b.back() == '.') {
            b.pop_back();
        }
        insert_edge(a, e, b);
    }
}

// Writes the graph back to file in triple notation.
void IncidenceListGraph::write_to_file(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out) {
        throw std::runtime_error("Could not open file for writing: " + filename);
    }

    for (const Edge* e : edges_) {
        out << e->from->label << " "
            << e->label << " "
            << e->to->label << ".\n";
    }
}

// Returns all node labels in the graph.
// Used by Tarjan's SCC algorithm.
std::vector<std::string> IncidenceListGraph::get_nodes() const {
    std::vector<std::string> labels;

    for (const auto& node : nodes_) {
        labels.push_back(node->label);
    }

    return labels;
}

// Returns all neighbors reachable by at least one outgoing edge,
// regardless of edge label.
// Used by Tarjan's SCC algorithm.
std::vector<std::string> IncidenceListGraph::get_neighbors(const std::string& node_label) const {
    std::vector<std::string> neighbors;

    Node* node = find_node_(node_label);
    if (!node) {
        return neighbors;
    }

    for (const auto& edge : node->outgoing) {
        neighbors.push_back(edge->to->label);
    }

    return neighbors;
}

// Returns all neighbors reachable from node_label by an edge
// with exactly the given edge label.
// Used in problem 3.2 for path queries with fixed label sequences.
std::vector<std::string> IncidenceListGraph::get_neighbors_by_label(
    const std::string& node_label,
    const std::string& edge_label) const {

    std::vector<std::string> neighbors;

    Node* node = find_node_(node_label);
    if (!node) {
        return neighbors;
    }

    for (const auto& edge : node->outgoing) {
        if (edge->label == edge_label) {
            neighbors.push_back(edge->to->label);
        }
    }

    return neighbors;
}

// Returns the node with the given label, or nullptr if not found.
IncidenceListGraph::Node* IncidenceListGraph::find_node_(const std::string& label) const {
    for (Node* n : nodes_) {
        if (n && n->label == label) return n;
    }
    return nullptr;
}

// Returns an existing node, or creates a new one if missing.
IncidenceListGraph::Node* IncidenceListGraph::get_or_create_node_(const std::string& label) {
    if (Node* existing = find_node_(label)) return existing;
    Node* n = new Node(label);
    nodes_.push_back(n);
    return n;
}

// Deletes all edges and nodes owned by the graph.
void IncidenceListGraph::clear_() {
    // Delete edges first so node pointers inside edges are still valid while cleaning up
    for (Edge* e : edges_) delete e;
    edges_.clear();

    for (Node* n : nodes_) delete n;
    nodes_.clear();
}

// Deep-copies nodes and edges from another incidence-list graph.
void IncidenceListGraph::deep_copy_from_(const IncidenceListGraph& other) {
    // 1) Copy nodes and build a map from old node pointers to new node pointers
    // 2) Copy edges using the node map
    // 3) Rebuild outgoing incidence lists

    std::unordered_map<const Node*, Node*> node_map;
    node_map.reserve(other.nodes_.size());

    for (const Node* old_n : other.nodes_) {
        Node* new_n = new Node(old_n->label);
        nodes_.push_back(new_n);
        node_map[old_n] = new_n;
    }

    for (const Edge* old_e : other.edges_) {
        Node* new_from = node_map.at(old_e->from);
        Node* new_to   = node_map.at(old_e->to);

        Edge* new_e = new Edge(old_e->label, new_from, new_to);
        edges_.push_back(new_e);
    }

    for (Node* n : nodes_) n->outgoing.clear();
    for (Edge* e : edges_) {
        e->from->outgoing.push_back(e);
    }
}

// Returns true if the node has no outgoing edges and no incoming edges.
bool IncidenceListGraph::is_isolated_(Node* n) const {
    if (!n) return true;
    if (!n->outgoing.empty()) return false;

    for (const Edge* e : edges_) {
        if (e && e->to == n) return false;
    }
    return true;
}

// Removes all isolated nodes from the graph.
// Repeats until no isolated nodes remain.
void IncidenceListGraph::remove_isolated_nodes_() {
    bool removed = true;

    while (removed) {
        removed = false;

        for (auto it = nodes_.begin(); it != nodes_.end();) {
            Node* n = *it;
            if (is_isolated_(n)) {
                remove_edges_incident_to_(n);
                it = nodes_.erase(it);
                delete n;
                removed = true;
            } else {
                ++it;
            }
        }
    }
}

// Removes all outgoing and incoming edges incident to n.
void IncidenceListGraph::remove_edges_incident_to_(Node* n) {
    if (!n) return;

    std::vector<Edge*> to_delete;

    // Collect outgoing edges from n
    for (Edge* e : n->outgoing) {
        if (e) to_delete.push_back(e);
    }
    n->outgoing.clear();

    // Remove incoming edges to n from other nodes' outgoing lists
    for (Node* m : nodes_) {
        if (!m || m == n) continue;

        auto& out = m->outgoing;
        out.erase(std::remove_if(out.begin(), out.end(),
            [&](Edge* e) {
                if (e && e->to == n) {
                    to_delete.push_back(e);
                    return true;
                }
                return false;
            }), out.end());
    }

    // Deduplicate in case the same edge was collected more than once
    std::sort(to_delete.begin(), to_delete.end());
    to_delete.erase(std::unique(to_delete.begin(), to_delete.end()), to_delete.end());

    // Remove from graph ownership and delete
    for (Edge* e : to_delete) {
        edges_.erase(std::remove(edges_.begin(), edges_.end(), e), edges_.end());
        delete e;
    }
}