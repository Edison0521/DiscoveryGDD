#include <chrono>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <thread>
#include "CLI11.hpp"
#include "globals.h"
#include "types.h"
#include "graph.h"
#include "matching.h"
#include "graphflow.h"
#include "graph_processing.h"


// Graph buildSubgraphFromDFSEncoding(const std::vector<DFSEdge>& dfSEncoding) {
//     Graph subgraph;
//     for (const DFSEdge& edge : dfSEncoding) {
//         subgraph.AddVertex(edge.fromId, edge.fromLabel);
//         subgraph.AddVertex(edge.toId, edge.toLabel);
//         subgraph.AddEdge(edge.fromId, edge.toId, edge.edgeLabel);
//     }
//     return subgraph;
// }

Graph buildSubgraphFromDFSEncoding(const Graph& originalGraph, const DFSEdge& edge) {
    Graph subgraph;
    uint maxOriginalVertexId = originalGraph.GetMaxVertexId();
    uint newVertexId = maxOriginalVertexId + 1;
    // subgraph.AddVertex(newVertexId, edge.fromLabel);
    // subgraph.AddVertex(newVertexId, edge.toLabel);
    // subgraph.AddEdge(edge.fromId, edge.toId, edge.edgeLabel);
    //for (const DFSEdge& edge : dfSEncoding) {
        if (edge.fromLabel != 4294967295){
            subgraph.AddVertex(newVertexId, edge.fromLabel);
            newVertexId++;
        }
        if (edge.toLabel != 4294967295){
            subgraph.AddVertex(newVertexId, edge.toLabel);
            newVertexId++;
        }
        subgraph.AddEdge(edge.fromId, edge.toId, edge.edgeLabel);
    //}
    return subgraph;
}




int main(int argc, char *argv[]){
    CLI::App app{"App description"};

    std::string initial_path = "";
    int threshold = 0;
    uint max_num_results =  100000, time_limit = UINT_MAX, initial_time_limit = UINT_MAX;
    bool print_prep = true, print_enum = true, homo = true, report_initial = true;
    std::vector<std::vector<uint>> orders;

    app.add_option("-d,--data", initial_path, "initial data graph path")->required();
    app.add_option("-t,--threshold", threshold, "initial threshold")->required();
    CLI11_PARSE(app, argc, argv);
    std::chrono::high_resolution_clock::time_point start, lstart;
    // std::cout << threshold <<std::endl;
    start = Get_Time();
    std::cout << "----------- Loading graphs ------------" << std::endl;
    Graph data_graph {};
    data_graph.LoadFromFile(initial_path);
    data_graph.PrintMetaData();
    //ProcessGraph(data_graph,threshold);
    Graph processedGraph = ProcessGraph(data_graph, threshold);
    processedGraph.PrintMetaData();
    Print_Time("Load Graphs: ", start);
    size_t convertedThreshold = static_cast<size_t>(threshold);
    // Graph newGraph = ProcessGraph(processedGraph,threshold);
    // newGraph.PrintMetaData();
    std::cout << "------------ Preprocessing ------------" << std::endl;
    // std::vector<Graph> subgraphs = processedGraph.ExpandByEdges();
    // Graph subg = subgraphs[1];
    // subg.Print();
    // std::vector<DFSEdge> encoding = processedGraph.gSpanDFSEncoding();
    //runGraphflowOnGraph(processedGraph, max_num_results, print_prep, print_enum, homo, threshold);
    //GraphData graphData = InitializeGraphData(processedGraph);
    // std::cout << "gSpan DFS Encoding:" << std::endl;
    // for (const DFSEdge& edge : encoding) {
    //     std::cout << "From Vertex Label: " << edge.fromLabel 
    //             << ", Edge Label: " << edge.edgeLabel 
    //             << ", To Vertex Label: " << edge.toLabel 
    //             << ", From Vertex ID: " << edge.fromId 
    //             << ", To Vertex ID: " << edge.toId << std::endl;
    // }
    // std::vector<DFSEdge> frequentEdges = processedGraph.getMinimumDFSEncoding();
    // // std::vector<DFSEdge> minDFSEncoding = data_graph.getMinimumDFSEncoding();
    // for (const DFSEdge& edge : frequentEdges) {
    // // std::vector<DFSEdge> initialDFSEncoding = {edge};
    // //Graph mySubgraph = buildSubgraphFromDFSEncoding(data_graph, edge);

    // Graph mySubgraph = {};
    // uint maxOriginalVertexId = mySubgraph.GetMaxVertexId();
    // uint newVertexId = maxOriginalVertexId;
    // //std::cout<< newVertexId<<std::endl;
    // std::cout << "From Vertex (ID: " << edge.fromId << ", Label: " << edge.fromLabel << ") "
    //           << "to Vertex (ID: " << edge.toId << ", Label: " << edge.toLabel << ") "
    //           << "with Edge Label: " << edge.edgeLabel << std::endl;
    // //mySubgraph.PrintMetaData();
    // if(edge.fromLabel != uint(-1)){
    //     mySubgraph.AddVertex(newVertexId,edge.fromLabel);
    //     newVertexId++;
    //     mySubgraph.AddVertex(newVertexId,edge.toLabel);
    //     //std::cout<< newVertexId-1<< "  "<<newVertexId<<std::endl;
    //     mySubgraph.AddEdge(newVertexId-1,newVertexId,edge.edgeLabel);
    //     }
    //     mySubgraph.PrintMetaData();
    //     mySubgraph.Print();
    //     mySubgraph.RemoveVerticesWithInvalidLabel();
    //     matching *mm = nullptr;
    //     Graphflow *graphflow = nullptr;
    //     mm = graphflow = new Graphflow(mySubgraph,processedGraph, max_num_results, print_prep, print_enum, homo); 
    //     mm->Preprocessing();
    //     auto InitialFun = [&mm]()
    //     {
    //         mm->InitialMatching();
    //     };
    //     size_t num_results = 0ul;
    //     mm->GetNumInitialResults(num_results);
    //     std::cout << num_results << " matches. \n";
    // }
    
    
    // for (const DFSEdge& edge : minDFSEncoding) {
    //     std::vector<DFSEdge> initialDFSEncoding = {edge};
    //     extendSubgraph(data_graph, initialDFSEncoding);
    // }

    auto astart = std::chrono::high_resolution_clock::now();
    std::cout << "----------- Initial Matching ----------" << std::endl; 
    std::vector<DFSEdge> frequentEdges = processedGraph.getMinimumDFSEncoding();
    // const size_t threshold = /* 您的阈值 */;

    Graph mySubgraph = {};
    uint maxOriginalVertexId = mySubgraph.GetMaxVertexId();
    uint newVertexId = maxOriginalVertexId;
    int k = 0;
    for (const DFSEdge& edge : frequentEdges) {
        std::cout << "From Vertex (ID: " << edge.fromId << ", Label: " << edge.fromLabel << ") "
                << "to Vertex (ID: " << edge.toId << ", Label: " << edge.toLabel << ") "
                << "with Edge Label: " << edge.edgeLabel << std::endl;

        if (edge.fromLabel != uint(-1)) {
            mySubgraph.AddVertex(newVertexId, edge.fromLabel);
            newVertexId++;
            mySubgraph.AddVertex(newVertexId, edge.toLabel);
            mySubgraph.AddEdge(newVertexId - 1, newVertexId, edge.edgeLabel);
        }
        std::cout << "----------- Informating  -----------------" << std::endl; 
        matching *mm = nullptr;
        Graphflow *graphflow = nullptr;
        mm = graphflow = new Graphflow(mySubgraph, data_graph, max_num_results, print_prep, print_enum=true, homo); 
        mm->Preprocessing();
        auto InitialFun = [&mm]()
        {
            mm->InitialMatching();
        };
        execute_with_time_limit(InitialFun, initial_time_limit, reach_time_limit);
        Print_Time("Initial Matching: ", start);
        size_t num_results = 0ul;
        mm->GetNumInitialResults(num_results);
        std::cout << num_results << " matches. \n";
        
        if (num_results < convertedThreshold) {
            std::cout << num_results << " matches exceed the threshold. Exiting loop. \n";
            delete graphflow;  // 如果您在之后不再需要graphflow和mm，确保删除它们以避免内存泄漏
            //delete mm;
            continue;  // 跳出frequentEdges的循环
        }
        mySubgraph.PrintMetaData();
        
        std::ofstream fout;
        fout.open("output.txt",std::ios::app);
        fout << "---------------------------------------------" << " "; 
        fout << std::endl;
        fout.close();
        std::cout << num_results << " matches. \n";
        k = k + 1;
        delete graphflow;  // 如果您在之后不再需要graphflow和mm，确保删除它们以避免内存泄漏
        //delete mm;
    }
    auto aend = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = aend - astart;
    std::ofstream fout;
    fout.open("output.txt",std::ios::app);
    fout << "---------------------------------------------" << " "; 
    fout << std::endl;
    fout << "total subgraph number is  "<<k<<" ";
    fout << std::endl;
    fout << "Algorithm execution time: " << duration.count() << " seconds" << " "; 
    fout << std::endl;
    fout.close();
}
