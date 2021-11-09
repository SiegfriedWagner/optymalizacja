//
// Created by Mateusz Chojnowski on 28.03.2021.
//
#include <cassert>
#include <forward_list>
#include <climits>
#include "Hungarian.h"
#include "Bipartiate.h"
#include "Matching.h"

std::variant<bool, matching::match> matching::HungarianMatch(digraph::Digraph &graph) {
    auto bipartiate_result = bipariate::BipariateGraph(graph);
    if (std::holds_alternative<bool>(bipartiate_result))
        // HungarianMatch for graphs that are not bipartite is no implemented
        return false;
    auto &bipartiteInfo = std::get<bipariate::BipariateInfo>(bipartiate_result);
    auto in_matching = std::vector<bool>(graph.VerticesNum(), false);
    auto labels = std::vector<int>(graph.VerticesNum(), 0);
    auto equality_graph = digraph::Digraph(graph.VerticesNum());
    std::unordered_set<std::tuple<int, int>> edges_in_equality_graph;
    // initialize labels and create equality graph
    bipariate::VertexColor fromColor = bipariate::VertexColor::UNCOLORED;
    for (int i = 0; i < graph.VerticesNum(); ++i) {
        if (std::size(graph.AdjList()[i]) != 0)
        {
            fromColor = bipartiteInfo.vertexColors[i];
            break;
        }
    }
    assert(fromColor != bipariate::VertexColor::UNCOLORED);
    for (int i = 0; i < graph.VerticesNum(); ++i) {
        if (bipartiteInfo.vertexColors[i] == fromColor) {
            int maxEdgeWeight = -1;
            int maxEdgeToVertex = -1;
            for (int edgeIndex = 0; edgeIndex < std::size(graph.AdjList()[i]); ++edgeIndex) {
                auto edge = graph.AdjList()[i][edgeIndex];
                if (maxEdgeWeight < edge.weight) {
                    maxEdgeWeight = edge.weight;
                    maxEdgeToVertex = edge.to;
                }
            }
            labels[i] = maxEdgeWeight;
            equality_graph.AddEdge(i, maxEdgeToVertex, maxEdgeWeight);
            edges_in_equality_graph.emplace(std::make_tuple(i, maxEdgeToVertex));
        }
    }
    const int maxMatchingSize = std::min(bipartiteInfo.leftVertexes, bipartiteInfo.rightVertexes);
    // compute initial maximum matching
    auto match_result = matching::MaximumMatching(equality_graph); // TODO: Unwrap and optimize
    assert(std::holds_alternative<matching::match>(match_result));
    auto &match = std::get<matching::match>(match_result);
    while(match.size() < maxMatchingSize && equality_graph.EdgesNum() < graph.EdgesNum()) {
        // update matching information based on current maximum matching
        for (auto &match_element : match) {
            in_matching[std::get<0>(match_element)] = true;
            in_matching[std::get<1>(match_element)] = true;
        }
        // find new delta and collect edges from graph that will be added to equality graph
        int delta = INT_MAX;
        auto edges_to_add = std::forward_list<std::tuple<int, digraph::Edge>>();
        for (int vertexIndex = 0; vertexIndex < graph.VerticesNum(); ++vertexIndex) {
            if (bipartiteInfo.vertexColors[vertexIndex] == fromColor) {
                for (auto &edge : graph.AdjList()[vertexIndex]) {
                    if (!edges_in_equality_graph.contains(std::make_tuple(vertexIndex, edge.to))) {
                    //if (!in_matching[edge.to] && !edges_in_equality_graph.contains(std::make_tuple(vertexIndex, edge.to))) {
                        // found candidate
                        int delta_candidate = labels[vertexIndex] + labels[edge.to] - edge.weight;
                        if (delta > delta_candidate) {
                            edges_to_add.clear();
                            delta = delta_candidate;
                            edges_to_add.push_front(std::make_tuple(vertexIndex, edge));
                        } else if (delta == delta_candidate) {
                            edges_to_add.push_front(std::make_tuple(vertexIndex, edge));
                        }
                    }
                }
            }
        }
        // update labels using delta
        for (int vertexIndex = 0; vertexIndex < graph.VerticesNum(); ++vertexIndex) {
            if (in_matching[vertexIndex]) {
                if (bipartiteInfo.vertexColors[vertexIndex] == fromColor)
                    labels[vertexIndex] -= delta;
                else {
                    assert(bipartiteInfo.vertexColors[vertexIndex] != fromColor);
                    labels[vertexIndex] += delta;
                }
            }
        }
        // add edges to equality graph that have minimum delta
        for (auto &edge : edges_to_add) {
            auto &from = std::get<0>(edge);
            auto &e = std::get<1>(edge);
            auto tuple = std::make_tuple(from, e.to);
            if(!edges_in_equality_graph.contains(tuple)) {
                equality_graph.AddEdge(std::get<0>(edge), e.to, e.weight);
                edges_in_equality_graph.emplace(tuple);
            }
        }
        // find new match based on new equality graph
        match_result = matching::MaximumMatching(equality_graph); // TODO: Unwrap and optimize
        assert(std::holds_alternative<matching::match>(match_result));
        auto &new_match = std::get<matching::match>(match_result);
        // if new matching is the same size as previous (no augmenting path) break
        if(new_match.size() > match.size())
            match = new_match;
    }
    return match;
}