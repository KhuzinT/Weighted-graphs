#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

//********************************************************************************************

using Vertex = uint64_t;

using WeightT = uint64_t;

const uint64_t kInfinity = 1000 * 1000 * 1000 * 1LL;

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
    uint64_t q_edge_ = 0;
    uint64_t q_vertex_ = 0;

    std::vector<Edge> list_;

    void Add(const Vertex& begin, const Vertex& end, const WeightT& weight) {
        Edge edge(begin, end, weight);
        list_.push_back(edge);
        ++q_edge_;
    }

public:
    explicit GraphListEdge(const uint64_t& quantity_vertex) {
        q_vertex_ = quantity_vertex;
    }

    uint64_t GetQVertex() const {
        return q_vertex_;
    }

    uint64_t GetQEdge() const {
        return q_edge_;
    }

    void AddEdge(const Vertex& begin, const Vertex& end, const WeightT& weight) {
        Add(begin - 1, end - 1, weight);
    }

    void SortEdges() {
        std::sort(list_.begin(), list_.end());
    }

    Edge GetEdge(const uint64_t& index) const {
        return list_[index];
    }
};

class DSU {
private:
    std::vector<uint64_t> parent_;
    std::vector<uint64_t> rank_;

public:
    explicit DSU(const uint64_t& quantity) {
        parent_.resize(quantity, kInfinity);
        for (uint64_t i = 0; i < quantity; ++i) {
            parent_[i] = i;
        }
        rank_.resize(quantity, 0);
    }

    uint64_t FindSet(const uint64_t& element) {
        if (element == parent_[element]) {
            return element;
        }
        parent_[element] = FindSet(parent_[element]);
        return parent_[element];
    }

    void UnionSets(const uint64_t& first_elem, const uint64_t& second_elem) {
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

uint64_t KruskalAlgorithm(GraphListEdge& g) {
    uint64_t answer = 0;

    DSU system(g.GetQVertex());
    g.SortEdges();

    for (uint64_t i = 0; i < g.GetQEdge(); ++i) {
        auto edge = g.GetEdge(i);
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

    uint64_t q_vertex = 0;
    uint64_t q_edge = 0;
    std::cin >> q_vertex >> q_edge;

    GraphListEdge g(q_vertex);

    for (uint64_t i = 0; i < q_edge; ++i) {
        Vertex begin = 0;
        Vertex end = 0;
        WeightT weight = 0;
        std::cin >> begin >> end >> weight;
        g.AddEdge(begin, end, weight);
    }

    uint64_t answer = KruskalAlgorithm(g);

    std::cout << answer;

    return 0;
}

//********************************************************************************************
