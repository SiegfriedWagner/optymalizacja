//
// Created by mateu on 15.03.2021.
//

#include <variant>
#include <unordered_set>
#include <queue>
#include "Digraph.h"
#include "Matching.h"
#include "Bipariate.h"
#include "std_extensions.h"
#include <cassert>


std::pair<int, int> make_ordered_pair(int a, int b) {
    if (a > b)
        return {b, a};
    return {a, b};
}


/// based on: http://www.csl.mtu.edu/cs4321/www/Lectures/Lecture%2022%20-%20Maximum%20Matching%20in%20Bipartite%20Graph.htm
std::variant<bool, matching::match> matching::MaximumMatching(digraph::Digraph &graph) {
    const int UNLABELED = -1;
    auto bipariate_result = bipariate::BipariateGraph(graph);
    if (std::holds_alternative<bool>(bipariate_result))
        return false; // graph is not bipartite, general approach was not implemented
    auto &bipariateInfo = std::get<bipariate::BipariateInfo>(bipariate_result);
    std::queue<int> freeLeftVertices;
    std::vector<bool> vertexInMatching(graph.VerticesNum(), false);
    std::vector<int> vertexLabels(graph.VerticesNum(), UNLABELED);
    matching::match matching_result;
    // add all freeLeftVertices to queue
    for (int vertex_index = 0; vertex_index < bipariateInfo.vertexColors.size(); ++vertex_index) {
        if (bipariateInfo.vertexColors[vertex_index] == bipariate::VertexColor::LEFT) {
            freeLeftVertices.push(vertex_index);
        }
    }
    while (!freeLeftVertices.empty()) {
        int dequeued_vertex = freeLeftVertices.front();
        freeLeftVertices.pop();
        if (bipariateInfo.vertexColors[dequeued_vertex] == bipariate::VertexColor::LEFT) { // left vertex case
            for (auto &edge : graph.AdjList()[dequeued_vertex]) {
                int considered_right_vertex = edge.to;
                if (!vertexInMatching[considered_right_vertex]) {
                    // add vertex to matching
                    matching_result.insert(make_ordered_pair(dequeued_vertex, considered_right_vertex));
                    vertexInMatching[considered_right_vertex] = true;
                    vertexInMatching[dequeued_vertex] = true;
                    // follow path
                    int altering_left_vertex = dequeued_vertex;
                    while (vertexLabels[altering_left_vertex] != UNLABELED) {
                        // alternate path  -> swap edges 'color' along path,
                        considered_right_vertex = vertexLabels[altering_left_vertex];
                        matching_result.erase(make_ordered_pair(altering_left_vertex, considered_right_vertex));
                        altering_left_vertex = vertexLabels[considered_right_vertex];
                        matching_result.insert(make_ordered_pair(altering_left_vertex, considered_right_vertex));
                    }
                    // prepare for next iteration - create queue of unvisited left vertexes and clear labels
                    for (int i = 0; i < vertexLabels.size(); ++i)
                        vertexLabels[i] = UNLABELED;
                    std::queue<int>().swap(freeLeftVertices); // fast way to clear queue
                    for (int vertex_index = 0; vertex_index < graph.VerticesNum(); ++vertex_index)
                        // add all free left vertices to queue
                        if (bipariateInfo.vertexColors[vertex_index] == bipariate::VertexColor::LEFT &&
                            !vertexInMatching[vertex_index])
                            freeLeftVertices.push(vertex_index);
                    break;
                }
                else {
                    if (!matching_result.contains(make_ordered_pair(dequeued_vertex, considered_right_vertex)) && vertexLabels[considered_right_vertex] == UNLABELED) {
                        // label right vertex with potential predecessor and push it to visit queue
                        vertexLabels[considered_right_vertex] = dequeued_vertex;
                        freeLeftVertices.push(considered_right_vertex);
                    }
                }
            }
        } else { // right vertex case
            int left_vertex = -1;
            // find matching left vertex to label it with current right vertex
            for (auto &pair : matching_result)
                if (pair.first == dequeued_vertex) {
                    left_vertex = pair.second;
                    break;
                } else if (pair.second == dequeued_vertex) {
                    left_vertex = pair.first;
                    break;
                }
            assert(left_vertex != -1);
            // label left vertex with potential predecessor and push it to visit queue
            vertexLabels[left_vertex] = dequeued_vertex;
            freeLeftVertices.push(left_vertex); // enqueue left vertex for revisit
        }
    }
    return matching_result;
}
