#include <iostream>
#include <vector>

const uint32_t kInfinity = 100 * 100 * 100;

template <typename Type>
const Type kZero = 0;

template <typename Type>
class DSU {
private:
    std::vector<uint32_t> parent_;
    std::vector<uint32_t> rank_;

    std::vector<Type> leader_value_;

public:
    explicit DSU(const uint32_t& quantity) {
        parent_.resize(quantity, kInfinity);
        for (uint32_t i = 0; i < quantity; ++i) {
            parent_[i] = i;
        }
        rank_.resize(quantity, 0);
        leader_value_.resize(quantity, kZero<Type>);
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

    void AddConnection(const uint32_t& first_elem, const uint32_t& second_elem, const Type& value) {
        uint32_t first_leader = FindSet(first_elem);
        uint32_t second_leader = FindSet(second_elem);

        if (first_leader == second_leader) {
            leader_value_[first_leader] += value;
        } else {
            UnionSets(first_elem, second_elem);
            uint32_t new_leader = FindSet(first_elem);

            Type new_weight = value;
            new_weight += leader_value_[first_leader];
            new_weight += leader_value_[second_leader];
            leader_value_[new_leader] = new_weight;
        }
    }

    Type GetValueComp(const uint32_t& elem) {
        uint32_t leader = FindSet(elem);
        return leader_value_[leader];
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    uint32_t quantity_vertex = 0;
    uint32_t quantity_requests = 0;
    std::cin >> quantity_vertex >> quantity_requests;

    DSU<uint32_t> graph(quantity_vertex);

    for (uint32_t i = 0; i < quantity_requests; ++i) {
        char indicator = 0;
        std::cin >> indicator;
        if (indicator == '1') {
            uint32_t begin = 0;
            uint32_t end = 0;
            uint32_t weight = 0;
            std::cin >> begin >> end >> weight;
            graph.AddConnection(begin - 1, end - 1, weight);
        }
        if (indicator == '2') {
            uint32_t vertex = 0;
            std::cin >> vertex;
            std::cout << graph.GetValueComp(vertex - 1) << '\n';
        }
    }

    return 0;
}
