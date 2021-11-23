#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>

//********************************************************************************************

using Vertex = int64_t;

using WeightT = int64_t;

const int64_t kPoison = 101 * 19891099 * 1LL;

//********************************************************************************************

struct Neighbor {
    Vertex vertex;
    WeightT weight;

    explicit Neighbor(const Vertex& v, const WeightT& w) {
        vertex = v;
        weight = w;
    }

    bool operator<(const Neighbor& other) const {
        if (weight < other.weight) {
            return true;
        }
        if (weight > other.weight) {
            return false;
        }
        return vertex < other.vertex;
    }
};

//********************************************************************************************

class GraphList {
private:
    std::vector<std::vector<Neighbor>> list_;

    int64_t q_vertex_ = 0;

    bool is_oriented_ = false;

    void Add(const Vertex& a, const Vertex& b, const WeightT& w) {
        list_[a].emplace_back(b, w);
        if (!is_oriented_) {
            list_[b].emplace_back(a, w);
        }
    }

public:
    explicit GraphList(const Vertex& quantity, bool oriented = false) {
        for (Vertex i = 0; i < quantity; ++i) {
            std::vector<Neighbor> tmp;
            list_.push_back(tmp);
        }
        q_vertex_ = quantity;
        is_oriented_ = oriented;
    }

    [[nodiscard]] std::vector<Neighbor> GetNeighbors(const Vertex& v) const {
        return list_[v];
    }

    [[nodiscard]] int64_t GetQVertex() const {
        return q_vertex_;
    }

    void AddEdge(const Vertex& begin, const Vertex& end, const WeightT& weight) {
        Add(begin - 1, end - 1, weight);
    }
};

class GraphAlgorithm {
    std::vector<bool> visit_;
    std::vector<WeightT> dist_;
    std::vector<Vertex> parent_;

    explicit GraphAlgorithm(const uint64_t& quantity) {
        visit_.resize(quantity, false);
        dist_.resize(quantity, kPoison);
        parent_.resize(quantity, kPoison);
    };

    std::vector<Vertex> FordBellmanAlgorithm(GraphList& g, const Vertex& start) {
        dist_.clear();
        parent_.clear();
        dist_.resize(g.GetQVertex(), kPoison);
        parent_.resize(g.GetQVertex(), kPoison);

        dist_[start] = 0;
        visit_[start] = true;

        for (int64_t t = 0; t < g.GetQVertex(); ++t) {
            for (int64_t v = 0; v < g.GetQVertex(); ++v) {
                auto neighbors = g.GetNeighbors(v);
                for (auto& neighbor : neighbors) {
                    Vertex u = neighbor.vertex;
                    WeightT w = neighbor.weight;
                    if (dist_[v] != kPoison) {
                        if (dist_[u] > dist_[v] + w) {
                            dist_[u] = dist_[v] + w;
                            parent_[u] = v;
                            visit_[v] = true;
                        }
                    }
                }
            }
        }

        Vertex current_vertex = kPoison;
        for (int64_t v = 0; v < g.GetQVertex(); ++v) {
            auto neighbors = g.GetNeighbors(v);
            for (auto& neighbor : neighbors) {
                Vertex u = neighbor.vertex;
                WeightT w = neighbor.weight;
                if (dist_[v] != kPoison) {
                    if (dist_[u] > dist_[v] + w) {
                        dist_[u] = dist_[v] + w;
                        parent_[u] = v;
                        current_vertex = u;
                    }
                }
            }
        }
        if (current_vertex == kPoison) {
            return {};
        }

        std::vector<Vertex> answer;
        for (int i = 0; i < g.GetQVertex(); i++) {
            current_vertex = parent_[current_vertex];
        }
        answer.push_back(current_vertex + 1);
        for (auto v = parent_[current_vertex]; v != current_vertex; v = parent_[v]) {
            answer.push_back(v + 1);
        }
        answer.push_back(current_vertex + 1);
        std::reverse(answer.begin(), answer.end());
        return answer;
    }

    friend std::vector<Vertex> HasNegativeCycle(GraphList& g);
};

std::vector<Vertex> HasNegativeCycle(GraphList& g) {
    GraphAlgorithm g_alg(g.GetQVertex());

    std::vector<Vertex> answer;
    for (int64_t i = 0; i < g.GetQVertex(); ++i) {
        if (!g_alg.visit_[i]) {
            answer = g_alg.FordBellmanAlgorithm(g, i);
            if (!answer.empty()) {
                return answer;
            }
        }
    }
    return answer;
}

int main() {
    int64_t q_vertex = 0;
    std::cin >> q_vertex;

    GraphList g(q_vertex, true);

    for (int64_t i = 0; i < q_vertex; ++i) {
        for (int64_t j = 0; j < q_vertex; ++j) {
            WeightT weight = 0;
            std::cin >> weight;

            if (weight != 100000) {
                g.AddEdge(i + 1, j + 1, weight);
            }
        }
    }

    auto answer = HasNegativeCycle(g);
    if (answer.empty()) {
        std::cout << "NO" << std::endl;
    } else {
        std::cout << "YES" << std::endl;
        std::cout << answer.size() << std::endl;
        for (auto& elem : answer) {
            std::cout << elem << ' ';
        }
        std::cout << std::endl;
    }

    return 0;
}