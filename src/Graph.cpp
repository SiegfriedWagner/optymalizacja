//
// Created by Mateusz Chojnowski on 06.03.2021.
//

#include "Graph.h"

using namespace graph;

bool Edge::operator==(const Edge &other) const {
    return from == other.from && to == other.to && weight == other.weight;
}

int Edge::Other(int vertex) const {
    if (vertex == from)
        return to;
    else if (vertex == to)
        return from;
    else
        throw std::invalid_argument("Vertex id must be either to or v");
}

Graph::Graph() : Graph(0) {}

Graph::Graph(int verticesNum) : verticesNum_(verticesNum), edgesNum_(0), adjList_(verticesNum,
                                                                                  std::vector<std::shared_ptr<Edge>>()) {}

Graph::Graph(Graph &&moved) : verticesNum_(moved.verticesNum_), edgesNum_(moved.edgesNum_), adjList_(std::move(moved.adjList_)) {}

Graph::Graph(int verticesNum, const std::vector<std::tuple<int, int, int>> &initializer) : Graph(verticesNum) {
    for (auto &element : initializer) {
        AddEdge(std::get<0>(element), std::get<1>(element), std::get<2>(element));
    }
}



void Graph::AddEdge(int f_vertex, int s_vertex, int weight) {
    if (f_vertex < 0 || f_vertex >= verticesNum_)
        throw std::out_of_range("f_vertex has invalid value");
    if (s_vertex < 0 || s_vertex >= verticesNum_)
        throw std::out_of_range("s_vertex has invalid value");
    // edges is stored between vertices (adjList_). Lower vertex index is always first in internal edges representation.
    if (f_vertex > s_vertex)
        std::swap(f_vertex, s_vertex);
    // check if edge exists
    for (auto &edge : adjList_[f_vertex])
        if (edge->to == s_vertex)
            throw std::invalid_argument("Attempt to add edge for second time was made");
    auto edge = std::make_shared<Edge>(f_vertex, s_vertex, weight);
    adjList_[f_vertex].push_back(edge);
    adjList_[s_vertex].push_back(edge);
    edgesNum_++;
}

int Graph::EdgesNum() const {
    return edgesNum_;
}

int Graph::VerticesNum() const {
    return verticesNum_;
}

const std::vector<std::vector<std::shared_ptr<Edge>>> &Graph::AdjList() const {
    return adjList_;
}

