//
// Created by Mateusz Chojnowski on 15.03.2021.
//

#include "Bipartiate.h"
#include <queue>
#include <stack>
using namespace bipariate;

template<typename GraphType>
// TODO: Improve Coloring algorithm
bool vertexColoring(BipariateInfo &info, GraphType &graph)
{
    // iterate trough vertexes and color rest of graph O(V + E)
    for (int vertex = 0; vertex < graph.VerticesNum(); ++vertex) {
        VertexColor current_color = info.vertexColors[vertex];
        for(auto &edge : graph.AdjList()[vertex])
        {
            if(info.vertexColors[edge.to] != VertexColor::UNCOLORED)
            {
                if (info.vertexColors[edge.to] == current_color)
                    return false;
                else if (current_color == VertexColor::UNCOLORED)
                {
                    if(info.vertexColors[edge.to] == VertexColor::RIGHT)
                    {
                        current_color = VertexColor::LEFT;
                        info.leftVertexes++;
                    }
                    else
                    {
                        current_color = VertexColor::RIGHT;
                        info.rightVertexes++;
                    }
                    info.vertexColors[vertex] = current_color;
                }

            }
            else if (info.vertexColors[edge.to] == VertexColor::UNCOLORED)
            {
                if(current_color == VertexColor::RIGHT)
                {
                    info.vertexColors[edge.to] = VertexColor::LEFT;
                    info.leftVertexes++;
                }
                else if(current_color == VertexColor::LEFT)
                {
                    info.vertexColors[edge.to] = VertexColor::RIGHT;
                    info.rightVertexes++;
                }
                else
                {
                    info.vertexColors[edge.to] = VertexColor::RIGHT;
                    info.vertexColors[vertex] = VertexColor::LEFT;
                    info.rightVertexes++;
                    info.leftVertexes++;
                }
            }
        }
    }
    return true;
}

std::variant<bool, BipariateInfo> bipariate::BipariateGraph(digraph::Digraph &graph) {
    BipariateInfo info{0, 0, std::vector<VertexColor>(graph.VerticesNum(), VertexColor::UNCOLORED)};
    std::queue<int> uncoloredVertices;
    // color first vertex
    for (int vertex = 0; vertex < graph.EdgesNum(); ++vertex) {
        if(std::size(graph.AdjList()[vertex]) > 0) {
            info.vertexColors[vertex] = VertexColor::LEFT;
            info.leftVertexes++;
            break;
        }
    }
    if(!vertexColoring(info, graph))
        return false;
    return info;
}


std::variant<bool, BipariateInfo> bipariate::BipariateGraph(graph::Graph<> &graph) {
    BipariateInfo info{0, 0, std::vector<VertexColor>(graph.VerticesNum(), VertexColor::UNCOLORED)};
    // color first vertex
    for (int vertex = 0; vertex < graph.EdgesNum(); ++vertex) {
        if(std::size(graph.AdjList()[vertex]) > 0) {
            info.vertexColors[vertex] = VertexColor::LEFT;
            info.leftVertexes++;
        }

    }
    // color rest of graph
    for (int vertex = 0; vertex < graph.EdgesNum(); ++vertex) {
        VertexColor current_color = info.vertexColors[vertex];
        for(auto &edge : graph.AdjList()[0])
        {
            if(info.vertexColors[edge->Other(vertex)] == current_color)
                return false;
            else if (info.vertexColors[edge->Other(vertex)] == VertexColor::UNCOLORED)
            {
                if(current_color == VertexColor::RIGHT)
                {
                    info.vertexColors[edge->Other(vertex)] = VertexColor::LEFT;
                    info.leftVertexes++;
                }
                else
                {
                    info.vertexColors[edge->Other(vertex)] = VertexColor::RIGHT;
                    info.rightVertexes++;
                }
            }
        }
    }
    return info;
}
