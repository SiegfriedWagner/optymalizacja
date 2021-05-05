//
// Created by Mateusz Chojnowski on 07.03.2021.
//
#include <iostream>
#include <unordered_set>
#include <fstream>
#include "../src/Digraph.h"
#include "../src/Matching.h"
#include "../external/argparse/argparse.h"
#include "../src/IOHelpers.h"

void PrintMatching(std::unordered_set<std::pair<int, int>> &matching) {
    for(auto &edge : matching)
    {
        std::cout << "(" << edge.first + 1<< ", " << edge.second + 1<< ")" << std::endl;
    }
}
int main(const int argc, const char **argv) {

    argparse::ArgumentParser parser("maximum matching");
    parser.add_description("Program that finds maximum matching for directed graph");
    parser.add_argument("input_file")
            .help("Input file to parse")
            .action([](const std::string &value) {
                std::fstream inputFile;
                inputFile.open(value.c_str(), std::fstream::in);
                auto result = io_helpers::ParseFile<int, int>(inputFile);
                digraph::Digraph graph(result.verticesNum, result.edges);
                auto matrching_result = matching::MaximumMatching(graph);
                if(!std::holds_alternative<bool>(matrching_result)) {
                    std::cout << "Found matching" << std::endl;
                    PrintMatching(std::get<std::unordered_set<std::pair<int, int >>>(matrching_result));
                }
                else
                    std::cout << "No matching found" << std::endl;
                inputFile.close();
            });
    parser.parse_args(argc, argv);
    return 0;
}


