//
// Created by mateu on 07.06.2021.
//
#include <cfloat>
#include <cassert>
#include <vector>
#include <iostream>
#include "../src/BinaryTree.h"

using namespace std;
using namespace btree;
struct NodeData {
    float total_weight = 0.0f;
    float value = 0.0f;
    int depth = 0;
    bool canGoFurther = true;
};

bool is_right_valid(const shared_ptr<BTreeNode<NodeData>> &node) {
    return node->GetRight() != nullptr && node->GetRight()->GetData().canGoFurther;
}

void pop_solution(shared_ptr<BTreeNode<NodeData>> leaf_node, vector<int> &best_solution_container) {
    best_solution_container.clear();
    while (leaf_node != nullptr) {
        if (leaf_node->IsLeftChild())
            best_solution_container.push_back(leaf_node->GetData().depth - 1);
        leaf_node = leaf_node->GetParent();
    }
}

shared_ptr<BTreeNode<NodeData>>
solve_backpack_step(shared_ptr<BTreeNode<NodeData>> current, const vector<float> &values,
                    const vector<float> &weights, float maximumWeight, float &best_solution_value,
                    vector<int> &best_solution) {
    // descent right if possible
    if (is_right_valid(current)) {
        cout << "Moving down right" << endl;
        current = current->GetRight();
    }
    // extend or shrink current node
    assert(current->GetData().canGoFurther);
    if (current->GetLeft() == nullptr && current->GetRight() == nullptr) {
        auto &current_data = current->GetData();
        // check if deepest node and compare solution
        if (current_data.depth == weights.size()) {
            if (current_data.value > best_solution_value) {
                best_solution_value = current_data.value;
                cout << "Found potential solution - value: " << best_solution_value << endl;
                pop_solution(current, best_solution);
                for (auto val : best_solution) {
                    cout << val << " ";
                }
                cout << endl;
            }
            current_data.canGoFurther = false;
            cout << "Moving up" << endl;
            return current->GetParent();
        }
        // check if left can be created
        if (current_data.total_weight + weights[current_data.depth] <= maximumWeight) {
            // current_data.value += values[current_data.depth];
            BTreeNode<NodeData>::SetLeft(current, make_shared<BTreeNode<NodeData>>(NodeData{
                    current_data.total_weight + weights[current_data.depth],
                    current_data.value + values[current_data.depth],
                    current_data.depth + 1, true}));
        }
        // check if right can be created
        BTreeNode<NodeData>::SetRight(current, make_shared<BTreeNode<NodeData>>(NodeData{
                current_data.total_weight, current_data.value,
                current_data.depth + 1, true}));
        if (current->GetLeft() != nullptr) {
            cout << "Moving down left" << endl;
            return current->GetLeft();
        }
        cout << "Moving down right" << endl;
        return current->GetRight();
    } else {
        BTreeNode<NodeData>::SetRight(current, nullptr);
        BTreeNode<NodeData>::SetLeft(current, nullptr);
        current->GetData().canGoFurther = false;
    }
    cout << "Moving up" << endl;
    return current->GetParent();
}

vector<int> solve_backpacking(const vector<float> &values, const vector<float> &weights, float maximumWeight) {
    float best_solution_value = -1;
    vector<int> result;
    shared_ptr<BTreeNode<NodeData>> root = make_shared<BTreeNode<NodeData>>(NodeData{0.0, 0.0, 0, true});
    shared_ptr<BTreeNode<NodeData>> current = root;
    while (root->GetData().canGoFurther) {
        current = solve_backpack_step(current, values, weights, maximumWeight, best_solution_value, result);
    }
    return result;
}

int main(int argc, char *argv[]) {
    vector<float> values{2.0f, 7.0f, 6.0f, 3.0f, 4.0f, 5.0f};
    vector<float> weights{1.0f, 2.0f, 4.0f, 4.0f, 1.0f, 3.0f};
    assert(values.size() == weights.size());
    float maximumWeight = 5.0f;
    auto result = solve_backpacking(values, weights, maximumWeight);
    if (!result.empty()) {
        cout << "Found solution, element indexes are: " << endl;
        float totoal = 0.0f;
        for (auto val : result) {
            cout << val << " ";
            totoal += values[val];
        }
        cout << endl << "Total:" << totoal << endl;
    }
    else {
        cout << "Solution not found" << endl;
    }
    return 0;
}