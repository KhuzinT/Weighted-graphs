#include <iostream>
#include <vector>

const uint64_t kPoison = UINT64_MAX;

struct Edge {
    uint64_t begin = 0;
    uint64_t end = 0;

    explicit Edge(const uint64_t& a, const uint64_t& b) {
        begin = a;
        end = b;
    }
};

class DSU {
private:
    std::vector<uint64_t> parent_;
    std::vector<uint64_t> rank_;

    uint64_t size_ = 0;

public:
    explicit DSU(const uint64_t& quantity) {
        parent_.resize(quantity, kPoison);
        for (uint64_t i = 0; i < quantity; ++i) {
            parent_[i] = i;
        }
        rank_.resize(quantity, 0);
        size_ = quantity;
    }

    [[nodiscard]] uint64_t GetSize() const {
        return size_;
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
};

uint64_t MinUnity(DSU& system, std::vector<Edge>& edge_list) {
    uint64_t answer = 0;
    for (auto& edge : edge_list) {
        ++answer;
        system.UnionSets(edge.begin, edge.end);
        bool is_in_same_set = true;
        for (uint64_t vertex = 0; vertex < system.GetSize() - 1; ++vertex) {
            uint64_t first_leader = system.FindSet(vertex);
            uint64_t second_leader = system.FindSet(vertex + 1);
            if (first_leader != second_leader) {
                is_in_same_set = false;
                break;
            }
        }
        if (is_in_same_set) {
            return answer;
        }
    }
    return answer;
}

int main() {

    uint64_t quantity_vertex = 0;
    uint64_t quantity_edge = 0;
    std::cin >> quantity_vertex >> quantity_edge;

    DSU system(quantity_vertex);

    std::vector<Edge> edge_list;

    for (uint64_t i = 0; i < quantity_edge; ++i) {
        uint64_t first_vertex = 0;
        uint64_t second_vertex = 0;
        std::cin >> first_vertex >> second_vertex;
        edge_list.emplace_back(first_vertex, second_vertex);
    }

    uint64_t answer = MinUnity(system, edge_list);

    std::cout << answer << std::endl;
    return 0;
}
