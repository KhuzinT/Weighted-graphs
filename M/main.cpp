#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <vector>

//********************************************************************************************

using Vertex = uint64_t;

using DistT = uint64_t;

using WeightT = uint64_t;

const uint64_t kInfinity = 101 * 19891099 * 1LL;

//********************************************************************************************

//********************************************************************************************

class GraphMatrix {
private:
    std::vector<std::vector<WeightT>> matrix_;

    uint64_t q_vertex_ = 0;

    bool is_oriented_ = false;

    void Add(const Vertex& a, const Vertex& b, const WeightT& w) {
        matrix_[a][b] = w;
        if (!is_oriented_) {
            matrix_[b][a] = w;
        }
    }

public:
    explicit GraphMatrix(const Vertex& quantity, bool oriented = false) {
        std::vector<WeightT> tmp;
        tmp.resize(quantity, kInfinity);
        matrix_.resize(quantity, tmp);
        q_vertex_ = quantity;
        is_oriented_ = oriented;
    }

    [[nodiscard]] const std::vector<WeightT>& GetNeighbors(const Vertex& v) const {
        return matrix_[v];
    }

    [[nodiscard]] uint64_t GetQVertex() const {
        return q_vertex_;
    }

    void AddEdge(const Vertex& begin, const Vertex& end, const WeightT& weight) {
        Add(begin - 1, end - 1, weight);
    }
};

//********************************************************************************************

DistT DijkstraAlgorithm(GraphMatrix& graph, const Vertex& start, const Vertex& finish) {
    std::vector<DistT> dist(graph.GetQVertex(), kInfinity);
    std::vector<Vertex> parent(graph.GetQVertex(), kInfinity);
    std::vector<bool> visit(graph.GetQVertex(), false);

    dist[start] = 0;
    DistT min_dist = 0;
    Vertex min_vertex = start;

    while (min_dist < kInfinity) {
        visit[min_vertex] = true;

        auto neighbors = graph.GetNeighbors(min_vertex);
        for (Vertex i = 0; i < neighbors.size(); ++i) {
            if (neighbors[i] != kInfinity) {
                Vertex vertex = i;
                WeightT weight = neighbors[i];

                if (dist[min_vertex] + weight < dist[vertex]) {
                    dist[vertex] = dist[min_vertex] + weight;
                    parent[vertex] = min_vertex;
                }
            }
        }

        min_dist = kInfinity;
        for (Vertex v = 0; v < graph.GetQVertex(); ++v) {
            if (!visit[v]) {
                if (dist[v] < min_dist) {
                    min_dist = dist[v];
                    min_vertex = v;
                }
            }
        }
    }

    return dist[finish];
}

DistT ShortPath(GraphMatrix& graph, const Vertex& start, const Vertex& finish) {
    return DijkstraAlgorithm(graph, start - 1, finish - 1);
}

//********************************************************************************************

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    uint64_t q_vertex = 0;
    std::cin >> q_vertex;

    GraphMatrix g(q_vertex, true);

    Vertex start = 0;
    Vertex finish = 0;
    std::cin >> start >> finish;

    for (uint64_t i = 1; i <= q_vertex; ++i) {
        for (uint64_t j = 1; j <= q_vertex; ++j) {
            int64_t weight = 0;
            std::cin >> weight;

            if (weight != -1) {
                g.AddEdge(i, j, static_cast<WeightT>(weight));
            }
        }
    }

    auto answer = ShortPath(g, start, finish);
    if (answer == kInfinity) {
        std::cout << -1 << std::endl;
    } else {
        std::cout << answer << std::endl;
    }
    return 0;
}

//********************************************************************************************
