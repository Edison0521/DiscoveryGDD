#ifndef GRAPH_GRAPH
#define GRAPH_GRAPH

#include <queue>
#include <tuple>
#include <vector>
#include "types.h"
#include "utils.h"
#include <map>
#include <stack>
#include <unordered_set>
#include <iostream>

class DFSEdge {
public:
    uint fromLabel;
    uint edgeLabel;
    uint toLabel;
    uint fromId;
    uint toId;

    DFSEdge(uint fLabel, uint eLabel, uint tLabel, uint fId, uint tId);
    bool operator<(const DFSEdge& other) const {
        if (fromLabel != other.fromLabel) return fromLabel < other.fromLabel;
        if (edgeLabel != other.edgeLabel) return edgeLabel < other.edgeLabel;
        if (toLabel != other.toLabel) return toLabel < other.toLabel;
        if (fromId != other.fromId) return fromId < other.fromId;
        return toId < other.toId;
    }
};

class Graph
{
protected:
    uint edge_count_;
    uint vlabel_count_;
    uint elabel_count_;
    mutable std::unordered_map<uint, uint> vlabel_num; 
    mutable std::unordered_map<uint, uint> elabel_num; 
    std::vector<std::vector<uint>> neighbors_;
    std::vector<std::vector<uint>> elabels_;
    std::map<std::tuple<uint, uint, uint>, uint> edgeTypeCount_;

public:
    std::queue<InsertUnit> updates_;
    std::vector<uint> vlabels_;

public:
    Graph();
    uint GetMaxVertexId() const {
        if (vlabels_.empty()) return 0;
        return vlabels_.size() - 1; // 假设顶点的id是从0开始的并且是连续的
    }
    virtual uint NumVertices() const { return vlabels_.size(); }
    virtual uint NumEdges() const { return edge_count_; }
    uint NumVLabels() const { return vlabel_count_; }
    uint NumELabels() const { return elabel_count_; }
    uint GetDiameter() const;

    void AddVertex(uint id, uint label);
    void RemoveVertex(uint id);
    void AddEdge(uint v1, uint v2, uint label);
    void RemoveEdge(uint v1, uint v2);

    uint GetVertexLabel(uint u) const;
    const std::vector<uint>& GetNeighbors(uint v) const;
    const std::vector<uint>& GetNeighborLabels(uint v) const;
    uint GetDegree(uint v) const;
    std::tuple<uint, uint, uint> GetEdgeLabel(uint v1, uint v2) const;

    void LoadFromFile(const std::string &path);
    void LoadUpdateStream(const std::string &path);
    void PrintMetaData() const;
    void AddEdgeAndUpdateCount(uint v1, uint v2, uint label);
    void RemoveEdgeAndUpdateCount(uint v1, uint v2, uint label);
    uint GetEdgeCount(uint v1, uint v2, uint label) const;
    bool EdgeExists(uint v1, uint v2) const;
    void RemoveEdgeLabels(uint v1, uint v2);
    void RemoveVerticesWithInvalidLabel();
    // 其他成员函数和数据成员...
    // Graph DFS() const;
    // void DFSUtil(uint vertex, std::unordered_set<uint>& visited) const;
    // void AddVertexToSubgraph(uint vertex, Graph& subgraph) const;
    // void AddEdgeToSubgraph(uint v1, uint v2, Graph& subgraph) const;
    std::vector<Graph> ExpandByEdges() const;
    void DFSFromEdge(uint v1, uint v2, Graph& subgraph, std::unordered_set<uint>& visited) const;
    void Print() const;
    std::vector<DFSEdge> gSpanDFSEncoding() const;
    std::vector<DFSEdge> getMinimumDFSEncoding() const;
    // bool isFrequent(const std::vector<DFSEdge>& dfSEncoding, const std::vector<Graph>& dataset);
    // Graph buildSubgraphFromDFSEncoding(const std::vector<DFSEdge>& dfSEncoding);
    // void extendSubgraph(const Graph& data_graph, const std::vector<DFSEdge>& currentDFSEncoding);
    // bool isInCurrentSubgraph(uint vertexId, const std::vector<DFSEdge>& dfSEncoding);
    // std::unordered_set<uint> getBoundaryVertices(const std::vector<DFSEdge>& dfSEncoding);

private:
    void gSpanDFS(uint v, int prev, std::vector<bool>& visited, std::vector<DFSEdge>& encoding) const;
};


#endif //GRAPH_GRAPH
