//
// Created by mateu on 04.05.2021.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../src/BinaryTree.h"
using namespace btree;

TEST(TESTBTree, BTreeLeft) {
    std::shared_ptr<BTreeNode<int>> root = std::make_shared<BTreeNode<int>>(10);
    std::shared_ptr<BTreeNode<int>> left = std::make_shared<BTreeNode<int>>(1);
    BTreeNode<int>::SetLeft(root, left);
    ASSERT_TRUE(left->IsLeftChild());
    ASSERT_TRUE(root->GetLeft() == left);
}

TEST(TESTBTree, BTreeRight) {
    std::shared_ptr<BTreeNode<int>> root = std::make_shared<BTreeNode<int>>(10);
    std::shared_ptr<BTreeNode<int>> left = std::make_shared<BTreeNode<int>>(1);
    BTreeNode<int>::SetRight(root, left);
    ASSERT_TRUE(left->IsRightChild());
    ASSERT_TRUE(root->GetRight() == left);
}


//int main(int argc, char *argv[])
//{
//    testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
//}