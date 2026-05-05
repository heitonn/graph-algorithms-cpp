#pragma once

#include "IGraph.h"
#include <string>
#include <vector>
#include <utility>

class DiamondQuery {
public:
    DiamondQuery(const IGraph& graph, bool verbose = false);

    void read_query_file(const std::string& filename);

    std::vector<std::pair<std::string, std::string>> run();

private:
    const IGraph& graph_;
    bool verbose_;

    std::vector<std::string> path1_;
    std::vector<std::string> path2_;

    std::vector<std::string> parse_query_line_(const std::string& line) const;

    std::vector<std::string> follow_path_from_(
        const std::string& start_node,
        const std::vector<std::string>& labels) const;
};