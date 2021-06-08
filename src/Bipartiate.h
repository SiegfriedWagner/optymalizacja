//
// Created by Mateusz Chojnowski on 15.03.2021.
//
#ifndef OPTYMALIZACJA_BIPARTIATE_H
#define OPTYMALIZACJA_BIPARTIATE_H

#include "Graph.h"
#include "Digraph.h"
#include <vector>

namespace bipariate {
    enum VertexColor {
        UNCOLORED = 0, // free vertices without connection
        LEFT = 1,
        RIGHT = 2
    };
    struct BipariateInfo {
        int leftVertexes;
        int rightVertexes;
        std::vector<VertexColor> vertexColors;
    };
    std::variant<bool, BipariateInfo> BipariateGraph(digraph::Digraph &graph);
    std::variant<bool, BipariateInfo> BipariateGraph(graph::Graph<> &graph);
}
#endif //OPTYMALIZACJA_BIPARTIATE_H
