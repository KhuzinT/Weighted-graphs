#include <iostream>
#include <vector>

const uint64_t kInfinity = 100 * 100 * 100;

struct Edge {
    uint64_t begin;
    uint64_t end;

    uint64_t weight;

    explicit Edge(const uint64_t& a, const uint64_t& b, const uint64_t& w) {
        begin = a - 1;
        end = b - 1;

        weight = w;
    }
};

class DSU {
private:
    std::vector<uint64_t> parent_;
    std::vector<uint64_t> rank_;

    std::vector<uint64_t> leader_weight_;

public:
    explicit DSU(const uint64_t& quantity) {
        parent_.resize(quantity, kInfinity);
        for (uint64_t i = 0; i < quantity; ++i) {
            parent_[i] = i;
        }
        rank_.resize(quantity, 0);
        leader_weight_.resize(quantity, 0);
    }

    uint64_t FindSet(const uint64_t& element) {
        if (element == parent_[element]) {
            return element;
        }
        parent_[element] = FindSet(parent_[element]);
        return parent_[element];
    }

    void UnionSets(const uint64_t& first_elem, const uint64_t& second_elem) {
        uint64_t new_first = FindSet(first_elem);
        uint64_t new_second = FindSet(second_elem);
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

    void AddEdge(const Edge& edge) {
        uint64_t first_leader = FindSet(edge.begin);
        uint64_t second_leader = FindSet(edge.end);

        if (first_leader == second_leader) {
            leader_weight_[first_leader] += edge.weight;
        } else {
            UnionSets(edge.begin, edge.end);
            uint64_t new_leader = FindSet(edge.begin);

            uint64_t new_weight = edge.weight;
            new_weight += leader_weight_[first_leader];
            new_weight += leader_weight_[second_leader];
            leader_weight_[new_leader] = new_weight;
            leader_weight_[first_leader] = new_weight;
            leader_weight_[second_leader] = new_weight;
        }
    }

    uint64_t GetWeightComp(const uint64_t& vertex) {
        uint64_t leader = FindSet(vertex - 1);
        return leader_weight_[leader];
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    uint64_t quantity_vertex = 0;
    uint64_t quantity_requests = 0;
    std::cin >> quantity_vertex >> quantity_requests;

    DSU graph(quantity_vertex);

    for (uint64_t i = 0; i < quantity_requests; ++i) {
        char indicator = 0;
        std::cin >> indicator;
        if (indicator == '1') {
            uint64_t begin = 0;
            uint64_t end = 0;
            uint64_t weight = 0;
            std::cin >> begin >> end >> weight;
            Edge edge(begin, end, weight);
            graph.AddEdge(edge);
        }
        if (indicator == '2') {
            uint64_t vertex = 0;
            std::cin >> vertex;
            std::cout << graph.GetWeightComp(vertex) << '\n';
        }
    }

    return 0;
}
