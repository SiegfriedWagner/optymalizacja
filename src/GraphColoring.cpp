//
// Created by mateu on 29.05.2021.
//

#include "GraphColoring.h"
#include <cassert>
using namespace std;
const size_t NOT_COLORED = -1;

vector<int> ApproximatelyMaximumIndependentSubset(graph::Graph graph, unordered_set<int> considered_nodes) {
    vector<int> maximumSubset;
    while(!considered_nodes.empty()) {
        int minimumDegreeFound = INT_MAX;
        int minimumVertexIdx = -1;
        for (auto vertex : considered_nodes) {
            auto adjList = graph.AdjList()[vertex];
            if (adjList.empty() || adjList.size() < minimumDegreeFound) {
                minimumVertexIdx = vertex;
                minimumDegreeFound = adjList.size();
            }
            if (minimumDegreeFound == 0) break;
        }
        assert(minimumVertexIdx != -1);
        {
            vector<int> toDisconnect(graph.AdjList()[minimumVertexIdx].size());
            for (auto &edge : graph.AdjList()[minimumVertexIdx])
                toDisconnect.push_back(edge->Other(minimumVertexIdx));
            for(auto disconecting : toDisconnect) {
                graph.DisconnectVertex(disconecting);
                considered_nodes.erase(disconecting);
            }
        }
        assert(graph.AdjList()[minimumVertexIdx].empty());
        considered_nodes.erase(minimumVertexIdx);
        maximumSubset.push_back(minimumVertexIdx);
    }
    return maximumSubset;
}

GraphColoringResult ColorGraph(graph::Graph graph) {
    size_t currentColorNum = 0;
    vector<size_t> colors(graph.VerticesNum(), NOT_COLORED);
    size_t processedVertices = 0;
    unordered_set<int> considered_nodes;
    // process lonely (and sad :<) vertices
    for (int i = 0; i < graph.VerticesNum(); ++i) {
        if(graph.AdjList()[i].empty()) {
            colors[i] = currentColorNum;
            processedVertices++;
        }
        else
            considered_nodes.insert(i);
    }
    // process rest of vertices
    while(!considered_nodes.empty()) {
        auto verticesToColor = ApproximatelyMaximumIndependentSubset(graph, considered_nodes);
        for (auto vertexToColor : verticesToColor) {
            colors[vertexToColor] = currentColorNum;
            graph.DisconnectVertex(vertexToColor);
            considered_nodes.erase(vertexToColor);
            processedVertices++;
        }
        currentColorNum++;
    }
    if (processedVertices != graph.VerticesNum()) {
        for (size_t & color : colors) {
            if (color == NOT_COLORED)
                color = currentColorNum;
        }
        currentColorNum++;
    }
    return {
        .colors = move(colors),
        .numberOfColorsUsed = currentColorNum
    };
}