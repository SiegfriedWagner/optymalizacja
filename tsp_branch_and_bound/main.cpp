//
// Created by Mateusz Chojnowski on 03.05.2021.
//

#include <fstream>
#include <climits>
#include "../external/argparse/argparse.h"
#include "../src/IOHelpers.h"
#include "../src/MatrixGraph.h"
#include "../src/TSP.h"

int main(int argc, char *argv[])
{
    std::srand(std::time(NULL));
    argparse::ArgumentParser parser("Traveling Salesman Problem - Branch and Bound");
    parser.add_description("Program that solves TSP using Branch and Bound algorithm");
    parser.add_argument("input_file")
            .help("Input file to parse")
            .action([](const std::string &value) {
                std::fstream  inputFile;
                inputFile.open(value.c_str(), std::fstream::in);
                auto result = io_helpers::ParseFile<int, int>(inputFile);
                graph::MatrixGraph graph(result.isDirected, result.verticesNum, result.edges);
                auto path = tsp::BranchAndBoundSolve(graph, false);
                io_helpers::PrintGraph(path);
                std::cout << "Total cost: " << path.AllEdgesCost() << std::endl;
                inputFile.close();
            });
    parser.parse_args(argc, argv);
    return 0;
}