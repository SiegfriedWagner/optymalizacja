//
// Created by mateu on 06.06.2021.
//
#include <fstream>
#include <random>
#include <cfloat>
#include "../external/argparse/argparse.h"
#include "../src/IOHelpers.h"
#include "../src/Digraph.h"
#include "../src/Graph.h"

struct AntEdge : graph::Edge {
    float pheromones;
    AntEdge(int f_vertex, int s_vertex, int weight) : Edge(f_vertex, s_vertex, weight), pheromones(0.0) { }
};

class Ant {
private:
    int starting_node;
    float total_cost;
    digraph::Digraph path;
    std::unordered_set<std::shared_ptr<AntEdge>> original_path_edges;
public:
    Ant(int starting_node, int totalnodes) : starting_node(starting_node), total_cost(0), path(totalnodes) { }
    [[nodiscard]] bool has_valid_path() {
        return path.VerticesNum() == path.EdgesNum();
    }
    [[nodiscard]] const digraph::Digraph &Path() const {
        return path;
    }
    const std::unordered_set<std::shared_ptr<AntEdge>> &OriginalPathEdges() const {
        return original_path_edges;
    }
    float PathCost() const {
        return total_cost;
    }
    void traverse_graph(const graph::Graph<AntEdge> &traversed, float alpha, float beta) {
        // alpha - pheromone factor
        // beta - local distance factor
        int currentNode = starting_node;
        bool canMakeValidMove = true;
        while(canMakeValidMove) {
            canMakeValidMove = false;
            std::shared_ptr<AntEdge> next_edge = nullptr;
            double cumulativeSum = 0.0;
            // calculate cumulative sum
            for (const auto &edge : traversed.AdjList()[currentNode]) {
                if (path.AdjList()[edge->Other(currentNode)].empty())
                {
                    cumulativeSum += std::pow(edge->pheromones, alpha) + std::pow(1.0f / (float) edge->weight, beta);
                }
            }
            // pick next hop based on weighted random choice
            double next_pick_weight = std::rand() * cumulativeSum / RAND_MAX;
            // pick edge
            cumulativeSum = 0.0;
            for (const auto &edge : traversed.AdjList()[currentNode]) {
                if (path.AdjList()[edge->Other(currentNode)].empty()) {
                    cumulativeSum += std::pow(edge->pheromones, alpha) + std::pow(1.0f / (float) edge->weight, beta);
                    if (cumulativeSum >= next_pick_weight) {
                        next_edge = edge;
                        break;
                    }
                }
            }
            if (next_edge == nullptr)
                continue;
            canMakeValidMove = true;
            path.AddEdge(currentNode, next_edge->Other(currentNode), next_edge->weight);
            original_path_edges.insert(next_edge);
            assert(path.AdjList()[currentNode].size() == 1);
            total_cost += (float) next_edge->weight;
            currentNode = next_edge->Other(currentNode);
        }
        // close path or make it empty
        if (path.VerticesNum() - 1== path.EdgesNum()) {
            for(const auto &edge : traversed.AdjList()[currentNode])
                if (edge->Other(currentNode) == starting_node) {
                    original_path_edges.insert(edge);
                    path.AddEdge(currentNode, starting_node, edge->weight);
                    total_cost = edge->weight;
                    break;
                }
        }
        else {
            path = digraph::Digraph(traversed.VerticesNum());
            original_path_edges.clear();
            total_cost = 0;
        }
    }
};

digraph::Digraph solve_with_ant_colony(graph::Graph<AntEdge> &graph, int iterations, float alpha, float beta, float rho) {
    // alpha - pheromone factor
    // beta - local distance factor
    // rho - pheromone evaporation factor
    graph::Graph<AntEdge> antGraph(graph.VerticesNum());
    for(int i=0; i < graph.VerticesNum(); i++)
        for(const auto &edge : graph.AdjList()[i])
            if (edge->to > i)
                antGraph.AddEdge(edge->from, edge->to, edge->weight);
    digraph::Digraph result(0);
    auto best_total_cost = DBL_MAX;
    for (int i = 0; i < iterations; ++i) {
        Ant ant(std::rand() % graph.VerticesNum(), graph.VerticesNum());
        ant.traverse_graph(graph, alpha, beta);
        if (ant.has_valid_path()) {
            if (ant.PathCost() < best_total_cost)
                result = ant.Path();
            float pheromone_increment = 1.0f / ant.PathCost();
            for(auto &edge : ant.OriginalPathEdges()) {
                edge->pheromones += pheromone_increment;
            }
        }
        // evaporate pheromones
        for(auto &list : graph.AdjList())
            for(auto &edge : list)
                edge->pheromones *= 1.0f - rho;
    }
    return result;
}

int main(int argc, char *argv[])
{
    std::srand(1000);
    argparse::ArgumentParser parser("Traveling Salesman Problem - Ant colony");
    parser.add_description("Program that solves TSP using Ant colony");
    parser.add_argument("input_file")
            .help("Input file to parse")
            .action([](const std::string &value) {
                std::fstream  inputFile;
                inputFile.open(value.c_str(), std::fstream::in);
                auto result = io_helpers::ParseFile<int, int>(inputFile);
                if (result.isDirected)
                    throw std::runtime_error("Directed graphs are not supported");
                graph::Graph<AntEdge> graph(result.verticesNum, result.edges);
                auto path = solve_with_ant_colony(graph, 1000, 0.4f, 0.8f, 0.01f);
                io_helpers::PrintGraph(path);
                std::cout << "Total cost: " << path.AllEdgesCost() << std::endl;
                inputFile.close();
            });
    parser.parse_args(argc, argv);
    return 0;
}