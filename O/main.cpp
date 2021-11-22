#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <set>

//********************************************************************************************

using Vertex = uint64_t;

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
    explicit GraphList(const int64_t& quantity, bool oriented = false) {
        for (int64_t i = 0; i < quantity; ++i) {
            std::vector<Neighbor> tmp;
            list_.push_back(tmp);
        }
        q_vertex_ = quantity;
        is_oriented_ = oriented;
    }

    [[nodiscard]] std::vector<Neighbor>& GetNeighbors(const Vertex& v) {
        return list_[v];
    }

    [[nodiscard]] int64_t GetQVertex() const {
        return q_vertex_;
    }

    void AddEdge(const Vertex& begin, const Vertex& end, const WeightT& weight) {
        Add(begin - 1, end - 1, weight);
    }
};

std::vector<WeightT> BellmanFordAlgorithm(GraphList& g, const Vertex& start) {
    std::vector<WeightT> dist(g.GetQVertex(), kPoison);
    dist[start] = 0;
    Vertex t = 0;
    for (int64_t i = 0; i < g.GetQVertex() - 1; ++i) {
        for (int64_t j = 0; j < g.GetQVertex(); ++j) {
            auto neighbors = g.GetNeighbors(j);
            for (auto& neighbor : neighbors) {
                if (dist[t] != kPoison) {
                    if (dist[neighbor.vertex] > dist[t] + neighbor.weight) {
                        dist[neighbor.vertex] = dist[t] + neighbor.weight;
                    }
                }
            }
            ++t;
        }
        t = 0;
    }
    return dist;
}

std::vector<WeightT> DijkstraAlgorithm(GraphList& g, const Vertex& start) {
    std::vector<WeightT> dist(g.GetQVertex(), kPoison);
    dist[start] = 0;

    std::set<Neighbor> queue;
    queue.insert(Neighbor(start, dist[start]));

    while (!queue.empty()) {
        Vertex v = queue.begin()->vertex;
        queue.erase(queue.begin());
        auto neighbors = g.GetNeighbors(v);
        for (uint64_t i = 0; i < neighbors.size(); ++i) {
            Vertex u = neighbors[i].vertex;
            WeightT w = neighbors[i].weight;
            if (dist[v] + w < dist[u]) {
                queue.erase(Neighbor(u, dist[u]));
                dist[u] = dist[v] + w;
                queue.insert(Neighbor(u, dist[u]));
            }
        }
    }

    return dist;
}

int64_t JohnsonAlgorithm(GraphList g) {
    int64_t answer = 0;

    Vertex fiction = g.GetQVertex() - 1;
    auto dist = BellmanFordAlgorithm(g, fiction);
    for (uint64_t v = 0; v < fiction; ++v) {
        for (auto& neighbor : g.GetNeighbors(v)) {
            int64_t difference = dist[v] - dist[neighbor.vertex];
            neighbor.weight += difference;
        }
    }

    Vertex current_vertex = 0;
    for (uint64_t i = 0; i < fiction; ++i) {
        auto path = DijkstraAlgorithm(g, current_vertex);
        for (Vertex v = 0; v < path.size() - 1; ++v) {
            if (path[v] != kPoison) {
                path[v] -= dist[current_vertex];
                path[v] += dist[v];
            }
        }
        for (Vertex v = 0; v < path.size() - 1; ++v) {
            if (i == v) {
                continue;
            }
            if (path[v] != kPoison) {
                if (path[v] > answer) {
                    answer = path[v];
                }
            }
        }
        ++current_vertex;
    }

    return answer;
}

int main() {
    int64_t q_vertex = 0;
    int64_t q_edge = 0;
    std::cin >> q_vertex >> q_edge;

    GraphList g(q_vertex + 1, true);

    for (int64_t i = 0; i < q_edge; ++i) {
        Vertex begin = 0;
        Vertex end = 0;
        WeightT weight = 0;
        std::cin >> begin >> end >> weight;
        g.AddEdge(begin + 1, end + 1, weight);
    }

    std::cout << JohnsonAlgorithm(g) << std::endl;
    return 0;
}
