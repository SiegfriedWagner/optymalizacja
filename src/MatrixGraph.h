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
        int verticesNum_;
        std::vector<std::vector<int>> matrix_;
    public:
        static const int noEdgeValue = -1;
        MatrixGraph(bool directed, int verticesNum, const std::vector<std::tuple<int, int, int>> initializer);
        MatrixGraph(int verticesNum, const std::vector<std::tuple<int, int, int>> initializer);
        MatrixGraph(int verticesNum);
        std::vector<std::vector<int>>& Matrix();
        void AddEdge(int from, int to, int weight);
    };
}
#endif //OPTYMALIZACJA_MATRIXGRAPH_H
