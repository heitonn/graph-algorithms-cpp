#include "TarjanSCC.h"
#include <algorithm>
#include <iostream>


// Constructor: stores a reference to the graph, the verbose flag,
// and initializes the running DFS index to 0.
TarjanSCC::TarjanSCC(const IGraph& graph, bool verbose)
    : graph_(graph), verbose_(verbose), current_index_(0) {}


// Runs Tarjan's algorithm on all nodes in the graph.
// if a node has not yet been assigned an index, it has not been visited,
// so we start a DFS from that node.
std::vector<std::vector<std::string>> TarjanSCC::run() {
    for (const auto& node : graph_.get_nodes()) {
        if (index_.find(node) == index_.end()) {
            strongconnect(node);
        }
    }
    return sccs_;
}

// Recursive DFS function for Tarjan's algorithm.
// It assigns an index and lowlink value to v, explores its neighbors,
// and identifies a strongly connected component when v is a root node.
void TarjanSCC::strongconnect(const std::string& v) {
    // Assign discovery index and initialize lowlink to the same value.
    index_[v] = current_index_;
    lowlink_[v] = current_index_;
    current_index_++;

    // Push v to stack and mark as currently active
    stack_.push_back(v);
    on_stack_[v] = true;

    // In verbose mode, print the node when it is first visited.
    if (verbose_) {
        std::cout << "Visit: " << v
                  << " index=" << index_[v]
                  << " lowlink=" << lowlink_[v] << "\n";
    }

    
    // Finding all outgoing neighbors of v.
    for (const auto& w : graph_.get_neighbors(v)) {
         // If w has not been visited, recurse on it.
        // After returning, update lowlink[v] using lowlink[w].
        if (index_.find(w) == index_.end()) {
            strongconnect(w);
            lowlink_[v] = std::min(lowlink_[v], lowlink_[w]);
        } 
        // If w is on the stack, then w is part of the current DFS.
        // Updates lowlink[v] with index[w].
        else if (on_stack_[w]) {
            lowlink_[v] = std::min(lowlink_[v], index_[w]);
        }
    }

    // If v is the root of an SCC, pop nodes from the stack
    // until v is reached. These nodes form one SCC.
    if (lowlink_[v] == index_[v]) {
        std::vector<std::string> component;

        while (true) {
            std::string w = stack_.back();
            stack_.pop_back();
            on_stack_[w] = false;
            component.push_back(w);

            if (w == v) break;
        }

        sccs_.push_back(component);

        // In verbose mode, print the discovered SCC.
        if (verbose_) {
            std::cout << "SCC found: ";
            for (const auto& x : component) {
                std::cout << x << " ";
            }
            std::cout << "\n";
        }
    }
}