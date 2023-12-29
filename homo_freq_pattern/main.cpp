#include <chrono>
#include <iostream>
#include <numeric>
#include <string>
#include <thread>
#include "CLI11.hpp"
#include "globals.h"
#include "types.h"
#include "graph.h"
#include "matching.h"
#include "graphflow.h"

int main(int argc, char *argv[]){
    CLI::App app{"App description"};

    std::string query_path = "", initial_path = "", stream_path = "../test", algorithm = "graphflow";
    uint max_num_results = UINT_MAX, time_limit = UINT_MAX, initial_time_limit = UINT_MAX;
    bool print_prep = true, print_enum = true, homo = true, report_initial = true;
    std::vector<std::vector<uint>> orders;
    app.add_option("-q,--query", query_path, "query graph path")->required();
    app.add_option("-d,--data", initial_path, "initial data graph path")->required();
    CLI11_PARSE(app, argc, argv);
    std::chrono::high_resolution_clock::time_point start, lstart;

    start = Get_Time();
    std::cout << "----------- Loading graphs ------------" << std::endl;
    Graph query_graph {};
    query_graph.LoadFromFile(query_path);
    query_graph.PrintMetaData();

    Graph data_graph {};
    data_graph.LoadFromFile(initial_path);
    data_graph.PrintMetaData();
    Print_Time("Load Graphs: ", start);

    std::cout << "------------ Preprocessing ------------" << std::endl;
    matching *mm = nullptr;
    Graphflow *graphflow = nullptr;
    start = Get_Time();
    mm = graphflow      = new Graphflow(query_graph, data_graph, max_num_results, print_prep, print_enum, homo); 
    mm->Preprocessing();
    Print_Time("Preprocessing: ", start);
    std::cout << "----------- Initial Matching ----------" << std::endl; 
    start = Get_Time();
    auto InitialFun = [&mm]()
    {
        mm->InitialMatching();
    };
    execute_with_time_limit(InitialFun, initial_time_limit, reach_time_limit);
    Print_Time("Initial Matching: ", start);
        
    size_t num_results = 0ul;
    mm->GetNumInitialResults(num_results);
    std::cout << num_results << " matches. \n";
}