#include <iostream>
#include <vector>
#include <map>

using WeightT = int64_t;

using Vertex = int64_t;

const int64_t kPoison = 1000 * 1000 * 1000 * 1LL;

struct Edge {
    Vertex begin;
    Vertex end;

    explicit Edge(const Vertex& a, const Vertex& b) {
        begin = a;
        end = b;
    }

    bool operator<(const Edge& other) const {
        if (begin < other.begin) {
            return true;
        }
        if (begin > other.begin) {
            return false;
        }
        return end < other.end;
    }
};

struct EdgeInfo {
    WeightT weight;
    int64_t order;

    EdgeInfo() {
        weight = 0;
        order = 0;
    }

    explicit EdgeInfo(const WeightT& w, const int64_t& o) {
        weight = w;
        order = o;
    }

    bool operator<(const EdgeInfo& other) const {
        if (weight < other.weight) {
            return true;
        }
        if (weight > other.weight) {
            return false;
        }
        return order < other.order;
    }
};

class GraphMatrix {
private:
    std::vector<std::vector<WeightT>> matrix_;

    int64_t q_vertex_ = 0;

    void Add(const Vertex& begin, const Vertex& end, const WeightT& weight) {
        if (matrix_[begin][end] == kPoison) {
            matrix_[begin][end] = -weight;
            return;
        }
        if (-weight < matrix_[begin][end]) {
            matrix_[begin][end] = -weight;
        }
    }

public:
    explicit GraphMatrix(const int64_t& quantity) {
        std::vector<WeightT> tmp(quantity, kPoison);
        matrix_.resize(quantity, tmp);

        for (int64_t i = 0; i < quantity; ++i) {
            matrix_[i][i] = 0;
        }
        q_vertex_ = quantity;
    }

    void AddEdge(const Vertex& begin, const Vertex& end, const WeightT& weight) {
        Add(begin - 1, end - 1, weight);
    }

    [[nodiscard]] std::vector<std::vector<WeightT>>& GetMatrix() {
        return matrix_;
    }

    [[nodiscard]] int64_t GetQVertex() const {
        return q_vertex_;
    }
};

std::vector<Vertex> FloydWarshallAlgorithm(GraphMatrix& g, std::vector<Vertex>& path,
                                           std::map<Edge, EdgeInfo>& edge_list, bool& has_cycle) {
    auto matrix = g.GetMatrix();
    std::vector<std::vector<Vertex>> parent(g.GetQVertex(), std::vector<Vertex>(g.GetQVertex(), kPoison));
    for (auto edge : edge_list) {
        Vertex begin = edge.first.begin - 1;
        Vertex end = edge.first.end - 1;

        parent[begin][end] = end;
    }

    for (int64_t t = 0; t < g.GetQVertex(); ++t) {
        for (int64_t i = 0; i < g.GetQVertex(); ++i) {
            for (int64_t j = 0; j < g.GetQVertex(); ++j) {
                if (matrix[i][j] > matrix[i][t] + matrix[t][j]) {
                    matrix[i][j] = matrix[i][t] + matrix[t][j];
                    parent[i][j] = parent[i][t];
                }
            }
        }
    }

    std::vector<Vertex> shortest_path;
    shortest_path.push_back(path[0] - 1);
    for (uint64_t i = 0; i < path.size() - 1; ++i) {
        Vertex begin = path[i] - 1;
        Vertex end = path[i + 1] - 1;

        if (parent[begin][end] != kPoison) {
            while (begin != end) {
                if (matrix[begin][begin] < 0) {
                    has_cycle = true;
                    return {};
                }
                begin = parent[begin][end];
                shortest_path.push_back(begin);
            }
        }
    }
    std::vector<int64_t> answer;
    for (uint64_t i = 0; i < shortest_path.size() - 1; ++i) {
        Vertex first = shortest_path[i];
        Vertex second = shortest_path[i + 1];
        Edge edge(first + 1, second + 1);
        answer.push_back(edge_list[edge].order);
    }

    return answer;
}
int main() {
    int64_t q_vertex = 0;
    int64_t q_edge = 0;
    std::cin >> q_vertex >> q_edge;

    GraphMatrix g(q_vertex);

    int64_t path_size = 0;
    std::cin >> path_size;

    std::map<Edge, EdgeInfo> edge_list;

    for (int64_t order = 1; order <= q_edge; ++order) {
        Vertex begin = 0;
        Vertex end = 0;
        WeightT weight = 0;
        std::cin >> begin >> end >> weight;
        g.AddEdge(begin, end, weight);
        Edge tmp(begin, end);
        auto it = edge_list.find(tmp);
        if (it != edge_list.cend()) {
            if (weight > edge_list[tmp].weight) {
                EdgeInfo tmp_info(weight, order);
                edge_list[tmp] = tmp_info;
            }
        } else {
            EdgeInfo tmp_info(weight, order);
            edge_list[tmp] = tmp_info;
        }
    }

    std::vector<Vertex> path(path_size);
    for (int64_t i = 0; i < path_size; ++i) {
        std::cin >> path[i];
    }
    bool has_cycle = false;
    auto answer = FloydWarshallAlgorithm(g, path, edge_list, has_cycle);
    if (has_cycle) {
        std::cout << "infinitely kind" << std::endl;
    } else {
        std::cout << answer.size() << std::endl;
        for (auto& elem : answer) {
            std::cout << elem << ' ';
        }
        std::cout << std::endl;
    }
    return 0;
}
