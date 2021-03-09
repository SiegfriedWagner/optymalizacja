//
// Created by mateu on 08.03.2021.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../src/EulerCycle.h"

TEST(TestEuler, DigraphHasCycle) {
    std::vector<std::tuple<int, int, int>> initializer = {{0, 2, 1},
                                                          {2, 1, 1},
                                                          {1, 0, 1},
                                                          {0, 3, 1},
                                                          {3, 4, 1},
                                                          {4, 0, 1}};
    digraph::Digraph graph(5, initializer);
    auto result = euler_cycle::EulerianCycle(graph);
    ASSERT_TRUE(std::holds_alternative<std::forward_list<int>>(result));
    ASSERT_THAT(std::get<std::forward_list<int>>(result), testing::ElementsAre(0, 2, 1, 0, 3, 4, 0));
}

TEST(TestEuler, GraphHasCycle) {
    std::vector<std::tuple<int, int, int>> initializer = {{0, 1, 1},
                                                          {0, 3, 1},
                                                          {1, 2, 1},
                                                          {1, 3, 1},
                                                          {1, 4, 1},
                                                          {2, 3, 1},
                                                          {3, 4, 1}};
    graph::Graph graph(5, initializer);
    auto result = euler_cycle::EulerianCycle(graph);
    ASSERT_TRUE(std::holds_alternative<std::forward_list<int>>(result));
    ASSERT_THAT(std::get<std::forward_list<int>>(result), testing::ElementsAre(0, 1, 2, 3, 1, 4, 3, 0));
}

TEST(TestEuler, NoEdges) {
    std::vector<std::tuple<int, int, int>> initializer = {{1, 2, 3}};
    auto result = euler_cycle::EulerianCycle(graph::Graph(5, initializer));
    ASSERT_TRUE(std::holds_alternative<bool>(result));
    ASSERT_FALSE(std::get<bool>(result));
    result= euler_cycle::EulerianCycle( digraph::Digraph(5, initializer));
    ASSERT_TRUE(std::holds_alternative<bool>(result));
    ASSERT_FALSE(std::get<bool>(result));
}

TEST(TestEuler, GraphHasNoCycle) {
    std::vector<std::tuple<int, int, int>> initializer = {{0, 1, 1},
                                                          {1, 2, 1},
                                                          {2, 3, 1},
                                                          {3, 0, 1},
                                                          {0, 2, 1}};
    auto result = euler_cycle::EulerianCycle(graph::Graph(5, initializer));
    ASSERT_TRUE(std::holds_alternative<bool>(result));
    ASSERT_FALSE(std::get<bool>(result));
    result= euler_cycle::EulerianCycle( digraph::Digraph(5, initializer));
    ASSERT_TRUE(std::holds_alternative<bool>(result));
    ASSERT_FALSE(std::get<bool>(result));
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}