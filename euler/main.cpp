#include <iostream>
#include <fstream>
#include <filesystem>
#include <tuple>
#include <forward_list>

#include "../src/Digraph.h"
#include "../src/Graph.h"
#include "../src/EulerCycle.h"
#include "../src/IOHelpers.h"
#include "../external/argparse/argparse.h"

int main(int argc, const char* argv[]) {
    argparse::ArgumentParser parser("euler");
    parser.add_description("Program that finds euler cycle for directed or undirected graphs");
    parser.add_argument("input_file")
    .help("Input file to parse")
    .action([](const std::string &value) {
        std::fstream inputFile;
        inputFile.open(value.c_str(), std::fstream::in);
        auto result = io_helpers::ParseFile<int, int>(inputFile);
        if (std::get<0>(result)) {
            digraph::Digraph graph(std::get<1>(result), std::get<2>(result));
            auto cycle_result = euler_cycle::EulerianCycle(graph);
            if (std::holds_alternative<std::forward_list<int>>(cycle_result))
                euler_cycle::PrintCycle(std::get<std::forward_list<int>>(cycle_result));
            else
                std::cout << "No cycle" << std::endl;
        }
        else
        {
            graph::Graph undirected_graph(std::get<1>(result), std::get<2>(result));
            auto cycle_result = euler_cycle::EulerianCycle(undirected_graph);
            if (std::holds_alternative<std::forward_list<int>>(cycle_result))
                euler_cycle::PrintCycle(std::get<std::forward_list<int>>(cycle_result));
            else
                std::cout << "No cycle" << std::endl;
        }
        inputFile.close();
    });
    parser.parse_args(argc, argv);
    return 0;
}
