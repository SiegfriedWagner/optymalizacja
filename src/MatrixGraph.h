//
// Created by Mateusz Chojnowski on 03.05.2021.
//

#ifndef OPTYMALIZACJA_MATRIXGRAPH_H
#define OPTYMALIZACJA_MATRIXGRAPH_H
#include <vector>
#include <tuple>
#include <algorithm>
namespace graph {
    class MatrixGraph {
    private:
        size_t verticesNum_;
        std::vector<std::vector<int>> matrix_;
    public:
        static const int noEdgeValue = -1;
        MatrixGraph(bool directed, int verticesNum, const std::vector<std::tuple<int, int, int>> initializer);
        MatrixGraph(int verticesNum, const std::vector<std::tuple<int, int, int>> initializer);
        MatrixGraph(int verticesNum);
        std::vector<std::vector<int>>& Matrix();
        [[nodiscard]] const std::vector<std::vector<int>>& Matrix() const;
        void AddEdge(int from, int to, int weight);
        [[nodiscard]] size_t VerticesNum() const;
        [[nodiscard]] int Weight(int from, int to) const;
    };
}
#endif //OPTYMALIZACJA_MATRIXGRAPH_H
