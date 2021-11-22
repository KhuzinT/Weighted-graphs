#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <limits>

//********************************************************************************************

using Vertex = uint64_t;

using DistT = uint64_t;

using WeightT = uint64_t;

const uint64_t kPoison = UINT64_MAX;

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

    uint64_t q_vertex_ = 0;

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

    [[nodiscard]] const std::vector<Neighbor>& GetNeighbors(const Vertex& v) const {
        return list_[v];
    }

    [[nodiscard]] uint64_t GetQVertex() const {
        return q_vertex_;
    }

    void AddEdge(const Vertex& begin, const Vertex& end, const WeightT& weight) {
        Add(begin - 1, end - 1, weight);
    }
};

//********************************************************************************************

class GraphAlgorithm {

    std::vector<DistT> dist_;

    explicit GraphAlgorithm(const uint64_t& quantity) {
        dist_.resize(quantity, kPoison);
    }

    DistT DijkstraAlgorithm(GraphList& g, std::vector<Vertex>& array, const Vertex& finish) {
        for (uint64_t i = 0; i < array.size(); ++i) {
            dist_[array[i]] = 0;
        }

        std::set<Neighbor> queue;
        for (uint64_t i = 0; i < array.size(); ++i) {
            queue.insert(Neighbor(array[i], dist_[array[i]]));
        }

        while (!queue.empty()) {
            Vertex v = queue.begin()->vertex;
            queue.erase(queue.begin());
            auto neighbors = g.GetNeighbors(v);
            for (uint64_t i = 0; i < neighbors.size(); ++i) {
                Vertex u = neighbors[i].vertex;
                WeightT w = neighbors[i].weight;
                if (dist_[v] + w < dist_[u]) {
                    queue.erase(Neighbor(u, dist_[u]));
                    dist_[u] = dist_[v] + w;
                    queue.insert(Neighbor(u, dist_[u]));
                }
            }
        }

        return dist_[finish];
    }

    friend DistT ShortPath(GraphList& g, const Vertex& start, const Vertex& finish, std::vector<Vertex>& array);
};

DistT ShortPath(GraphList& g, const Vertex& start, const Vertex& finish, std::vector<Vertex>& array) {
    GraphAlgorithm g_alg(g.GetQVertex());

    auto virus_dist = g_alg.DijkstraAlgorithm(g, array, finish - 1);
    g_alg.dist_.clear();
    g_alg.dist_.resize(g.GetQVertex(), kPoison);
    std::vector<Vertex> tmp;
    tmp.push_back(start - 1);
    auto people_dist = g_alg.DijkstraAlgorithm(g, tmp, finish - 1);

    if (people_dist >= virus_dist || people_dist == kPoison) {
        return kPoison;
    }

    return people_dist;
}

int main() {
    uint64_t q_vertex = 0;
    uint64_t q_edge = 0;
    std::cin >> q_vertex >> q_edge;

    uint64_t q_country = 0;
    std::cin >> q_country;

    std::vector<Vertex> array;
    for (uint64_t i = 0; i < q_country; ++i) {
        Vertex country = 0;
        std::cin >> country;
        array.push_back(country - 1);
    }

    GraphList g(q_vertex);

    for (uint64_t i = 0; i < q_edge; ++i) {
        Vertex begin = 0;
        Vertex end = 0;
        WeightT weight = 0;
        std::cin >> begin >> end >> weight;
        g.AddEdge(begin, end, weight);
    }

    Vertex start = 0;
    Vertex finish = 0;
    std::cin >> start >> finish;

    auto answer = ShortPath(g, start, finish, array);
    if (answer == kPoison) {
        std::cout << -1 << std::endl;
    } else {
        std::cout << answer << std::endl;
    }

    return 0;
}
