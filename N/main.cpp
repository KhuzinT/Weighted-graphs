#include <iostream>
#include <vector>

class IGraph {
public:

    using WeightT = int32_t;

    const int32_t kINFINITY = 1000 * 1000 * 1000 * 1LL;

protected:
    uint32_t q_vertex_ = 0;

public:
    uint32_t GetQVertex() const {
        return q_vertex_;
    }

    virtual std::vector<std::vector<WeightT>> GetMatrix() const = 0;
};

class GraphMatrix final : public IGraph {
private:
    std::vector<std::vector<WeightT>> matrix_;

public:
    explicit GraphMatrix(const uint32_t& quantity) {
        std::vector<WeightT> tmp(quantity, kINFINITY);
        matrix_.resize(quantity, tmp);

        q_vertex_ = quantity;
    }

    void FillMatrix(const std::vector<std::vector<WeightT>>& matrix) {
        for (uint32_t iteration = 0; iteration < q_vertex_; ++iteration) {
            for (uint32_t position = 0; position < q_vertex_; ++position) {
                matrix_[iteration][position] = matrix[iteration][position];
            }
        }
    }

    std::vector<std::vector<WeightT>> GetMatrix() const override {
        return matrix_;
    }
};

std::vector<std::vector<IGraph::WeightT>> FloydWarshallAlgorithm(IGraph& graph) {
    auto matrix = graph.GetMatrix();
    for (uint32_t iteration = 0; iteration < graph.GetQVertex(); ++iteration) {
        for (uint32_t current_vertex = 0; current_vertex < graph.GetQVertex(); ++current_vertex) {
            for (uint32_t next_vertex = 0; next_vertex < graph.GetQVertex(); ++next_vertex) {
                matrix[current_vertex][next_vertex] =
                    std::min(matrix[current_vertex][next_vertex],
                             matrix[current_vertex][iteration] + matrix[iteration][next_vertex]);
            }
        }
    }

    return matrix;
}

void PrintMatrix(std::vector<std::vector<IGraph::WeightT>>& matrix) {
    for (uint32_t iteration = 0; iteration < matrix.size(); ++iteration) {
        for (uint32_t position = 0; position < matrix[0].size(); ++position) {
            std::cout << matrix[iteration][position] << ' ';
        }
        std::cout << std::endl;
    }
}

int main() {
    uint32_t q_vertex = 0;
    std::cin >> q_vertex;

    GraphMatrix graph(q_vertex);

    std::vector<std::vector<IGraph::WeightT>> matrix(q_vertex);

    for (uint32_t current_vertex = 0; current_vertex < q_vertex; ++current_vertex) {
        matrix[current_vertex].resize(q_vertex);
        for (uint32_t next_vertex = 0; next_vertex < q_vertex; ++next_vertex) {
            std::cin >> matrix[current_vertex][next_vertex];
        }
    }

    graph.FillMatrix(matrix);

    auto update_matrix = FloydWarshallAlgorithm(graph);
    PrintMatrix(update_matrix);

    return 0;
}