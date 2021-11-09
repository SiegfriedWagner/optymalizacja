//
// Created by Mateusz Chojnowski on 10.05.2021.
//
#include <unordered_set>
#include <fstream>
#include "../src/Graph.h"
#include "../src/IOHelpers.h"
#include "../external/argparse/argparse.h"
#include "../src/std_extensions.h"
#include <stack>
#include <climits>


graph::Graph<> MinimumSpanningTree(graph::Graph<> &G) {
    graph::Graph mst_result(G.VerticesNum());
    std::unordered_set<int> visited_vertices;
    std::shared_ptr<graph::Edge> invalidEdge = std::make_shared<graph::Edge>(-1, -1, INT_MAX);
    std::shared_ptr<graph::Edge> minimalEdge = invalidEdge;
    if (G.EdgesNum() == 0)
        return mst_result;
    // pick initial edge
    for (int vertex = 0; vertex < G.VerticesNum(); ++vertex) {
        // iterate edges
        for(auto &edge : G.AdjList()[vertex]) {
            if (edge->weight < minimalEdge->weight)
                minimalEdge = edge;
        }
    }
    mst_result.AddEdge(minimalEdge);
    visited_vertices.emplace(minimalEdge->from);
    visited_vertices.emplace(minimalEdge->to);
    do {
        minimalEdge = invalidEdge;
        // find edge with lowest cost
        for(auto vertex : visited_vertices) {
            // iterate edges
            for(auto &edge : G.AdjList()[vertex]) {
                if (edge->weight < minimalEdge->weight && !visited_vertices.contains(edge->Other(vertex)))
                    minimalEdge = edge;
            }
        }
        if (minimalEdge != invalidEdge) {
            visited_vertices.emplace(minimalEdge->from);
            visited_vertices.emplace(minimalEdge->to);
            mst_result.AddEdge(minimalEdge);
        }
    } while(minimalEdge != invalidEdge);
    return mst_result;
}

void DFS_with_replacement(graph::Graph<> &graph, std::forward_list<int> &order_of_visited_vertices) {
    std::unordered_set<int> visitedVertices;
    std::stack<int> toVisit;
    std::stack<int> wayBack;
    int first_vertex;
    // pick first vertex
    for (int vertex = 0; vertex < graph.VerticesNum(); ++vertex) {
        if (!graph.AdjList()[vertex].empty()) {
            visitedVertices.emplace(vertex);
            wayBack.push(vertex);
            std::cout << vertex + 1<< std::endl;
            first_vertex = vertex;
            order_of_visited_vertices.push_front(vertex);
            for(auto &edge : graph.AdjList()[vertex])
                toVisit.push(edge->Other(vertex));
            break;
        }
    }
    // dfs
    while(!toVisit.empty()) {
        int current = toVisit.top();
        visitedVertices.insert(current);
        std::cout << current + 1 << std::endl;
        order_of_visited_vertices.push_front(current);
        toVisit.pop();
        bool visitedAtLeastOnce = false;
        for(auto &edge : graph.AdjList()[current])
            if (!visitedVertices.contains(edge->Other(current))) {
                if (!visitedAtLeastOnce) {
                    wayBack.push(current);
                    visitedAtLeastOnce = true;
                }
                toVisit.push(edge->Other(current));
            }
        if (!visitedAtLeastOnce) {
            std::cout << "Back: " <<  wayBack.top()<< std::endl;
            wayBack.pop();
        }
    }
    order_of_visited_vertices.push_front(first_vertex); // complete cycle
}

void print_path_with_weights(graph::Graph<> &graph, std::forward_list<int> &vertiecs) {
    int sum_total = 0;
    auto it = vertiecs.begin();
    int from= 0;
    int to = *(it++);
    while(it != vertiecs.end()) {
        from = to;
        to = *(it++);
        for (auto &edge : graph.AdjList()[from]) {
            if (edge->Other(from) == to) {
                std::cout << edge->from + 1 << " " << edge->to + 1 << " " << edge->weight << std::endl;
                sum_total += edge->weight;
                break;
            }
        }
    }
    std::cout << "Total cost: " << sum_total << std::endl;
}

int main(int argc, char *argv[]) {
    argparse::ArgumentParser parser("Traveling Salesman Problem - Branch and Bound");
    parser.add_description("Program that solves TSP using Branch and Bound algorithm");
    parser.add_argument("input_file")
            .help("Input file to parse")
            .action([](const std::string &value) {
                std::fstream  inputFile;
                inputFile.open(value.c_str(), std::fstream::in);
                auto result = io_helpers::ParseFile<int, int>(inputFile);
                if (result.isDirected) {
                    std::cout << "Directed graphs are not supported" << std::endl;
                    return 1;
                }

                graph::Graph input_graph(result.verticesNum, result.edges);
                auto mst_graph = MinimumSpanningTree(input_graph);
                std::cout << "MST Graph" << std::endl;
                io_helpers::PrintGraph(mst_graph);
                std::forward_list<int> order_of_visited_vertices;
                DFS_with_replacement(mst_graph, order_of_visited_vertices);
                print_path_with_weights(input_graph, order_of_visited_vertices);
                inputFile.close();
                return 0;
            });
    parser.parse_args(argc, argv);
    return 0;
}
