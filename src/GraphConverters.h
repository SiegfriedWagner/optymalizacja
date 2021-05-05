//
// Created by mateu on 03.05.2021.
//

#ifndef OPTYMALIZACJA_GRAPHCONVERTERS_H
#define OPTYMALIZACJA_GRAPHCONVERTERS_H

#include "Graph.h"
#include "Digraph.h"
#include "MatrixGraph.h"

namespace graph_converters {
    MatrixGraph ToMatrixGraph(digraph::Digraph &graph) {
        MatrixGraph matrix(graph.VerticesNum());
        for(int vertexIndex = 0; vertexIndex < graph.VerticesNum(); ++vertexIndex)
            for(auto &edge : graph.AdjList()[vertexIndex])
                matrix.AddEdge(vertexIndex, edge.to, edge.weight);
        return matrix;
    }
}

#endif //OPTYMALIZACJA_GRAPHCONVERTERS_H
