//
// Created by mateu on 29.05.2021.
//
#include "../external/argparse/argparse.h"
#include "../src/GraphColoring.h"
#include "../src/IOHelpers.h"

int main(const int argc, const char **argv) {

    argparse::ArgumentParser parser("maximum matching");
    parser.add_description("Program that finds maximum matching for directed graph");
    parser.add_argument("input_file")
            .help("Input file to parse")
            .action([](const std::string &value) {
                std::fstream inputFile;
                inputFile.open(value.c_str(), std::fstream::in);
                auto result = io_helpers::ParseFile<int, int>(inputFile);
                if (result.isDirected)
                    throw std::runtime_error("Directed graphs are not supported");
                graph::Graph toColor(result.verticesNum, result.edges);
                auto coloring = ColorGraph(toColor);
                std::cout << "Colors used " << coloring.numberOfColorsUsed << std::endl;
                for (int i = 0; i < coloring.colors.size(); ++i)
                    std::cout << "V(" << i + 1 << "): " << coloring.colors[i] << std::endl;
                inputFile.close();
            });
    parser.parse_args(argc, argv);
    return 0;
}
