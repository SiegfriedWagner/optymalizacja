//
// Created by Mateusz Chojnowski on 02.03.2021.
// mail: 286333@stud.umk.pl
//

#include "Digraph.h"
#include <algorithm>
#include <stack>
#include <exception>
#include <vector>

using namespace digraph;

Digraph::Digraph(int vertices_num, std::vector<std::tuple<int, int, int>> &edges) : verticesNum_(vertices_num),
                                                                                    edgesNum_(std::size(edges)),
                                                                                    adjList_(vertices_num),
                                                                                    toEdgesNum_(vertices_num) {
    for (auto &tuple : edges) {
        int from = std::get<0>(tuple);
        int to = std::get<1>(tuple);
        int weight = std::get<2>(tuple);
        adjList_[from].emplace_back(to, weight);
        toEdgesNum_[to]++;
    }
}

void Digraph::AddEdge(int from, int to, int weight) {
    if (from < 0 || from >= verticesNum_)
        throw std::invalid_argument("Invalid 'from' vertex");
    if (to < 0 || to >= verticesNum_)
        throw std::invalid_argument("Invalid 'to' vertex");
    adjList_[from].emplace_back(to, weight);
    toEdgesNum_[to]++;
    edgesNum_++;
}

int Digraph::EdgesNum() const {
    return edgesNum_;
}

const std::vector<std::vector<Edge>> &Digraph::AdjList() const{
    return adjList_;
}

const std::vector<int> &Digraph::InEdgesNum() const {
    return toEdgesNum_;
}

int Digraph::VerticesNum() const {
    return verticesNum_;
}