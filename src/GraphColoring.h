//
// Created by mateu on 29.05.2021.
//

#ifndef OPTYMALIZACJA_GRAPHCOLORING_H
#define OPTYMALIZACJA_GRAPHCOLORING_H
#include <vector>
#include "Graph.h"

struct GraphColoringResult {
    std::vector<size_t> colors;
    size_t numberOfColorsUsed;
};

GraphColoringResult ColorGraph(graph::Graph graph);
#endif //OPTYMALIZACJA_GRAPHCOLORING_H
