#include <iostream>
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

uint64_t BoruvkaAlgorithm(GraphListEdge& g) {
    std::vector<uint64_t> min_cost(g.GetQVertex(), kInfinity);

    DSU system(g.GetQVertex());

    uint64_t answer = 0;
    uint64_t quantity = g.GetQVertex();

    while (quantity > 1) {

        for (uint64_t i = 0; i < g.GetQEdge(); ++i) {
            uint64_t begin_leader = system.FindSet(g.GetEdge(i).begin);
            uint64_t end_leader = system.FindSet(g.GetEdge(i).end);

            if (begin_leader == end_leader) {
                continue;
            }

            if (min_cost[begin_leader] == kInfinity) {
                min_cost[begin_leader] = i;
            }
            if (min_cost[end_leader] == kInfinity) {
                min_cost[end_leader] = i;
            }

            if (g.GetEdge(min_cost[begin_leader]).weight > g.GetEdge(i).weight) {
                min_cost[begin_leader] = i;
            }
            if (g.GetEdge(min_cost[end_leader]).weight > g.GetEdge(i).weight) {
                min_cost[end_leader] = i;
            }
        }

        for (uint64_t i = 0; i < g.GetQVertex(); ++i) {
            if (min_cost[i] != kInfinity) {
                uint64_t begin_leader = system.FindSet(g.GetEdge(min_cost[i]).begin);
                uint64_t end_leader = system.FindSet(g.GetEdge(min_cost[i]).end);

                if (begin_leader == end_leader) {
                    continue;
                }

                answer += g.GetEdge(min_cost[i]).weight;
                system.UnionSets(begin_leader, end_leader);
                --quantity;
            }
        }

        min_cost.clear();
        min_cost.resize(g.GetQVertex(), kInfinity);
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

    uint64_t answer = BoruvkaAlgorithm(g);

    std::cout << answer;

    return 0;
}

//********************************************************************************************
