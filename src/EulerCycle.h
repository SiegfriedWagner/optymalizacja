//
// Created by mateu on 07.03.2021.
//

#ifndef OPT_EULER_H
#define OPT_EULER_H
#include <variant>
#include <forward_list>
#include "Digraph.h"
#include "Graph.h"
namespace euler_cycle {
    std::variant<bool, std::forward_list<int>> EulerianCycle(graph::Graph &&graph);
    std::variant<bool, std::forward_list<int>> EulerianCycle(graph::Graph &graph);
    std::variant<bool, std::forward_list<int>> EulerianCycle(digraph::Digraph &&graph);
    std::variant<bool, std::forward_list<int>> EulerianCycle(digraph::Digraph &graph);
}
#endif //OPT_EULER_H
