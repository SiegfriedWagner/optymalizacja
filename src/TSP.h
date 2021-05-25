//
// Created by mateu on 03.05.2021.
//

#ifndef OPTYMALIZACJA_TSP_H
#define OPTYMALIZACJA_TSP_H

#include "./MatrixGraph.h"
#include "./BinaryTree.h"
#include <memory>
#include <cassert>
#include <ctime>
#include <cstdlib>

template<typename T>
void PrintArray(std::vector<std::vector<T>> &arr) {
    for (int i = 0; i < arr.size(); ++i) {
        for (int j = 0; j < arr.size(); ++j) {
            std::cout << arr[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

template<typename T>
void PrintArray(std::vector<T> &arr) {
    for (int i = 0; i < arr.size(); ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

struct SplitStep {
    int cost;
    std::vector<std::vector<int>> array;
    std::tuple<int, int> visited;
    std::vector<int> rows_left;
    std::vector<int> cols_left;

    void print() {
        // for debugging only
        std::cout << "Cost: " << cost << std::endl;
        std::cout << "Array: " << std::endl;
        PrintArray(array);
        std::cout << "Visited: (" << std::get<0>(visited) << ", " << std::get<1>(visited) << ")" << std::endl;
        std::cout << "Rows left: ";
        PrintArray(rows_left);
        std::cout << "Cols left: ";
        PrintArray(cols_left);
    }
};

namespace tsp {
    typedef std::vector<std::vector<int>> intmat;

    int MakeZeros(intmat &arr) {
        int subtractedTotal = 0;
        for (int row = 0; row < arr.size(); ++row) {
            int minValue = INT_MAX;
            for (int col = 0; col < arr[row].size(); ++col) {
                if (arr[row][col] == graph::MatrixGraph::noEdgeValue)
                    continue;
                minValue = std::min(minValue, arr[row][col]);
                if (minValue == 0)
                    // found 0 escaping
                    break;
            }
            if (minValue != 0) {
                subtractedTotal += minValue;
                for (int col = 0; col < arr[row].size(); ++col)
                    if (arr[row][col] != graph::MatrixGraph::noEdgeValue)
                        arr[row][col] -= minValue;
            }
        }
        for (int col = 0; col < arr[0].size(); ++col) {
            int minValue = INT_MAX;
            for (int row = 0; row < arr.size(); ++row) {
                if (arr[row][col] == graph::MatrixGraph::noEdgeValue)
                    continue;
                minValue = std::min(minValue, arr[row][col]);
                if (minValue == 0)
                    break;
            }
            if (minValue != 0) {
                subtractedTotal += minValue;
                for (int row = 0; row < arr.size(); ++row)
                    if (arr[row][col] > 0)
                        arr[row][col] -= minValue;
            }
        }
        return subtractedTotal;
    }

    bool FindZero(intmat &arr, int &row, int &col, bool randomPick) {
        if (randomPick) {
            int tempRow = -1;
            int tempCol = -1;
            for (row = 0; row < arr.size(); ++row)
                for (col = 0; col < arr[0].size(); ++col) {
                    if (arr[row][col] == 0) {
                        tempRow = row;
                        tempCol = col;
                        if (rand() % 100 > 50)
                            return true;
                    }
                }
            row = tempRow;
            col = tempCol;
            return true;
        } else {
            for (row = 0; row < arr.size(); ++row)
                for (col = 0; col < arr[0].size(); ++col)
                    if (arr[row][col] == 0)
                        return true;
        }
        return false;
    }

    int GetCost(std::shared_ptr<btree::BTreeNode<SplitStep>> node) {
        if (node == nullptr)
            return INT_MAX;
        else
            return node->GetData().cost;
    }

    void PropagateCost(std::shared_ptr<btree::BTreeNode<SplitStep>> current_step) {
        int pariMin = std::min(GetCost(current_step->GetLeft()), GetCost(current_step->GetRight()));
        while (GetCost(current_step) < pariMin) {
            current_step->GetData().cost = pariMin;
            if (current_step->HasParent())
                current_step = current_step->GetParent();
            pariMin = std::min(GetCost(current_step->GetLeft()), GetCost(current_step->GetRight()));
        }
    }

    bool AnyEdgesLeft(std::shared_ptr<btree::BTreeNode<SplitStep>> step) {
        assert(step != nullptr);
        assert(step->GetData().rows_left.size() == step->GetData().cols_left.size());
        return step->GetData().rows_left.size() != 0;
    }

    std::shared_ptr<btree::BTreeNode<SplitStep>>
    DescentLowestCost(std::shared_ptr<btree::BTreeNode<SplitStep>> step, int &descCount) {
        descCount = 0;
        while (step->GetLeft() != nullptr && step->GetRight() != nullptr) {
            int leftCost = step->GetLeft()->GetData().cost;
            int rightCost = step->GetRight()->GetData().cost;
            if (leftCost <= rightCost)
                step = step->GetLeft();
            else
                step = step->GetRight();
            descCount++;
        }
        return step;
    }

    void BranchAndBoundSolveStep(std::shared_ptr<btree::BTreeNode<SplitStep>> current_step, bool randomlyPickZeros) {
        using namespace btree;
        std::shared_ptr<BTreeNode<SplitStep>> right_node;
        {
            SplitStep right = {
                    .cost = current_step->GetData().cost,
                    .array = current_step->GetData().array,
                    .visited = std::make_tuple(-1, -1),
                    .rows_left = current_step->GetData().rows_left,
                    .cols_left = current_step->GetData().cols_left};
            right_node = std::make_shared<BTreeNode<SplitStep>>(std::move(right));
        }
        right_node->GetData().cost += MakeZeros(right_node->GetData().array);
        if (right_node->GetData().cost < current_step->GetData().cost)
            return;
        // select some col/row
        int selectedRow;
        int selectedCol;
        assert(FindZero(right_node->GetData().array, selectedRow, selectedCol, randomlyPickZeros));
        int newSize = right_node->GetData().array.size() - 1;
        std::shared_ptr<BTreeNode<SplitStep>> left_node;
        {
            assert(selectedRow < current_step->GetData().rows_left.size() && selectedCol >= 0);
            assert(selectedCol < current_step->GetData().rows_left.size() && selectedCol >= 0);
            SplitStep left = {
                    .cost = right_node->GetData().cost,
                    .array = intmat(newSize, std::vector<int>(newSize)),
                    .visited =  std::make_tuple(current_step->GetData().rows_left[selectedRow],
                                                current_step->GetData().cols_left[selectedCol]),
                    .rows_left = std::vector<int>(newSize),
                    .cols_left = std::vector<int>(newSize)
            };
            {
                int row = 0;
                for (int i = 0; i < right_node->GetData().rows_left.size(); ++i)
                    if (i != selectedRow)
                        left.rows_left[row++] = right_node->GetData().rows_left[i];
            }
            {
                int col = 0;
                for (int i = 0; i < right_node->GetData().cols_left.size(); ++i)
                    if (i != selectedCol)
                        left.cols_left[col++] = right_node->GetData().cols_left[i];
            }
            std::cout << std::endl;
            int newRow = 0;
            for (int row = 0; row < right_node->GetData().array.size(); ++row) {
                if (row == selectedRow)
                    continue;
                int newCol = 0;
                for (int col = 0; col < right_node->GetData().array.size(); ++col) {
                    if (col == selectedCol)
                        continue;
                    left.array[newRow][newCol] = right_node->GetData().array[row][col];
                    if (row == selectedCol && col == selectedRow)
                        left.array[newRow][newCol] = graph::MatrixGraph::noEdgeValue;
                    newCol++;
                }
                newRow++;
            }
            left_node = std::make_shared<BTreeNode<SplitStep>>(std::move(left));
        }
        // right step
        right_node->GetData().array[selectedRow][selectedCol] = graph::MatrixGraph::noEdgeValue;
        // check column if there is any zero left in column after removing edge
        {
            int min = INT_MAX;
            for (int row = 0; row < right_node->GetData().array.size(); ++row) {
                auto val = right_node->GetData().array[row][selectedCol];
                if (val != graph::MatrixGraph::noEdgeValue) {
                    min = std::min(min, val);
                    if (min == 0)
                        break;
                }
            }
            // subtract minimal value from column if needed
            if (min != 0) {
                for (int row = 0; row < right_node->GetData().array.size(); ++row) {
                    if (right_node->GetData().array[row][selectedCol] != graph::MatrixGraph::noEdgeValue)
                        right_node->GetData().array[row][selectedCol] -= min;
                }
                right_node->GetData().cost += min;
            }
        }
        // check row if there is any zero left in row after removing edge
        {
            int min = INT_MAX;
            for (int col = 0; col < right_node->GetData().array[0].size(); ++col) {
                auto val = right_node->GetData().array[selectedRow][col];
                if (val != graph::MatrixGraph::noEdgeValue) {
                    min = std::min(min, val);
                    if (min == 0)
                        break;
                }
            }
            // subtract minimal value from column if needed
            if (min != 0) {
                for (int col = 0; col < right_node->GetData().array[0].size(); ++col) {
                    if (right_node->GetData().array[selectedRow][col] != graph::MatrixGraph::noEdgeValue)
                        right_node->GetData().array[selectedRow][col] -= min;
                }
                right_node->GetData().cost += min;
            }
        }
        // avoid adding invalid right node where algorithm was unable to find alternative path
        if (right_node->GetData().cost < current_step->GetData().cost)
            right_node->GetData().cost = INT_MAX;
        BTreeNode<SplitStep>::SetRight(current_step, right_node);
        BTreeNode<SplitStep>::SetLeft(current_step, left_node);
    }

    void BranchAndBoundSolve(graph::MatrixGraph &matrixGraph, bool randomlyPickZeros) {
        using namespace btree;
        assert(matrixGraph.Matrix().size() == matrixGraph.Matrix()[0].size());
        // init btree root node
        std::shared_ptr<BTreeNode<SplitStep>> root_node;
        {
            SplitStep root = {
                    .cost = 0,
                    .array = matrixGraph.Matrix(),
                    .visited = std::make_tuple(-1, -1),
                    .rows_left = std::vector<int>(matrixGraph.Matrix().size()),
                    .cols_left = std::vector<int>(matrixGraph.Matrix().size())};
            root_node = std::make_shared<BTreeNode<SplitStep>>(std::move(root));
        }
        for (int index = 0; index < root_node->GetData().rows_left.size(); ++index) {
            root_node->GetData().cols_left[index] = index;
            root_node->GetData().rows_left[index] = index;
        }
        // solve
        {
            std::shared_ptr<BTreeNode<SplitStep>> current_node = root_node;
            int step = 0;
            while (AnyEdgesLeft(current_node)) {
                std::cout << std::endl << "Step: " << step << std::endl;
                BranchAndBoundSolveStep(current_node, randomlyPickZeros);
                PropagateCost(current_node);
                std::cout << "Root" << std::endl;
                current_node->GetData().print();
                std::cout << "Right" << std::endl;
                if (current_node->GetRight() != nullptr)
                    current_node->GetRight()->GetData().print();
                std::cout << "Left" << std::endl;
                if (current_node->GetLeft() != nullptr)
                    current_node->GetLeft()->GetData().print();
                current_node = DescentLowestCost(root_node, step);
            }
        }
        // check path
        std::shared_ptr<BTreeNode<SplitStep>> current_node = root_node;
        std::tuple<int, int> invalid = std::make_tuple(-1, -1);
        int totalCost = 0;
        while (current_node != nullptr) {
            if (current_node->GetData().visited != invalid) {
                auto row = std::get<0>(current_node->GetData().visited);
                auto col = std::get<1>(current_node->GetData().visited);
                std::cout << "(" << row + 1 << ", " << col + 1 << "): " << root_node->GetData().array[row][col]
                          << std::endl;
                totalCost += root_node->GetData().array[row][col];
            }
            std::cout << "Total cost: " << totalCost << std::endl;
            auto left_cost = INT_MAX;
            auto righ_cost = INT_MAX;
            if (current_node->GetRight() != nullptr) {
                auto cost = current_node->GetRight()->GetData().cost;
                if (cost > 0)
                    righ_cost = cost;
            }
            if (current_node->GetLeft() != nullptr) {
                auto cost = current_node->GetLeft()->GetData().cost;
                if (cost > 0)
                    left_cost = cost;
            }
            current_node = left_cost <= righ_cost ? current_node->GetLeft() : current_node->GetRight();
        }
    }
}
#endif //OPTYMALIZACJA_TSP_H
