//
// Created by Mateusz Chojnowski on 28.03.2021.
//

#ifndef OPTYMALIZACJA_HUNGARIAN_H
#define OPTYMALIZACJA_HUNGARIAN_H
#include <variant>
#include <unordered_set>
#include "Digraph.h"
#include "std_extensions.h"

namespace matching {
    typedef std::unordered_set<std::pair<int, int>> match;
    std::variant<bool, match> HungarianMatch(digraph::Digraph &graph);
}
#endif //OPTYMALIZACJA_HUNGARIAN_H
