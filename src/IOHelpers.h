//
// Created by mateu on 07.03.2021.
//

#ifndef OPT_IOHELPERS_H
#define OPT_IOHELPERS_H
#include <forward_list>
#include <iostream>
#include <fstream>
#include "Graph.h"
#include "Digraph.h"

namespace io_helpers {
    template<typename VertexType, typename WeightType>
    struct ParsingResult {
        const bool isDirected;
        const int verticesNum;
        std::vector<std::tuple<VertexType, VertexType, WeightType>> edges;
    };

    template<typename VertexType, typename WeightType>
    ParsingResult<VertexType, WeightType> ParseFile(std::istream &inputFile) {
        bool valid = false;
        int vertexNum = 0;
        bool digraph = false;
        std::vector<std::tuple<VertexType, VertexType, WeightType>> vec;
        std::string line;
        while (inputFile.good()) {
            inputFile >> line;
            if (line == "#DIGRAPH") {
                valid = true;
                inputFile >> line;
                if (line == "true")
                    digraph = true;
                std::cout << "Digraph: " << digraph << std::endl;
            } else if (line == "#EDGES") {
                int first, second, weight;
                while (inputFile.good()) {
                    // TODO: Validation for invalid file structure
                    inputFile >> first;
                    inputFile >> second;
                    inputFile >> weight;
                    vec.push_back({first - 1, second - 1, weight});
                    vertexNum = std::max(std::max(vertexNum, first), second);
                    std::cout << first << " " << second << " " << weight << " " << std::endl;
                }
            }
        }
        if(!valid)
            throw new std::runtime_error("File doesn't contain valid graph description");
        return {digraph, vertexNum, std::move(vec)};
    }
    template<typename E=graph::Edge>
    void PrintGraph(graph::Graph<E> &graph){
        std::cout << "VerticesNum: " << graph.VerticesNum() << std::endl;
        for (int vertex = 0; vertex < graph.VerticesNum(); ++vertex) {
            for(auto &edge : graph.AdjList()[vertex])
                if (edge->from == vertex)
                    std::cout << edge->from + 1 << " " << edge->to + 1 << " " << edge->weight << std::endl;
        }
    }

    void PrintGraph(digraph::Digraph &graph) {
        std::cout << "VerticesNum: " << graph.VerticesNum() << std::endl;
        for (int vertex = 0; vertex < graph.VerticesNum(); ++vertex) {
            for(auto &edge : graph.AdjList()[vertex])
                std::cout << vertex + 1 << " " << edge.to + 1 << " " << edge.weight << std::endl;
        }
    }

}


namespace euler_cycle {
    void PrintCycle(std::forward_list<int> &cycle) {
        auto iterator = cycle.begin();
        std::cout << *iterator + 1;
        iterator++;
        while (iterator != cycle.end()) {
            std::cout << " -> " << *iterator + 1;
            iterator++;
        }
        std::cout << std::endl;
    }
}
#endif //OPT_IOHELPERS_H
