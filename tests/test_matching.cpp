//
// Created by Mateusz Chojnowski on 15.03.2021.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../src/Matching.h"
#include "../src/Digraph.h"

TEST(TestMatching, DirectionalMatching) {
    std::vector<std::tuple<int, int, int>> tup {{0, 7,  1},
                                                  {0, 8,  1},
                                                  {2, 6,  1},
                                                  {2, 9,  1},
                                                  {3, 8,  1},
                                                  {4, 8,  1},
                                                  {4, 9,  1},
                                                  {5, 11, 1}};
    digraph::Digraph graph(12, tup);
    auto result = matching::MaximumMatching(graph);
    ASSERT_TRUE(std::holds_alternative<matching::match>(result));
    auto match = std::get<matching::match>(result);
    ASSERT_TRUE(5 == std::size(match));
    ASSERT_THAT(match, testing::UnorderedElementsAre(
            std::pair<int, int>{4, 9},
            std::pair<int, int>{3, 8},
            std::pair<int, int>{0, 7},
            std::pair<int, int>{5, 11},
            std::pair<int, int>{2, 6}));
}

TEST(TestMatching, DirectionalMatchingUnequalSides) {
    std::vector<std::tuple<int, int, int>> tup {
            {0, 5, 1},
            {1, 5, 1},
            {1, 6, 1},
            {1, 7, 1},
            {2, 6, 1},
            {3, 6, 1},
            {3, 8, 1},
            {4, 8, 1}
    };
    digraph::Digraph graph(9, tup);
    auto result = matching::MaximumMatching(graph);
    ASSERT_TRUE(std::holds_alternative<matching::match>(result));
    auto match = std::get<matching::match>(result);
    ASSERT_TRUE(4 == std::size(match));
    ASSERT_THAT(match, testing::UnorderedElementsAre(
            std::pair<int, int>{2, 6},
            std::pair<int, int>{1, 7},
            std::pair<int, int>{3, 8},
            std::pair<int, int>{0, 5}));
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}