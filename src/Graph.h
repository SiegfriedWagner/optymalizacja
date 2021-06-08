//
// Created by mateu on 06.03.2021.
//

#ifndef OPT_GRAPH_H
#define OPT_GRAPH_H
#include <vector>
#include <unordered_set>
#include <tuple>
#include <memory>
#include <algorithm>
#include <variant>
#include <forward_list>
#include <cassert>

namespace graph {
    struct Edge {
        const int from;
        const int to;
        const int weight; // TODO: Consider making weight const for sake of hash<Edge> correctness
        Edge(int f_vertex, int s_vertex, int weight) : from(f_vertex), to(s_vertex), weight(weight) { }
        bool operator==(const Edge &other) const {
            return from == other.from && to == other.to && weight == other.weight;
        };
        [[nodiscard]] int Other(int vertex) const {
            if (vertex == from)
                return to;
            else if (vertex == to)
                return from;
            else
                throw std::invalid_argument("Vertex id must be either to or v");
        };
    };
    template<typename E=Edge>
    class Graph {
    private:
        int verticesNum_{};
        int edgesNum_{};
        std::vector<std::unordered_set<std::shared_ptr<E>>> adjList_;
    public:
        Graph() : Graph(0) {};
        explicit Graph(int verticesNum) : verticesNum_(verticesNum), edgesNum_(0), adjList_(verticesNum,
                                                                                            std::unordered_set<std::shared_ptr<E>>()) {}
        Graph(int verticesNum, const std::vector<std::tuple<int, int, int>> &initializer) : Graph<E>(verticesNum) {
            for (auto &element : initializer) {
                AddEdge(std::get<0>(element), std::get<1>(element), std::get<2>(element));
            }
        }
        Graph(Graph<E> &graph) = default;
        Graph(Graph<E>&& moved) noexcept : verticesNum_(moved.verticesNum_), edgesNum_(moved.edgesNum_), adjList_(std::move(moved.adjList_)) { };
        void AddEdge(int f_vertex, int s_vertex, int weight) {
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
            auto edge = std::make_shared<E>(f_vertex, s_vertex, weight);
            adjList_[f_vertex].insert(edge);
            adjList_[s_vertex].insert(edge);
            edgesNum_++;
        }

        void AddEdge(std::shared_ptr<E> &edge) {
            assert(edge->from >= 0 && edge->to < verticesNum_);
            for (auto &existingEdge : AdjList()[edge->from]) {
                if(edge.get() == existingEdge.get())
                    throw std::runtime_error("Edge added twice");
            }
            for (auto &existingEdge : AdjList()[edge->to]) {
                if(edge.get() == existingEdge.get())
                    throw std::runtime_error("Edge added twice");
            }
            edgesNum_++;
            adjList_[edge->from].insert(edge);
            adjList_[edge->to].insert(edge);
        }

        void DisconnectVertex(int vertexIndex) {
            for(auto &edge : adjList_[vertexIndex]) {
                adjList_[edge->Other(vertexIndex)].erase(edge);
                edgesNum_--;
            }
            adjList_[vertexIndex].clear();
        };
        void RemoveEdge(const std::shared_ptr<E> &edge)  {
            adjList_[edge->to].erase(edge);
            adjList_[edge->from].erase(edge);
            edgesNum_--;
        }
        [[nodiscard]] int EdgesNum() const {
            return edgesNum_;
        }

        [[nodiscard]] int VerticesNum() const {
            return verticesNum_;
        }

        [[nodiscard]] const std::vector<std::unordered_set<std::shared_ptr<E>>> &AdjList() const {
            return adjList_;
        }
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
            static_assert(std::is_same<decltype(Edge::from), const int>::value,
                          "Edge hash function was created only for int types");
            static_assert(std::is_same<decltype(Edge::to), const int>::value,
                          "Edge hash function was created only for int types");
            static_assert(std::is_same<decltype(Edge::weight), const int>::value,
                          "Edge hash function was created only for int types");
            // implementation based on https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
            // TODO: Test hash function
            return ((edge.from ^ (edge.to << 1)) >> 1) ^ (edge.weight << 1);
        }
    };
}
#endif //OPT_GRAPH_H
