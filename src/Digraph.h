//
// Created by Mateusz Chojnowski on 02.03.2021.
// mail: 286333@stud.umk.pl
//

#ifndef OPT_DIGRAPH_H
#define OPT_DIGRAPH_H
#include <vector>
#include <tuple>
#include <stack>
#include <forward_list>
#include <variant>
namespace digraph {
    struct Edge {
        int to;
        int weight;
        Edge(int to, int weight) : to(to), weight(weight) {}
    };

    class Digraph {
    private:
        int verticesNum_;
        int edgesNum_;
        std::vector<std::vector<Edge>> adjList_;
        std::vector<int> toEdgesNum_;
    public:
        Digraph(int vertices_num);
        Digraph(int vertices_num, const std::vector<std::tuple<int, int, int>> &edges);
        void AddEdge(int from, int to, int weight);
        [[nodiscard]] int EdgesNum() const;
        [[nodiscard]] int VerticesNum() const;
        const std::vector<std::vector<Edge>> &AdjList() const;
        const std::vector<int> &InEdgesNum() const;
    };
}
#endif //OPTYMALIZACJA_DIGRAPH_H
