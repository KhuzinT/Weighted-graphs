#include <iostream>
#include <string>
#include <vector>

//********************************************************************************************

using Vertex = int64_t;

using WeightT = int64_t;

const int64_t kPoison = 1000 * 1000 * 1000 * 1LL;

//********************************************************************************************

struct Flow {
    WeightT flow = 0;
    WeightT capacity = 0;

    explicit Flow(const WeightT& f, const WeightT& c) {
        flow = f;
        capacity = c;
    }
};

class GraphMatrix {
private:
    std::vector<std::vector<Flow>> matrix_;

    int64_t q_vertex_ = 0;

    void Add(const Vertex& a, const Vertex& b, const WeightT& w) {
        matrix_[a][b].capacity = w;
        matrix_[a][b].flow = 0;
    }

public:
    explicit GraphMatrix(const Vertex& quantity) {
        std::vector<Flow> tmp(quantity, Flow(0, 0));
        matrix_.resize(quantity, tmp);
        q_vertex_ = quantity;
    }

    [[nodiscard]] int64_t GetQVertex() const {
        return q_vertex_;
    }

    void AddEdge(const Vertex& begin, const Vertex& end, const WeightT& weight) {
        Add(begin - 1, end - 1, weight);
    }

    [[nodiscard]] std::vector<Flow> GetNeighbors(const Vertex& v) const {
        return matrix_[v];
    }

    [[nodiscard]] WeightT GetDifference(const Vertex& begin, const Vertex& end) const {
        return matrix_[begin][end].capacity - matrix_[begin][end].flow;
    }

    friend class GraphAlgorithm;
};

class GraphAlgorithm {
    std::vector<WeightT> dist_;
    std::vector<WeightT> ptr_;

    explicit GraphAlgorithm(const int64_t& quantity) {
        dist_.resize(quantity, kPoison);
        ptr_.resize(quantity, 0);
    }

    bool BFS(GraphMatrix& g, const Vertex& start, const Vertex& finish) {
        int64_t qh = 0;
        int64_t qt = 0;
        std::vector<Vertex> queue(g.GetQVertex());

        dist_[start] = 0;
        ++qt;
        while (qh < qt) {
            Vertex v = queue[qh];
            qh++;
            auto neighbors = g.GetNeighbors(v);
            for (int64_t i = 0; i < g.GetQVertex(); ++i) {
                Flow f = neighbors[i];
                if (dist_[i] == kPoison && f.flow < f.capacity) {
                    queue[qt++] = i;
                    dist_[i] = dist_[v] + 1;
                }
            }
        }

        return dist_[finish] != kPoison;
    }

    WeightT DFS(GraphMatrix& g, const Vertex& start, const Vertex& finish, const WeightT& flow) {
        if (flow == 0) {
            return 0;
        }
        if (start == finish) {
            return flow;
        }
        for (int64_t& u = ptr_[start]; u < g.GetQVertex(); ++u) {
            if (dist_[u] != dist_[start] + 1) {
                continue;
            }
            WeightT new_flow = std::min(flow, g.GetDifference(start, u));
            WeightT push = DFS(g, u, finish, new_flow);
            if (push) {
                g.matrix_[start][u].flow += push;
                g.matrix_[u][start].flow -= push;
                return push;
            }
        }

        return 0;
    }

    friend WeightT DinicAlgorithm(GraphMatrix& g);
};

WeightT DinicAlgorithm(GraphMatrix& g) {
    WeightT answer = 0;

    GraphAlgorithm g_alg(g.GetQVertex());

    while (true) {
        if (!g_alg.BFS(g, 0, g.GetQVertex() - 1)) {
            break;
        }

        while (WeightT flow = g_alg.DFS(g, 0, g.GetQVertex() - 1, kPoison)) {
            answer += flow;
        }

        g_alg.dist_.clear();
        g_alg.dist_.resize(g.GetQVertex(), kPoison);

        g_alg.ptr_.clear();
        g_alg.ptr_.resize(g.GetQVertex(), 0);
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

    auto answer = DinicAlgorithm(g);
    std::cout << answer << std::endl;

    return 0;
}
