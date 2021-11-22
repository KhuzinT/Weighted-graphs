#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

//********************************************************************************************

using Vertex = uint32_t;

using WeightT = uint32_t;

const uint32_t kInfinity = 1000 * 1000 * 1000 * 1LL;

//********************************************************************************************

struct Edge {
    Vertex begin;
    Vertex end;

    WeightT weight;

    explicit Edge(const Vertex& a, const Vertex& b, const WeightT& w) {
        begin = a;
        end = b;
        weight = w;
    }

    bool operator<(const Edge& other) const {
        if (weight < other.weight) {
            return true;
        }
        if (weight > other.weight) {
            return false;
        }
        if (begin < other.begin) {
            return true;
        }
        if (begin > other.begin) {
            return false;
        }
        return end < other.end;
    }
};

class GraphListEdge {
private:
    uint32_t q_edge_ = 0;
    uint32_t q_vertex_ = 0;

    std::vector<Edge> list_;

    void Add(const Vertex& begin, const Vertex& end, const WeightT& weight) {
        Edge edge(begin, end, weight);
        list_.push_back(edge);
        ++q_edge_;
    }

public:
    explicit GraphListEdge(const uint32_t& quantity_vertex) {
        q_vertex_ = quantity_vertex;
    }

    uint32_t GetQVertex() const {
        return q_vertex_;
    }

    uint32_t GetQEdge() const {
        return q_edge_;
    }

    void AddEdge(const Vertex& begin, const Vertex& end, const WeightT& weight) {
        Add(begin - 1, end - 1, weight);
    }

    void SortEdges() {
        std::sort(list_.begin(), list_.end());
    }

    Edge GetEdge(const uint32_t& index) const {
        return list_[index];
    }
};

class DSU {
private:
    std::vector<uint32_t> parent_;
    std::vector<uint32_t> rank_;

public:
    explicit DSU(const uint32_t& quantity) {
        parent_.resize(quantity, kInfinity);
        for (uint32_t i = 0; i < quantity; ++i) {
            parent_[i] = i;
        }
        rank_.resize(quantity, 0);
    }

    uint32_t FindSet(const uint32_t& element) {
        if (element == parent_[element]) {
            return element;
        }
        parent_[element] = FindSet(parent_[element]);
        return parent_[element];
    }

    void UnionSets(const uint32_t& first_elem, const uint32_t& second_elem) {
        uint32_t new_first = FindSet(first_elem);
        uint32_t new_second = FindSet(second_elem);
        if (new_first != new_second) {
            if (rank_[new_first] < rank_[new_second]) {
                parent_[new_first] = new_second;
                if (rank_[new_first] == rank_[new_second]) {
                    ++rank_[new_second];
                }
            } else {
                parent_[new_second] = new_first;
                if (rank_[new_first] == rank_[new_second]) {
                    ++rank_[new_first];
                }
            }
        }
    }
};

uint32_t KruskalAlgorithm(GraphListEdge& graph) {
    uint32_t answer = 0;

    DSU system(graph.GetQVertex());
    graph.SortEdges();

    for (uint32_t i = 0; i < graph.GetQEdge(); ++i) {
        auto edge = graph.GetEdge(i);
        Vertex first_vertex = edge.begin;
        Vertex second_vertex = edge.end;

        if (system.FindSet(first_vertex) != system.FindSet(second_vertex)) {
            answer += edge.weight;
            system.UnionSets(first_vertex, second_vertex);
        }
    }

    return answer;
}

//********************************************************************************************

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    uint32_t q_vertex = 0;
    std::cin >> q_vertex;

    GraphListEdge graph(q_vertex + 1);

    std::vector<std::vector<Vertex>> matrix;
    for (uint32_t i = 0; i < q_vertex; ++i) {
        std::vector<Vertex> tmp(q_vertex, 0);
        matrix.push_back(tmp);
    }

    for (uint32_t i = 0; i < q_vertex; ++i) {
        for (uint32_t j = 0; j < q_vertex; ++j) {
            std::cin >> matrix[i][j];
            if (j < i) {
                graph.AddEdge(i + 1, j + 1, matrix[i][j]);
            }
        }
    }

    for (uint32_t i = 0; i < q_vertex; ++i) {
        uint32_t cost = 0;
        std::cin >> cost;
        graph.AddEdge(q_vertex + 1, i + 1, cost);
    }

    uint32_t answer = KruskalAlgorithm(graph);

    std::cout << answer;

    return 0;
}

//********************************************************************************************
