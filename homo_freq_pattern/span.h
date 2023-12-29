// gspan.h

#ifndef GSPAN_H
#define GSPAN_H

#include "graph.h"
#include <vector>
#include <map>

class GSpan {
public:
    GSpan(const Graph& graph, uint min_support);

    void Run();
    const std::vector<Graph>& GetFrequentSubgraphs() const;

private:
    const Graph& graph_;
    uint min_support_;
    std::vector<Graph> frequent_subgraphs_;

    void DFS(const Graph& subgraph);
    uint CalculateSupport(const Graph& subgraph);
};
class DFSCode {
public:
    struct Edge {
        uint from;
        uint to;
        uint from_label;
        uint edge_label;
        uint to_label;
    };

    void AddEdge(const Edge& edge);
    const std::vector<Edge>& GetEdges() const;

private:
    std::vector<Edge> edges_;
};

#endif // GSPAN_H
