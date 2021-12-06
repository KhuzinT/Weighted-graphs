#include <iostream>
#include <vector>

using WeightT = int32_t;

const int32_t kInfinity = 1000 * 1000 * 1000 * 1LL;

class IGraph {
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
        std::vector<WeightT> tmp(quantity, kInfinity);
        matrix_.resize(quantity, tmp);

        q_vertex_ = quantity;
    }

    void FillMatrix(const std::vector<std::vector<WeightT>>& matrix) {
        for (uint32_t i = 0; i < q_vertex_; ++i) {
            for (uint32_t j = 0; j < q_vertex_; ++j) {
                matrix_[i][j] = matrix[i][j];
            }
        }
    }

    std::vector<std::vector<WeightT>> GetMatrix() const override {
        return matrix_;
    }
};

std::vector<std::vector<WeightT>> FloydWarshallAlgorithm(IGraph& graph) {
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

void PrintMatrix(std::vector<std::vector<WeightT>>& matrix) {
    for (uint32_t i = 0; i < matrix.size(); ++i) {
        for (uint32_t j = 0; j < matrix[0].size(); ++j) {
            std::cout << matrix[i][j] << ' ';
        }
        std::cout << std::endl;
    }
}

int main() {
    uint32_t q_vertex = 0;
    std::cin >> q_vertex;

    GraphMatrix graph(q_vertex);

    std::vector<std::vector<WeightT>> matrix(q_vertex);

    for (uint32_t i = 0; i < q_vertex; ++i) {
        matrix[i].resize(q_vertex);
        for (uint32_t j = 0; j < q_vertex; ++j) {
            std::cin >> matrix[i][j];
        }
    }

    graph.FillMatrix(matrix);

    auto answer = FloydWarshallAlgorithm(graph);
    PrintMatrix(answer);

    return 0;
}