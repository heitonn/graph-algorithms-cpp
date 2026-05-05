#include "DiamondQuery.h"
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <algorithm>

// Constructor, stores reference to the graph and the verbose flag.
DiamondQuery::DiamondQuery(const IGraph& graph, bool verbose)
    : graph_(graph), verbose_(verbose) {}

// Parses one line from query file 
// Each line contains a sequence of edge labels and ending with a period
// ex: "r0 r2 r4."
std::vector<std::string> DiamondQuery::parse_query_line_(const std::string& line) const {
    std::vector<std::string> labels;
    std::stringstream ss(line);
    std::string token;

    while (ss >> token) {
        // Removing final "." from the last label (if present)
        if (!token.empty() && token.back() == '.') {
            token.pop_back();
        }
        labels.push_back(token);
    }

    return labels;
}

// REads the query file and stores the two query paths. 
// File must contain exactly two lines, representing the two branches 
// of the dimanond shaped pattern
void DiamondQuery::read_query_file(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        throw std::runtime_error("Could not open query file: " + filename);
    }

    std::string line1, line2;

    // Read the two query lines.
    if (!std::getline(in, line1) || !std::getline(in, line2)) {
        throw std::runtime_error("Query file must contain exactly two lines.");
    }

    path1_ = parse_query_line_(line1);
    path2_ = parse_query_line_(line2);

    // A dimaond query requires both paths to have the same length.
    if (path1_.size() != path2_.size()) {
        throw std::runtime_error("The two query paths must have the same length.");
    }

    // in verbose mode, pront the parsed query paths 
    if (verbose_) {
        std::cout << "Path 1: ";
        for (const auto& x : path1_) std::cout << x << " ";
        std::cout << "\n";

        std::cout << "Path 2: ";
        for (const auto& x : path2_) std::cout << x << " ";
        std::cout << "\n";
    }
}

// Starting from one node, follow a sequence of edge labels step by step.
// After each label, the set of reachable nodes is updated.
// The returned vector contains all nodes reachable from start_node
// by following exactly the given label sequence.
std::vector<std::string> DiamondQuery::follow_path_from_(
    const std::string& start_node,
    const std::vector<std::string>& labels) const {

    // initially only the starting node is reachable
    std::vector<std::string> current{start_node};

    for (const auto& label : labels) {
        std::vector<std::string> next;
 
        // For every currently reachable node, collect all neighbors
        // reachable by an edge with the required label.
        for (const auto& node : current) {
            auto neighbors = graph_.get_neighbors_by_label(node, label);
            next.insert(next.end(), neighbors.begin(), neighbors.end());
        }

        // Remove duplicates so that each reachable node appears only once.
        std::sort(next.begin(), next.end());
        next.erase(std::unique(next.begin(), next.end()), next.end());

        current = next;
        // If no nodes are reachable at some step, the path cannot continue.
        if (current.empty()) {
            break;
        }
    }

    return current;
}

// Runs the diamond query on the graph.
// For each start node, we follow both query paths and compute the
// intersection of their reachable end nodes. Each common end node gives
// a diamond pair (start, end).
std::vector<std::pair<std::string, std::string>> DiamondQuery::run() {
    std::vector<std::pair<std::string, std::string>> result;

    for (const auto& start : graph_.get_nodes()) {
        // Find all end nodes reachable from start along path 1 and path 2.
        auto reachable1 = follow_path_from_(start, path1_);
        auto reachable2 = follow_path_from_(start, path2_);

        // Sort the vectors before computing the intersection.
        std::sort(reachable1.begin(), reachable1.end());
        std::sort(reachable2.begin(), reachable2.end());

        std::vector<std::string> intersection;
        std::set_intersection(
            reachable1.begin(), reachable1.end(),
            reachable2.begin(), reachable2.end(),
            std::back_inserter(intersection)
        );

        // Every common end node completes a diamond-shaped path pattern
        // from start to end.
        for (const auto& end : intersection) {
            result.push_back({start, end});

            if (verbose_) {
                std::cout << "Diamond pair found: (" << start << ", " << end << ")\n";
            }
        }
    }

    return result;
}