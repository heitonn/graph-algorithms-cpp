#pragma once
#include "IGraph.h"

#include <string>
#include <vector>
#include <unordered_map>

// Graph implementation based on incidence lists.
// Each node stores its outgoing edges, and the graph owns all nodes and edges.
class IncidenceListGraph : public IGraph {
public:
    IncidenceListGraph() = default;

    // Rule of five
    ~IncidenceListGraph();                                   // 1) destructor
    IncidenceListGraph(const IncidenceListGraph& other);     // 2) copy ctor
    IncidenceListGraph& operator=(const IncidenceListGraph& other); // 3) copy assign
    IncidenceListGraph(IncidenceListGraph&& other) noexcept; // 4) move ctor
    IncidenceListGraph& operator=(IncidenceListGraph&& other) noexcept; // 5) move assign

    // IGraph interface (abstract class)
    void insert_edge(const std::string& node_a_label,
                     const std::string& edge_label,
                     const std::string& node_b_label) override;

    void disconnect(const std::string& node_a_label,
                    const std::string& node_b_label) override;

    void remove_node(const std::string& node_label) override;

    // Read/write graph in triple notation:
    // node_a edge_label node_b.
    void read_from_file(const std::string& filename) override;
    void write_to_file(const std::string& filename) const override;

    // Traversal for Tarjans scc algorithm
    std::vector<std::string> get_nodes() const override;
    std::vector<std::string> get_neighbors(const std::string& node_label) const override;

    // Traversal for finiding diamons-shapes
    std::vector<std::string> get_neighbors_by_label(
    const std::string& node_label,
    const std::string& edge_label) const override;

private:
    struct Edge;

    struct Node {
        std::string label;               // unique node label
        std::vector<Edge*> outgoing;     // all outgoing edges from this node


        explicit Node(std::string lbl) : label(std::move(lbl)) {}
    };

    struct Edge {
        std::string label; // not unique
        Node* from = nullptr;
        Node* to   = nullptr;

        Edge(std::string lbl, Node* a, Node* b)
            : label(std::move(lbl)), from(a), to(b) {}
    };

    // Graph owns all nodes and edges (raw pointers -> memory mgmt / rule-of-five)
    std::vector<Node*> nodes_;
    std::vector<Edge*> edges_;

    // Returns the node with the given label, or nullptr if not found
    Node* find_node_(const std::string& label) const;

    // Returns an existing node, or creates it if missing
    Node* get_or_create_node_(const std::string& label);

    // Deletes all nodes and edges owned by the graph 
    void clear_(); 

    // Deep-copy helper used by copy constructor and copy assignment
    void deep_copy_from_(const IncidenceListGraph& other);

    // Removes nodes with no incoming or outgoing edges
    void remove_isolated_nodes_();

    // Checks whether a node has neither incoming nor outgoing edges
    bool is_isolated_(Node* n) const;

    // Removes all incoming and outgoing edges incident to a node
    void remove_edges_incident_to_(Node* n); // remove incoming+outgoing edges touching n
};