//
// Created by mateu on 28.03.2021.
//
#include <iostream>
#include <fstream>
#include <unordered_set>
#include "../src/Hungarian.h"
#include "../external/argparse/argparse.h"
#include "../src/IOHelpers.h"

void PrintWeightedMatching(std::unordered_set<std::pair<int, int>> &matching, digraph::Digraph &graph) {
    // inefficient
    int weight_sum = 0;
    for(auto &match : matching)
    {
        bool printed = false;
        for(auto &edge : graph.AdjList()[match.first]) {
            if (edge.to == match.second || edge.to == match.first) {
                std::cout << "(" << match.first + 1 << ", " << match.second + 1 << ", " << edge.weight << ")" << std::endl;
                weight_sum += edge.weight;
                printed = true;
                break;
            }
        }
        if(!printed)
        for(auto &edge : graph.AdjList()[match.second]) {
            if (edge.to == match.second || edge.to == match.first) {
                std::cout << "(" << match.first + 1 << ", " << match.second + 1 << ", " << edge.weight << ")" << std::endl;
                weight_sum += edge.weight;
                break;
            }
        }
    }
    std::cout << "Weight sum: " << weight_sum << std::endl;
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
                auto matrching_result = matching::HungarianMatch(graph);
                if(!std::holds_alternative<bool>(matrching_result)) {
                    std::cout << "Found matching" << std::endl;
                    PrintWeightedMatching(std::get<std::unordered_set<std::pair<int, int >>>(matrching_result), graph);
                }
                else
                    std::cout << "No matching found" << std::endl;
                inputFile.close();
            });
    parser.parse_args(argc, argv);
    return 0;
}