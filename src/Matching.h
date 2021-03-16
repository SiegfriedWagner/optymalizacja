//
// Created by mateu on 15.03.2021.
//

#ifndef OPTYMALIZACJA_MATCHING_H
#define OPTYMALIZACJA_MATCHING_H
#include <unordered_set>
#include "std_extensions.h"
#include "Digraph.h"
namespace matching {
    typedef std::unordered_set<std::pair<int, int>> match;
    std::variant<bool, match> MaximumMatching(digraph::Digraph &graph);
}

#endif //OPTYMALIZACJA_MATCHING_H
