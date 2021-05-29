//
// Created by mateu on 04.05.2021.
//
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../src/TSP.h"

TEST(TestTSPInternal, SplitStepIsLeft) {
    std::shared_ptr<SplitStep> root = std::make_shared<SplitStep>();
    // ASSERT_TRUE(left->isLeftChild());
}


int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
