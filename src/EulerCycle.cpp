//
// Created by mateu on 07.03.2021.
//

#include <unordered_set>
#include "EulerCycle.h"
#include "Graph.h"
#include "Digraph.h"

std::variant<bool, std::forward_list<int>> euler_cycle::EulerianCycle(const graph::Graph &&graph) {
    return euler_cycle::EulerianCycle(graph);
}
std::variant<bool, std::forward_list<int>> euler_cycle::EulerianCycle(const graph::Graph &graph) {
    using graph::Edge;
    // at leas one edge in graph
    if (graph.EdgesNum() < 1)
        return false;
    // check if any vertex has odd number of vertices
    for (int i = 0; i < graph.VerticesNum(); ++i) {
        if (std::size(graph.AdjList()[i]) % 2 != 0)
            return false;
    }
    // create iterators through edges
    std::vector<std::vector<std::shared_ptr<Edge>>::const_iterator> adjListIterator;
    adjListIterator.resize(graph.VerticesNum());
    for (int i = 0; i < graph.VerticesNum(); i++) {
        adjListIterator[i] = graph.AdjList()[i].begin();
    }
    std::unordered_set<Edge> visited_edges;
    std::stack<int> nodes_to_visit;
    std::forward_list<int> cycle;
    int cycle_size = 0;
    // find non isolated vertex
    for (int i = 0; i < graph.VerticesNum(); ++i) {
        if (std::size(graph.AdjList()[i]) > 0) {
            nodes_to_visit.push(i);
            break;
        }
    }
    // dfs through graph
    while (!nodes_to_visit.empty()) {
        int current = nodes_to_visit.top();
        nodes_to_visit.pop();
        while (adjListIterator[current] != graph.AdjList()[current].end()) {
            Edge edge = adjListIterator[current]->operator*();
            adjListIterator[current]++;
            if (visited_edges.contains(edge)) continue;
            else visited_edges.insert(edge);
            nodes_to_visit.push(current);
            current = edge.Other(current);
        }
        cycle.push_front(current);
        cycle_size++;
    }
    if (cycle_size == graph.EdgesNum() + 1)
        return cycle;
    return false;
}
std::variant<bool, std::forward_list<int>> euler_cycle::EulerianCycle(const digraph::Digraph &&graph) {
    return euler_cycle::EulerianCycle(graph);
}
std::variant<bool, std::forward_list<int>> euler_cycle::EulerianCycle(const digraph::Digraph &graph) {
    using digraph::Edge;
    // check if there are at least 1 edge
    if (graph.EdgesNum() < 1) return false;
    // check if every vertex have equal number of incoming and outgoing connections
    for (int i = 0; i < graph.VerticesNum(); ++i)
        if (std::size(graph.AdjList()[i]) != graph.InEdgesNum()[i])
            return false;
    std::vector<std::vector<Edge>::const_iterator> adjListIterator;
    adjListIterator.resize(graph.VerticesNum());
    // create iterators through edges
    for (int i = 0; i < graph.VerticesNum(); ++i)
        adjListIterator[i] = graph.AdjList()[i].begin();
    std::stack<int> nodes_to_visit;
    std::forward_list<int> cycle;
    int cycle_size = 0;
    // pick starting vertex
    for (int i = 0; i < graph.VerticesNum(); ++i) {
        if (std::size(graph.AdjList()) > 0) {
            nodes_to_visit.push(i);
            break;
        }
    }

    while (!nodes_to_visit.empty()) {
        int current = nodes_to_visit.top();
        nodes_to_visit.pop();
        while (adjListIterator[current] != graph.AdjList()[current].end()) {
            nodes_to_visit.push(current);
            int newVertex = adjListIterator[current]->to;
            adjListIterator[current]++;
            current = newVertex;
        }
        cycle.push_front(current);
        cycle_size++;
    }
    if (cycle_size != graph.EdgesNum() + 1)
        return false;
    return cycle;
}

