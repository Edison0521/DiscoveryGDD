#include "graph.h"
#include <map>
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include "graphflow.h"
#include <iostream>
#include <unordered_set>

using namespace std;
Graph ProcessGraph(const Graph& inputGraph, uint threshold) {
    bool hasDeletedEdges = true;
    Graph processedGraph = inputGraph;
    while (hasDeletedEdges) {
        hasDeletedEdges = false;

        for (uint v1 = 0; v1 < processedGraph.NumVertices(); ++v1) {
            const std::vector<uint>& neighbors = processedGraph.GetNeighbors(v1);
            const std::vector<uint>& neighborLabels = processedGraph.GetNeighborLabels(v1);

            for (size_t i = 0; i < neighbors.size(); ++i) {
                uint v2 = neighbors[i];
                uint edgeLabel = neighborLabels[i];
                uint edgeCount = processedGraph.GetEdgeCount(v1, v2, edgeLabel);

                if (edgeCount < threshold) {
                    //cout << v1 << "----" << v2 << "----" << edgeLabel << "-----" << edgeCount << endl;
                    processedGraph.RemoveEdge(v1, v2);
                    hasDeletedEdges = true;
                }
            }
        }
    }

    return processedGraph;
}

