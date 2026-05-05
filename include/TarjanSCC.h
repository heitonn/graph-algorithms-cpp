#pragma once

#include "IGraph.h"
#include <vector>
#include <string>
#include <unordered_map>

class TarjanSCC {
public:
    TarjanSCC(const IGraph& graph, bool verbose = false);

    std::vector<std::vector<std::string>> run();

private:
    const IGraph& graph_;
    bool verbose_;
    int current_index_;

    std::unordered_map<std::string, int> index_;
    std::unordered_map<std::string, int> lowlink_;
    std::unordered_map<std::string, bool> on_stack_;
    std::vector<std::string> stack_;
    std::vector<std::vector<std::string>> sccs_;

    void strongconnect(const std::string& v);
};