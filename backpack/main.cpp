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
//void propagade_upward(shared_ptr<BTreeNode<NodeData>> &node, float maximumWeight) {
//    if (node == nullptr)
//        return;
//    // process left
//    auto left_node = node->GetLeft();
//    if (left_node != nullptr)
//    {
//        auto &left_data = left_node->GetData();
//        if (left_data.total_weight > maximumWeight)
//            node.
//    }
//    auto &left_data = node->GetLeft()->GetData();
//    if(!left_data.canGoFurther && left_data.total_weight)
//}

bool is_right_valid(const shared_ptr<BTreeNode<NodeData>> &node) {
    return node->GetRight() != nullptr && node->GetRight()->GetData().canGoFurther;
}

bool is_left_valid(const shared_ptr<BTreeNode<NodeData>> &node) {
    return node->GetLeft() != nullptr && node->GetLeft()->GetData().canGoFurther;
}
void pop_solution(shared_ptr<BTreeNode<NodeData>> leaf_node, vector<int> &best_solution_container) {
    best_solution_container.clear();
    while(leaf_node != nullptr) {
        if (leaf_node->IsLeftChild())
            best_solution_container.push_back(leaf_node->GetData().depth - 1);
        leaf_node = leaf_node->GetParent();
    }
}

void solve_backpack_step(shared_ptr<BTreeNode<NodeData>> current, const vector<float> &values,
                         const vector<float> &weights, float maximumWeight, float &best_solution_value, vector<int> &best_solution) {
    // descent left if possible
    bool left_valid = is_left_valid(current);
    cout << "Path start" << endl;
    while(left_valid || is_right_valid(current)) {

        if (left_valid) {
            cout << "Going left: " << current->GetData().depth << endl;
            current = current->GetLeft();
        }
        else {
            cout << "Going right: " << current->GetData().depth << endl;
            current = current->GetRight();
        }
        left_valid = is_left_valid(current);
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
            return;
        }
        // check if left can be created
        if (current_data.total_weight + weights[current_data.depth] <= maximumWeight)
        {
            // current_data.value += values[current_data.depth];
            BTreeNode<NodeData>::SetLeft(current, make_shared<BTreeNode<NodeData>>(NodeData{
                current_data.total_weight + weights[current_data.depth], current_data.value + values[current_data.depth],
                current_data.depth + 1, true}));
        }
        // check if right can be created
        BTreeNode<NodeData>::SetRight(current, make_shared<BTreeNode<NodeData>>(NodeData{
                current_data.total_weight, current_data.value ,
                current_data.depth + 1, true}));
        return;
    }
    else {
        BTreeNode<NodeData>::SetRight(current, nullptr);
        BTreeNode<NodeData>::SetLeft(current, nullptr);
        current->GetData().canGoFurther = false;
    }




}
//shared_ptr<BTreeNode<NodeData>> descent_highest_value(const shared_ptr<BTreeNode<NodeData>> &node) {
//    while(node.)
//    auto left_data = node->GetLeft()->GetData();
//    auto right_data = node->GetRight()->GetData();
//    if (left_data.canGoFurther && right_data.canGoFurther)
//        if (left_data.value >= right_data.value)
//            return descent_highest_value(node->GetLeft());
//        else
//            return descent_highest_value(node->GetRight());
//
//}
vector<int> solve_backpacking(const vector<float> &values, const vector<float> &weights, float maximumWeight) {
    float best_solution_value = -1;
    vector<int> result;
    shared_ptr<BTreeNode<NodeData>> root = make_shared<BTreeNode<NodeData>>(NodeData{0.0, 0.0, 0, true});
    // BTreeNode<NodeData>::SetLeft(root, make_shared<BTreeNode<NodeData>>(NodeData{weights[0], values[0], 1, true}));
    // BTreeNode<NodeData>::SetRight(root, make_shared<BTreeNode<NodeData>>(NodeData{0.0, 0.0, 1, true}));
    while(root->GetData().canGoFurther) {
        solve_backpack_step(root, values, weights, maximumWeight, best_solution_value, result);
    }
    return result;
}

int main(int argc, char *argv[]) {
    vector<float> values { 2.0f, 3.0f, 6.0f, 3.0f };
    vector<float> weights { 1.0f, 2.0f, 5.0f, 4.0f };
    assert(values.size() == weights.size());
    float maximumWeight = 5.0f;
    solve_backpacking(values, weights, maximumWeight);
    return 0;
}