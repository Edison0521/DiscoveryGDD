#ifndef GRAPH_PROCESSING_H
#define GRAPH_PROCESSING_H

#include "graph.h" // 包含图的定义文件
#include <map>
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <vector>
#include "graphflow.h"
#include <unordered_set>

// 声明函数，统计并删除边
Graph ProcessGraph(const Graph& inputGraph, uint threshold);
// void dfs(Graph& data_graph, Graphflow& graphflow, uint v, uint threshold, std::unordered_set<size_t>& visitedSubgraphs);
// void runGraphflowOnGraph(Graph& data_graph, uint max_num_results,
//                          bool print_prep, bool print_enum, bool homo, uint threshold);

#endif // GRAPH_PROCESSING_H
