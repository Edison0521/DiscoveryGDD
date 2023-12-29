#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <tuple>
#include <vector>
#include <map>
#include "types.h"
#include "utils.h"
#include "graph.h"
#include <unordered_map>
#include <stack> // 用于 std::cout 和 std::endl
#include <unordered_set> // 用于 std::unordered_set
 // 用于 std::stack
 // 用于 std::vector
using namespace std;
Graph::Graph()
: edge_count_(0)
, vlabel_count_(0)
, elabel_count_(0)
, neighbors_{}
, elabels_{}
, updates_{}
, vlabels_{}
{}

void Graph::AddVertex(uint id, uint label)
{
    if (id >= vlabels_.size())
    {
        vlabels_.resize(id + 1, NOT_EXIST);
        vlabels_[id] = label;
        neighbors_.resize(id + 1);
        elabels_.resize(id + 1);
        vlabel_num[label]++; 
    }
    else if (vlabels_[id] == NOT_EXIST)
    {
        vlabels_[id] = label;
        vlabel_num[label]++; 
    }
    
    vlabel_count_ = std::max(vlabel_count_, label + 1);
}

void Graph::RemoveVertex(uint id)
{
    vlabels_[id] = NOT_EXIST;
    neighbors_[id].clear();
    elabels_[id].clear();
}

void Graph::AddEdge(uint v1, uint v2, uint label)
{
    auto lower = std::lower_bound(neighbors_[v1].begin(), neighbors_[v1].end(), v2);
    if (lower != neighbors_[v1].end() && *lower == v2) return;
    
    size_t dis = std::distance(neighbors_[v1].begin(), lower);
    neighbors_[v1].insert(lower, v2);
    elabels_[v1].insert(elabels_[v1].begin() + dis, label);
    
    lower = std::lower_bound(neighbors_[v2].begin(), neighbors_[v2].end(), v1);
    dis = std::distance(neighbors_[v2].begin(), lower);
    neighbors_[v2].insert(lower, v1);
    elabels_[v2].insert(elabels_[v2].begin() + dis, label);

    edge_count_++;
    elabel_num[label]++;
    elabel_count_ = std::max(elabel_count_, label + 1);
}

void Graph::RemoveEdge(uint v1, uint v2)
{
    auto lower = std::lower_bound(neighbors_[v1].begin(), neighbors_[v1].end(), v2);
    if (lower == neighbors_[v1].end() || *lower != v2)
    {
        std::cout << "deletionv1 error" << std::endl;
        exit(-1);
    }
    neighbors_[v1].erase(lower);
    elabels_[v1].erase(elabels_[v1].begin() + std::distance(neighbors_[v1].begin(), lower));
    
    lower = std::lower_bound(neighbors_[v2].begin(), neighbors_[v2].end(), v1);
    if (lower == neighbors_[v2].end() || *lower != v1)
    {
        std::cout << "deletionv2 error" << std::endl;
        exit(-1);
    }
    neighbors_[v2].erase(lower);
    elabels_[v2].erase(elabels_[v2].begin() + std::distance(neighbors_[v2].begin(), lower));

    edge_count_--;
}

uint Graph::GetVertexLabel(uint u) const
{
    return vlabels_[u];
}

const std::vector<uint>& Graph::GetNeighbors(uint v) const
{
    return neighbors_[v];
}

const std::vector<uint>& Graph::GetNeighborLabels(uint v) const
{
    return elabels_[v];
}

std::tuple<uint, uint, uint> Graph::GetEdgeLabel(uint v1, uint v2) const
{
    uint v1_label, v2_label, e_label;
    v1_label = GetVertexLabel(v1);
    v2_label = GetVertexLabel(v2);

    const std::vector<uint> *nbrs;
    const std::vector<uint> *elabel;
    uint other;
    if (GetDegree(v1) < GetDegree(v2))
    {
        nbrs = &GetNeighbors(v1);
        elabel = &elabels_[v1];
        other = v2;
    }
    else
    {
        nbrs = &GetNeighbors(v2);
        elabel = &elabels_[v2];
        other = v1;
    }
    
    long start = 0, end = nbrs->size() - 1, mid;
    while (start <= end)
    {
        mid = (start + end) / 2;
        if (nbrs->at(mid) < other)
        {
            start = mid + 1;
        }
        else if (nbrs->at(mid) > other)
        {
            end = mid - 1;
        }
        else
        {
            e_label = elabel->at(mid);
            return {v1_label, v2_label, e_label};
        }
    }
    return {v1_label, v2_label, -1};
}

uint Graph::GetDegree(uint v) const
{
    return neighbors_[v].size();
}

uint Graph::GetDiameter() const
{
    uint diameter = 0;
    for (uint i = 0u; i < NumVertices(); i++)
    if (GetVertexLabel(i) != NOT_EXIST)
    {
        std::queue<uint> bfs_queue;
        std::vector<bool> visited(NumVertices(), false);
        uint level = UINT_MAX;
        bfs_queue.push(i);
        visited[i] = true;
        while (!bfs_queue.empty())
        {
            level++;
            uint size = bfs_queue.size();
            for (uint j = 0u; j < size; j++)
            {
                uint front = bfs_queue.front();
                bfs_queue.pop();

                const auto& nbrs = GetNeighbors(front);
                for (const uint nbr: nbrs)
                {
                    if (!visited[nbr])
                    {
                        bfs_queue.push(nbr);
                        visited[nbr] = true;
                    }
                }
            }
        }
        if (level > diameter) diameter = level;
    }
    return diameter;
}

void Graph::LoadFromFile(const std::string &path)
{
    if (!io::file_exists(path.c_str()))
    {
        std::cout << "Failed to open: " << path << std::endl;
        exit(-1);
    }
    std::ifstream ifs(path);

    char type;
    while (ifs >> type)
    {
        if (type == 't')
        {
            char temp1;
            uint temp2;
            ifs >> temp1 >> temp2;
        }
        else if (type == 'v')
        {
            uint vertex_id, label;
            ifs >> vertex_id >> label;
            AddVertex(vertex_id, label);
        }
        else
        {
            uint from_id, to_id, label;
            ifs >> from_id >> to_id >> label;
            AddEdge(from_id, to_id, label);
        }
    }
    ifs.close();
}

void Graph::LoadUpdateStream(const std::string &path)
{
    if (!io::file_exists(path.c_str()))
    {
        std::cout << "Failed to open: " << path << std::endl;
        exit(-1);
    }
    std::ifstream ifs(path);

    std::string type;
    while (ifs >> type)
    {
        if (type == "v" || type == "-v")
        {
            uint vertex_id, label;
            ifs >> vertex_id >> label;
            updates_.emplace('v', type == "v", vertex_id, 0u, label);
        }
        else
        {
            uint from_id, to_id, label;
            ifs >> from_id >> to_id >> label;
            updates_.emplace('e', type == "e", from_id, to_id, label);
        }
    }
    ifs.close();
}

void Graph::PrintMetaData() const
{
    std::cout << "# vertices = " << NumVertices() <<
        "\n# edges = " << NumEdges() << std::endl;
    
}

void Graph::AddEdgeAndUpdateCount(uint v1, uint v2, uint label) {
    // 添加边
    // ...

    // 更新边类型计数
    std::tuple<uint, uint, uint> edgeType = std::make_tuple(v1, v2, label);
    if (edgeTypeCount_.find(edgeType) == edgeTypeCount_.end()) {
        edgeTypeCount_[edgeType] = 1;
    } else {
        edgeTypeCount_[edgeType]++;
    }
}

// 在删除边时更新边类型计数
// void Graph::RemoveEdgeAndUpdateCount(uint v1, uint v2) {
//    // 找到v1的邻居列表中v2的位置
//     auto it1 = std::find(neighbors_[v1].begin(), neighbors_[v1].end(), v2);
//     if (it1 != neighbors_[v1].end()) {
//         // 找到v2的邻居列表中v1的位置
//         auto it2 = std::find(neighbors_[v2].begin(), neighbors_[v2].end(), v1);

//         // 获取v1和v2之间的边的标签
//         uint edge_label = elabels_[v1][it1 - neighbors_[v1].begin()];

//         // 删除v1的邻居列表中v2
//         neighbors_[v1].erase(it1);
//         // 删除v2的邻居列表中v1
//         neighbors_[v2].erase(it2);

//         // 删除相应的边标签
//         elabels_[v1].erase(elabels_[v1].begin() + (it1 - neighbors_[v1].begin()));
//         elabels_[v2].erase(elabels_[v2].begin() + (it2 - neighbors_[v2].begin()));

//         // 更新边计数
//         edge_count_--;

//         // 更新边标签计数
//         elabel_num[edge_label]--;
//     }
// }

// 获取特定类型边的数量
uint Graph::GetEdgeCount(uint v1, uint v2, uint label) const {
    std::tuple<uint, uint, uint> edgeType = std::make_tuple(v1, v2, label);
    //cout<<label<<"-----------------------"<<elabel_num[label]<<endl;
    return elabel_num[label];
}   
bool Graph::EdgeExists(uint v1, uint v2) const {
    if (v1 >= neighbors_.size() || v2 >= neighbors_.size()) {
        // 顶点 v1 或 v2 不存在
        return false;
    }

    const std::vector<uint>& neighbors = neighbors_[v1];
    for (const uint& neighbor : neighbors) {
        if (neighbor == v2) {
            // 找到了边 v1 -> v2
            return true;
        }
    }

    // 没有找到边 v1 -> v2
    return false;
}
void Graph::RemoveEdgeLabels(uint v1, uint v2) {
    // 找到v1的邻居列表中v2的位置
    auto it1 = std::find(neighbors_[v1].begin(), neighbors_[v1].end(), v2);
    if (it1 != neighbors_[v1].end()) {
        // 找到v2的邻居列表中v1的位置
        auto it2 = std::find(neighbors_[v2].begin(), neighbors_[v2].end(), v1);

        // 获取v1和v2之间的边的标签
        uint edge_label = elabels_[v1][it1 - neighbors_[v1].begin()];

        // 删除v1的邻居列表中v2
        neighbors_[v1].erase(it1);
        // 删除v2的邻居列表中v1
        neighbors_[v2].erase(it2);

        // 删除相应的边标签
        elabels_[v1].erase(elabels_[v1].begin() + (it1 - neighbors_[v1].begin()));
        elabels_[v2].erase(elabels_[v2].begin() + (it2 - neighbors_[v2].begin()));

        // 更新边计数
        edge_count_--;

        // 更新边标签计数
        elabel_num[edge_label]--;
    }
}
// void Graph::DFS() const {
//     std::unordered_set<uint> visited;
//     std::stack<uint> stack;

//     for (uint v = 0; v < NumVertices(); ++v) {
//         if (visited.find(v) == visited.end()) {
//             stack.push(v);

//             while (!stack.empty()) {
//                 uint vertex = stack.top();
//                 stack.pop();

//                 if (visited.find(vertex) == visited.end()) {
//                     std::cout << "Visiting vertex: " << vertex << " with label: " << GetVertexLabel(vertex) << std::endl;
//                     visited.insert(vertex);

//                     const std::vector<uint>& neighbors = GetNeighbors(vertex);
//                     for (uint neighbor : neighbors) {
//                         if (visited.find(neighbor) == visited.end()) {
//                             stack.push(neighbor);
//                         }
//                     }
//                 }
//             }
//         }
//     }
// }
// void Graph::DFS() const {
//     std::unordered_set<uint> visited;
//     std::stack<uint> stack;

//     for (uint v = 0; v < NumVertices(); ++v) {
//         if (visited.find(v) == visited.end()) {
//             stack.push(v);

//             while (!stack.empty()) {
//                 uint vertex = stack.top();
//                 stack.pop();

//                 if (visited.find(vertex) == visited.end()) {
                    
//                     visited.insert(vertex);

//                     const std::vector<uint>& neighbors = GetNeighbors(vertex);
//                     for (uint neighbor : neighbors) {
//                         if (visited.find(neighbor) == visited.end() && EdgeExists(vertex, neighbor)) {
//                             std::cout << "Visiting vertex: " << vertex << " with label: " << GetVertexLabel(vertex) << std::endl;
//                             std::tuple<unsigned int, unsigned int, unsigned int> edgeLabel = GetEdgeLabel(vertex, neighbor);
//                             std::cout << "Visiting edge: (" << vertex << ", " << neighbor << ") with label: (" 
//                                     << std::get<0>(edgeLabel) << ", " << std::get<1>(edgeLabel) << ", " << std::get<2>(edgeLabel) << ")" << std::endl;

//                             //std::cout << "Visiting edge: (" << vertex << ", " << neighbor << ") with label: " << GetEdgeLabel(vertex, neighbor) << std::endl;
//                             stack.push(neighbor);
//                         }
//                     }
//                 }
//             }
//         }
//     }
// }
// void Graph::DFS() const {
//     std::unordered_set<uint> visited;
//     std::stack<uint> stack;
//     for (uint v = 0; v < NumVertices(); ++v) {
//         if (visited.find(v) == visited.end()) {
//             stack.push(v);
//             while (!stack.empty()) {
//                 uint vertex = stack.top();
//                 stack.pop();
//                 if (visited.find(vertex) == visited.end()) {
//                     visited.insert(vertex);
//                     const std::vector<uint>& neighbors = GetNeighbors(vertex);
//                     for (uint neighbor : neighbors) {
//                         if (visited.find(neighbor) == visited.end() && EdgeExists(vertex, neighbor)) {
//                             std::cout << "Visiting vertex: " << vertex << " with label: " << GetVertexLabel(vertex) << std::endl;
//                             std::tuple<unsigned int, unsigned int, unsigned int> edgeLabel = GetEdgeLabel(vertex, neighbor);
//                             std::cout << "Visiting edge: (" << vertex << ", " << neighbor << ") with label: (" 
//                                       << std::get<0>(edgeLabel) << ", " << std::get<1>(edgeLabel) << ", " << std::get<2>(edgeLabel) << ")" << std::endl;
//                             stack.push(neighbor);
//                         }
//                     }
//                 }
//             }
//         }
//     }
// }
// void Graph::DFSUtil(uint vertex, std::unordered_set<uint>& visited) const {
//     visited.insert(vertex);
//     std::cout << "Visiting vertex: " << vertex << " with label: " << GetVertexLabel(vertex) << std::endl;

//     const std::vector<uint>& neighbors = GetNeighbors(vertex);
//     for (uint neighbor : neighbors) {
//         if (visited.find(neighbor) == visited.end() && EdgeExists(vertex, neighbor)) {
//             std::tuple<unsigned int, unsigned int, unsigned int> edgeLabel = GetEdgeLabel(vertex, neighbor);
//             std::cout << "Visiting edge: (" << vertex << ", " << neighbor << ") with label: (" 
//                       << std::get<0>(edgeLabel) << ", " << std::get<1>(edgeLabel) << ", " << std::get<2>(edgeLabel) << ")" << std::endl;
//             DFSUtil(neighbor, visited);
//         }
//     }
// }

// void Graph::DFS() const {
//     std::unordered_set<uint> visited;
//     for (uint v = 0; v < NumVertices(); ++v) {
//         if (visited.find(v) == visited.end()) {
//             DFSUtil(v, visited);
//         }
//     }
// }
// Graph Graph::DFS() const {
//     Graph subgraph; // 创建一个新的图对象来存储子图
//     std::unordered_set<uint> visited;

//     std::function<void(uint)> dfsUtil = [&](uint vertex) {
//         visited.insert(vertex);
//         AddVertexToSubgraph(vertex, subgraph); // 将顶点添加到子图中

//         const std::vector<uint>& neighbors = GetNeighbors(vertex);
//         for (uint neighbor : neighbors) {
//             if (visited.find(neighbor) == visited.end() && EdgeExists(vertex, neighbor)) {
//                 AddEdgeToSubgraph(vertex, neighbor, subgraph); // 将边添加到子图中
//                 dfsUtil(neighbor);
//             }
//         }
//     };

//     for (uint v = 0; v < NumVertices(); ++v) {
//         if (visited.find(v) == visited.end()) {
//             dfsUtil(v);
//         }
//     }

//     return subgraph; // 返回子图
// }

// void Graph::AddVertexToSubgraph(uint vertex, Graph& subgraph) const {
//     uint label = GetVertexLabel(vertex);
//     subgraph.AddVertex(vertex, label);
// }

// void Graph::AddEdgeToSubgraph(uint v1, uint v2, Graph& subgraph) const {
//     std::tuple<uint, uint, uint> edgeLabel = GetEdgeLabel(v1, v2);
//     subgraph.AddEdge(v1, v2, std::get<2>(edgeLabel));
// }
std::vector<Graph> Graph::ExpandByEdges() const {
    std::vector<Graph> subgraphs;
    std::unordered_set<uint> visited;

    for (uint v = 0; v < NumVertices(); ++v) {
        const std::vector<uint>& neighbors = GetNeighbors(v);
        for (uint neighbor : neighbors) {
            if (EdgeExists(v, neighbor)) {
                Graph subgraph;
                DFSFromEdge(v, neighbor, subgraph, visited);
                subgraphs.push_back(subgraph);
                visited.clear();
            }
        }
    }

    return subgraphs;
}

// void Graph::DFSFromEdge(uint v1, uint v2, Graph& subgraph, std::unordered_set<uint>& visited) const {
//     std::stack<uint> stack;
//     stack.push(v2);
//     stack.push(v1);

//     while (!stack.empty()) {
//         uint vertex = stack.top();
//         stack.pop();

//         if (visited.find(vertex) == visited.end()) {
//             visited.insert(vertex);
//             subgraph.AddVertex(vertex, GetVertexLabel(vertex));

//             const std::vector<uint>& neighbors = GetNeighbors(vertex);
//             for (uint neighbor : neighbors) {
//                 if (visited.find(neighbor) == visited.end() && EdgeExists(vertex, neighbor)) {
//                     subgraph.AddVertex(neighbor,GetVertexLabel(neighbor));
//                     subgraph.AddEdge(vertex, neighbor, std::get<2>(GetEdgeLabel(vertex, neighbor)));
//                     stack.push(neighbor);
//                 }
//             }
//         }
//     }
// }
void Graph::DFSFromEdge(uint v1, uint v2, Graph& subgraph, std::unordered_set<uint>& visited) const {
    std::stack<uint> stack;
    stack.push(v2);
    stack.push(v1);

    while (!stack.empty()) {
        uint vertex = stack.top();
        stack.pop();

        if (visited.find(vertex) == visited.end() && GetVertexLabel(vertex) != NOT_EXIST) {
            visited.insert(vertex);
            subgraph.AddVertex(vertex, GetVertexLabel(vertex));

            const std::vector<uint>& neighbors = GetNeighbors(vertex);
            for (uint neighbor : neighbors) {
                std::cout<<neighbor<<" , "<<GetVertexLabel(neighbor)<<std::endl;
                if (visited.find(neighbor) == visited.end() && EdgeExists(vertex, neighbor) && GetVertexLabel(neighbor) != NOT_EXIST) {
                    subgraph.AddVertex(neighbor,int(GetVertexLabel(neighbor)));
                    subgraph.AddEdge(vertex, neighbor, std::get<2>(GetEdgeLabel(vertex, neighbor)));
                    stack.push(neighbor);
                }
            }
        }
    }
}
void Graph::RemoveVerticesWithInvalidLabel() {
    // 使用uint(-1)表示无效标签
    const uint INVALID_LABEL = uint(-1);

    // 从后向前遍历顶点，这样在删除顶点时不会影响未检查的顶点的索引
    for (int i = NumVertices() - 1; i >= 0; --i) {
        if (GetVertexLabel(i) == INVALID_LABEL) {
            RemoveVertex(i);
        }
    }
}

void Graph::Print() const {
    std::cout << "Vertices:" << std::endl;
    for (uint i = 0; i < NumVertices(); ++i) {
        //if(GetVertexLabel(i) != uint(-1)){
        std::cout << "Vertex " << i << " with label: " << GetVertexLabel(i) << std::endl;
        //}
        // else{
        //     RemoveVertex(i);
        // }
    }

    std::cout << "Edges:" << std::endl;
    for (uint i = 0; i < NumVertices(); ++i) {
        const std::vector<uint>& neighbors = GetNeighbors(i);
        for (uint neighbor : neighbors) {
            if (EdgeExists(i, neighbor)) {
                std::tuple<uint, uint, uint> edgeLabel = GetEdgeLabel(i, neighbor);
                std::cout << "Edge (" << i << ", " << neighbor << ") with label: (" 
                          << std::get<0>(edgeLabel) << ", " << std::get<1>(edgeLabel) << ", " << std::get<2>(edgeLabel) << ")" << std::endl;
            }
        }
    }
}
// DFSEdge::DFSEdge(uint f, uint t, uint l) : from(f), to(t), label(l) {}
// std::vector<DFSEdge> Graph::DFSEncoding() const {
//     std::vector<DFSEdge> encoding;
//     std::vector<bool> visited(vlabels_.size(), false);
//     for (uint i = 0; i < vlabels_.size(); ++i) {
//         if (!visited[i] && vlabels_[i] != NOT_EXIST) {
//             DFS(i, visited, encoding);
//         }
//     }
//     return encoding;
// }

// void Graph::DFS(uint v, std::vector<bool>& visited, std::vector<DFSEdge>& encoding) const {
//     visited[v] = true;
//     for (uint i = 0; i < neighbors_[v].size(); ++i) {
//         uint neighbor = neighbors_[v][i];
//         if (!visited[neighbor]) {
//             encoding.push_back(DFSEdge(v, neighbor, elabels_[v][i]));
//             DFS(neighbor, visited, encoding);
//         }
//     }
// }
// ... [其他代码]

DFSEdge::DFSEdge(uint fLabel, uint eLabel, uint tLabel, uint fId, uint tId) 
    : fromLabel(fLabel), edgeLabel(eLabel), toLabel(tLabel), fromId(fId), toId(tId) {}

std::vector<DFSEdge> Graph::gSpanDFSEncoding() const {
    std::vector<DFSEdge> encoding;
    std::vector<bool> visited(vlabels_.size(), false);
    for (uint i = 0; i < vlabels_.size(); ++i) {
        if (!visited[i] && vlabels_[i] != NOT_EXIST) {
            gSpanDFS(i, -1, visited, encoding);
        }
    }
    return encoding;
}
void Graph::gSpanDFS(uint v, int prev, std::vector<bool>& visited, std::vector<DFSEdge>& encoding) const {
    visited[v] = true;
    for (uint i = 0; i < neighbors_[v].size(); ++i) {
        uint neighbor = neighbors_[v][i];
        if (!visited[neighbor]) {
            encoding.push_back(DFSEdge(vlabels_[v], elabels_[v][i], vlabels_[neighbor], v, neighbor));
            gSpanDFS(neighbor, v, visited, encoding);
        } else if (neighbor != prev) {
            // Backward edge
            encoding.push_back(DFSEdge(vlabels_[v], elabels_[v][i], vlabels_[neighbor], v, neighbor));
        }
    }
}
std::vector<DFSEdge> Graph::getMinimumDFSEncoding() const {
    std::vector<DFSEdge> minEncoding;

    for (uint i = 0; i < vlabels_.size(); ++i) {
        if (vlabels_[i] != NOT_EXIST) {
            std::vector<DFSEdge> currentEncoding;
            std::vector<bool> visited(vlabels_.size(), false);
            gSpanDFS(i, -1, visited, currentEncoding);

            if (minEncoding.empty() || currentEncoding < minEncoding) {
                minEncoding = currentEncoding;
            }
        }
    }

    return minEncoding;
}



// std::unordered_set<uint> getBoundaryVertices(const std::vector<DFSEdge>& dfSEncoding) {
//     std::unordered_set<uint> boundaryVertices;
//     std::unordered_set<uint> allVertices;

//     for (const DFSEdge& edge : dfSEncoding) {
//         allVertices.insert(edge.fromId);
//         allVertices.insert(edge.toId);
//     }

//     for (const DFSEdge& edge : dfSEncoding) {
//         if (allVertices.count(edge.toId) == 1) {
//             boundaryVertices.insert(edge.toId);
//         }
//     }

//     return boundaryVertices;
// }

// bool isInCurrentSubgraph(uint vertexId, const std::vector<DFSEdge>& dfSEncoding) {
//     for (const DFSEdge& edge : dfSEncoding) {
//         if (edge.fromId == vertexId || edge.toId == vertexId) {
//             return true;
//         }
//     }
//     return false;
// }
// void extendSubgraph(const Graph& data_graph, const std::vector<DFSEdge>& currentDFSEncoding) {
//     std::unordered_set<uint> boundaryVertices = getBoundaryVertices(currentDFSEncoding);

//     for (uint vertex : boundaryVertices) {
//         std::vector<uint> neighbors = data_graph.GetNeighbors(vertex);
//         for (uint neighbor : neighbors) {
//             if (!isInCurrentSubgraph(neighbor, currentDFSEncoding)) {
//                 std::vector<DFSEdge> newDFSEncoding = currentDFSEncoding;
//                 // uint edgeLabel = data_graph.GetEdgeLabel(vertex, neighbor);
//                 auto edgeLabelTuple = data_graph.GetEdgeLabel(vertex, neighbor);
//                 uint edgeLabel = std::get<2>(edgeLabelTuple);  // 假设第二个元素是您需要的
//                 newDFSEncoding.push_back(DFSEdge(data_graph.GetVertexLabel(vertex), edgeLabel, data_graph.GetVertexLabel(neighbor), vertex, neighbor));

//                 if (isFrequent(newDFSEncoding,data_graph)) {
//                     extendSubgraph(data_graph, newDFSEncoding);
//                 }
//             }
//         }
//     }
// }

// bool isFrequent(const std::vector<DFSEdge>& dfSEncoding, const std::vector<Graph>& dataset) {
//     // Step 1: Build the subgraph from the DFS encoding
//     Graph subgraph = buildSubgraphFromDFSEncoding(dfSEncoding);
//     Graph datagraph = dataset;
//     matching *mm = nullptr;
//     Graphflow *graphflow = nullptr;
//     // Step 2: Check the frequency of the subgraph in the dataset
//     // mm = graphflow = new Graphflow(subgraph, datagraph, max_num_results, print_prep, print_enum, homo); 
//     // mm->Preprocessing();
//     // std::cout << "----------- Initial Matching ----------" << std::endl; 
//     // auto InitialFun = [&mm]()
//     // {
//     //     mm->InitialMatching();
//     // };

        
//     // size_t num_results = 0ul;
//     // mm->GetNumInitialResults(num_results);
//     // std::cout << num_results << " matches. \n";

//     int frequency = 0;

//     return 1;  // Assuming min_sup is the minimum support threshold
// }

// Graph buildSubgraphFromDFSEncoding(const std::vector<DFSEdge>& dfSEncoding) {
//     Graph subgraph;
//     for (const DFSEdge& edge : dfSEncoding) {
//         if (!subgraph.vertexExists(edge.fromId)) {
//             subgraph.AddVertex(edge.fromId, edge.fromLabel);
//         }
//         if (!subgraph.vertexExists(edge.toId)) {
//             subgraph.AddVertex(edge.toId, edge.toLabel);
//         }
//         subgraph.AddEdge(edge.fromId, edge.toId, edge.edgeLabel);
//     }
//     return subgraph;
// }
