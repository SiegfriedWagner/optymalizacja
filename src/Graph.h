//
// Created by mateu on 06.03.2021.
//

#ifndef OPT_GRAPH_H
#define OPT_GRAPH_H
#include <vector>
#include <tuple>
#include <memory>
#include <algorithm>
#include <variant>
#include <forward_list>

namespace graph {
    struct Edge {
        // v <= w (always)
        const int v;
        const int w;
        int weight; // TODO: Consider making weight const for sake of hash<Edge> correctness
        Edge(int f_vertex, int s_vertex, int weight) : v(f_vertex), w(s_vertex), weight(weight) {
           if (v > w)
               std::swap((int&) v,(int&) w);
        }
        bool operator==(const Edge &other) const;
        [[nodiscard]] int Other(int vertex) const;
    };
    class Graph {
    private:
        int verticesNum_;
        int edgesNum_;
        std::vector<std::vector<std::shared_ptr<Edge>>> adjList_;
    public:
        Graph();
        Graph(int verticesNum);
        Graph(int verticesNum, std::vector<std::tuple<int, int, int>> &initializer);
        Graph(Graph&& moved);
        void AddEdge(int f_vertex, int s_vertex, int weight);
        [[nodiscard]] int EdgesNum() const;

        [[nodiscard]] int VerticesNum() const;

        std::vector<std::vector<std::shared_ptr<Edge>>> &AdjList();
    };
}
namespace std {
    // std extension with hash function for graph edge
    using graph::Edge;
    using std::size_t;
    using std::hash;

    template<>
    struct hash<Edge> {
        size_t operator()(const Edge &edge) const {
            static_assert(std::is_same<decltype(Edge::w), const int>::value,
                          "Edge hash function was created only for int types");
            static_assert(std::is_same<decltype(Edge::v), const int>::value,
                          "Edge hash function was created only for int types");
            static_assert(std::is_same<decltype(Edge::weight), int>::value,
                          "Edge hash function was created only for int types");
            // implementation based on https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
            // TODO: Test hash function
            return ((edge.v ^ (edge.w << 1)) >> 1) ^ (edge.weight << 1);
        }
    };
}
#endif //OPT_GRAPH_H
