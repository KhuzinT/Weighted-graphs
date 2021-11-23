#include <iostream>
#include <string>
#include <vector>

//********************************************************************************************

using Vertex = int64_t;

using WeightT = int64_t;

const int64_t kPoison = INT64_MAX;

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

//********************************************************************************************

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

bool IsValidMolecule(std::vector<std::vector<char>>& molecule, const int64_t& n, const int64_t& m) {
    GraphMatrix g(n * m + 2);

    Vertex start = 0;
    Vertex finish = n * m + 1;

    int64_t start_flow = 0;
    int64_t finish_flow = 0;

    for (int64_t i = 0; i < n; ++i) {
        for (int64_t j = 0; j < m; ++j) {
            WeightT w = 0;
            if (molecule[i][j] == 'H') {
                w = 1;
            } else if (molecule[i][j] == 'O') {
                w = 2;
            } else if (molecule[i][j] == 'N') {
                w = 3;
            } else if (molecule[i][j] == 'C') {
                w = 4;
            } else {
                continue;
            }

            Vertex v = i * m + j + 1;

            if ((i % 2 + j % 2) % 2 == 0) {
                g.AddEdge(start + 1, v + 1, w);
                start_flow += w;
                if (i + 1 < n) {
                    Vertex u = (i + 1) * m + j + 1;
                    g.AddEdge(v + 1, u + 1, 1);
                }
                if (j + 1 < m) {
                    Vertex u = i * m + j + 2;
                    g.AddEdge(v + 1, u + 1, 1);
                }
                if (i - 1 >= 0) {
                    Vertex u = (i - 1) * m + j + 1;
                    g.AddEdge(v + 1, u + 1, 1);
                }
                if (j - 1 >= 0) {
                    Vertex u = i * m + j;
                    g.AddEdge(v + 1, u + 1, 1);
                }
            } else {
                g.AddEdge(v + 1, finish + 1, w);
                finish_flow += w;
            }
        }
    }

    auto flow = DinicAlgorithm(g);
    if (flow == 0) {
        return false;
    }

    return (start_flow == flow && finish_flow == flow);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int64_t n = 0;
    int64_t m = 0;
    std::cin >> n >> m;

    std::vector<std::vector<char>> molecule;

    for (int64_t i = 0; i < n; ++i) {
        std::vector<char> tmp;
        for (int64_t j = 0; j < m; ++j) {
            char element = 0;
            std::cin >> element;
            tmp.push_back(element);
        }
        molecule.push_back(tmp);
    }

    bool answer = IsValidMolecule(molecule, n, m);
    if (answer) {
        std::cout << "Valid" << std::endl;
    } else {
        std::cout << "Invalid" << std::endl;
    }
    return 0;
}

//********************************************************************************************
