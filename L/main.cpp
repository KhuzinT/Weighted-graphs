#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <vector>
#include <set>

//********************************************************************************************

using Vertex = uint64_t;

using DistT = uint64_t;

using WeightT = uint64_t;

const uint64_t kInfinity = 101 * 19891099 * 1LL;

//********************************************************************************************

class IGraph {
protected:
    Vertex q_vertex = 0;

    bool is_oriented = false;

    virtual void Add(const Vertex& a, const Vertex& b, const WeightT& w) = 0;

    struct Vector {
        std::vector<Vertex> vertex;
        std::vector<WeightT> weight;
    };

public:
    [[nodiscard]] Vertex GetQVertex() const {
        return q_vertex;
    }

    [[nodiscard]] virtual Vector GetNeighbors(const Vertex& v) const = 0;

    void AddEdge(const Vertex& a, const Vertex& b, const WeightT& w) {
        Add(a - 1, b - 1, w);
    }
};

//********************************************************************************************

class GraphList final : public IGraph {
private:
    std::unordered_map<Vertex, Vector> list_;

    void Add(const Vertex& a, const Vertex& b, const WeightT& w) override {
        if (a == b) {
            return;
        }
        if (a >= q_vertex || b >= q_vertex) {
            return;
        }
        list_[a].vertex.push_back(b);
        list_[a].weight.push_back(w);
        if (!is_oriented) {
            list_[b].vertex.push_back(a);
            list_[b].weight.push_back(w);
        }
    }

public:
    explicit GraphList(const Vertex& quantity, bool oriented = false) {
        for (Vertex i = 0; i < quantity; ++i) {
            Vector tmp;
            list_[i] = tmp;
        }
        q_vertex = quantity;
        is_oriented = oriented;
    }

    [[nodiscard]] Vector GetNeighbors(const Vertex& v) const override {
        Vector answer;
        answer.vertex = list_.at(v).vertex;
        answer.weight = list_.at(v).weight;
        return answer;
    }
};

//********************************************************************************************

class GraphAlgorithm {

    std::vector<DistT> dist_;

    explicit GraphAlgorithm(const uint64_t& quantity) {
        dist_.resize(quantity, kInfinity);
    }

    struct Ends {
        Vertex vertex;
        WeightT weight;

        explicit Ends(const Vertex& v, const WeightT& w) {
            vertex = v;
            weight = w;
        }

        bool operator<(const Ends& other) const {
            if (weight < other.weight) {
                return true;
            }
            if (weight > other.weight) {
                return false;
            }
            return vertex < other.vertex;
        }
    };

    std::vector<DistT> DijkstraAlgorithm(IGraph& graph, const Vertex& start) {
        dist_[start] = 0;

        std::set<Ends> queue;
        queue.insert(Ends(start, dist_[start]));

        while (!queue.empty()) {
            Vertex current = queue.begin()->vertex;
            queue.erase(queue.begin());
            auto neighbors = graph.GetNeighbors(current);
            for (uint64_t i = 0; i < neighbors.vertex.size(); ++i) {
                Vertex u = neighbors.vertex[i];
                WeightT w = neighbors.weight[i];
                if (dist_[current] + w < dist_[u]) {
                    queue.erase(Ends(u, dist_[u]));
                    dist_[u] = dist_[current] + w;
                    queue.insert(Ends(u, dist_[u]));
                }
            }
        }

        return dist_;
    }

    friend std::vector<DistT> ShortPath(IGraph& g, const Vertex& start);
};

std::vector<DistT> ShortPath(IGraph& graph, const Vertex& start) {
    GraphAlgorithm g_alg(graph.GetQVertex());
    return g_alg.DijkstraAlgorithm(graph, start - 1);
}

//********************************************************************************************

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    uint64_t q_requests = 0;
    std::cin >> q_requests;

    for (uint64_t i = 0; i < q_requests; ++i) {
        uint64_t q_vertex = 0;
        uint64_t q_edge = 0;
        std::cin >> q_vertex >> q_edge;

        GraphList graph(q_vertex);

        for (uint64_t j = 0; j < q_edge; ++j) {
            Vertex begin = 0;
            Vertex end = 0;
            WeightT weight = 0;
            std::cin >> begin >> end >> weight;
            graph.AddEdge(begin + 1, end + 1, weight);
        }

        Vertex start = 0;
        std::cin >> start;

        auto answer = ShortPath(graph, start + 1);
        for (auto& element : answer) {
            if (element == kInfinity) {
                std::cout << "2009000999" << ' ';
            } else {
                std::cout << element << ' ';
            }
        }
        std::cout << std::endl;
    }
    return 0;
}

//********************************************************************************************
