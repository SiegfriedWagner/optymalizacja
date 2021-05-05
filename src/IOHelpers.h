//
// Created by mateu on 07.03.2021.
//

#ifndef OPT_IOHELPERS_H
#define OPT_IOHELPERS_H
#include <forward_list>

namespace io_helpers {
    template<typename VertexType, typename WeightType>
    struct ParsingResult {
        const bool isDirected;
        const int verticesNum;
        const std::vector<std::tuple<VertexType, VertexType, WeightType>> edges;
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
