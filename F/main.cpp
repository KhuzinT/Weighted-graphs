#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <queue>

//********************************************************************************************

using Vertex = int64_t;

using WeightT = int64_t;

const int64_t kPoison = 1000 * 1000 * 1000 * 1LL;

//********************************************************************************************

class GraphMatrix {
private:
    std::vector<std::vector<Vertex>> matrix_;

    int64_t q_vertex_ = 0;

    void Add(const Vertex& a, const Vertex& b, const WeightT& w) {
        matrix_[a][b] = w;
    }

public:
    explicit GraphMatrix(const Vertex& quantity) {
        std::vector<Vertex> tmp(quantity, 0);
        matrix_.resize(quantity, tmp);
        q_vertex_ = quantity;
    }

    [[nodiscard]] int64_t GetQVertex() const {
        return q_vertex_;
    }

    void AddEdge(const Vertex& begin, const Vertex& end, const WeightT& weight) {
        Add(begin - 1, end - 1, weight);
    }

    [[nodiscard]] std::vector<Vertex> GetNeighbors(const Vertex& v) const {
        return matrix_[v];
    }

    friend int64_t EdmondsKarpAlgorithm(GraphMatrix& g);
};

//********************************************************************************************

class GraphAlgorithm {

    std::vector<bool> visit_;
    std::vector<Vertex> parent_;

    explicit GraphAlgorithm(const int64_t& quantity) {
        visit_.resize(quantity, false);
        parent_.resize(quantity, kPoison);
    }

    bool BFS(GraphMatrix& g, const Vertex& start, const Vertex& finish) {
        std::queue<Vertex> queue;
        queue.push(start);

        visit_[start] = true;
        parent_[start] = kPoison;

        while (!queue.empty()) {
            Vertex v = queue.front();
            queue.pop();

            auto neighbors = g.GetNeighbors(v);
            for (Vertex u = 0; u < g.GetQVertex(); ++u) {
                if (!visit_[u] && neighbors[u] > 0) {
                    parent_[u] = v;
                    if (u == finish) {
                        return true;
                    }
                    queue.push(u);
                    parent_[u] = v;
                    visit_[u] = true;
                }
            }
        }

        return false;
    }

    friend int64_t EdmondsKarpAlgorithm(GraphMatrix& g);
};

int64_t EdmondsKarpAlgorithm(GraphMatrix& g) {
    GraphAlgorithm g_alg(g.GetQVertex());

    Vertex start = 0;
    Vertex finish = g.GetQVertex() - 1;

    int64_t answer = 0;

    while (g_alg.BFS(g, start, finish)) {
        WeightT flow = kPoison;

        for (Vertex v = finish; v != start; v = g_alg.parent_[v]) {
            Vertex u = g_alg.parent_[v];
            flow = std::min(flow, g.matrix_[u][v]);
        }

        for (Vertex v = finish; v != start; v = g_alg.parent_[v]) {
            Vertex u = g_alg.parent_[v];

            g.matrix_[u][v] -= flow;
            g.matrix_[v][u] += flow;
        }

        answer += flow;

        g_alg.parent_.clear();
        g_alg.visit_.clear();

        g_alg.parent_.resize(g.GetQVertex(), kPoison);
        g_alg.visit_.resize(g.GetQVertex(), false);
    }

    return answer;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int64_t q_vertex = 0;
    int64_t q_edge = 0;
    std::cin >> q_vertex >> q_edge;

    GraphMatrix g(q_vertex);

    for (int64_t i = 0; i < q_edge; ++i) {
        Vertex begin = 0;
        Vertex end = 0;
        WeightT weight = 0;
        std::cin >> begin >> end >> weight;
        g.AddEdge(begin, end, weight);
    }

    auto answer = EdmondsKarpAlgorithm(g);
    std::cout << answer << std::endl;

    return 0;
}

//********************************************************************************************
