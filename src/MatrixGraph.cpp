//
// Created by mateu on 03.05.2021.
//

#include "MatrixGraph.h"
#include <cassert>

// graph::MatrixGraph::noEdgeValue = -1;
graph::MatrixGraph::MatrixGraph(const int verticesNum) : verticesNum_(
        verticesNum), matrix_(std::vector<std::vector<int>>(verticesNum,std::vector<int>(verticesNum, -1))) {

}

graph::MatrixGraph::MatrixGraph(const int verticesNum, const std::vector<std::tuple<int, int, int>> initializer)
        : MatrixGraph(verticesNum) {
    for (int i = 0; i < std::size(initializer); ++i)
        matrix_[std::get<0>(initializer[i])][std::get<1>(initializer[i])] = std::get<2>(initializer[i]);
}

graph::MatrixGraph::MatrixGraph(const bool directed, const int verticesNum,
                                const std::vector<std::tuple<int, int, int>> initializer) : MatrixGraph(verticesNum,
                                                                                                         initializer) {
    if (!directed)
        for (int i = 0; i < std::size(initializer); ++i)
            matrix_[std::get<1>(initializer[i])][std::get<0>(initializer[i])] = std::get<2>(initializer[i]);
}

std::vector<std::vector<int>>& graph::MatrixGraph::Matrix() {
    return matrix_;
}

const std::vector<std::vector<int>>& graph::MatrixGraph::Matrix() const {
    return matrix_;
}

void graph::MatrixGraph::AddEdge(int from, int to, int weight) {
    assert(from < verticesNum_);
    assert(to < verticesNum_);
    matrix_[from][to] = weight;
}

size_t graph::MatrixGraph::VerticesNum() const {
    return verticesNum_;
}

int graph::MatrixGraph::Weight(int from, int to) const {
    return matrix_[from][to];
}