#include <iostream>

#include "matrix.hpp"

int main() {
   
    Matrix<int, 0> matrix;

    // Главная диагональ
    for (int i = 0; i <= 9; ++i) {
        matrix[i][i] = i;
    }

    // Второстепенная диагональ
    for (int i = 0; i <= 9; ++i) {
        matrix[i][9 - i] = 9 - i;
    }

    std::cout << "Matrix from [1,1] to [8,8]:" << std::endl;
    for (int i = 1; i <= 8; ++i) {
        for (int j = 1; j <= 8; ++j) {
            std::cout << matrix(i, j);
            if (j < 8) std::cout << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Number of occupied cells: " << matrix.size() << std::endl;
    std::cout << std::endl;

    std::cout << "All occupied cells:" << std::endl;
    for (const auto& [coords, value] : matrix) {
        std::cout << "  [" << coords.first << "," << coords.second << "] = " << value << std::endl;
    }
    std::cout << std::endl;

    return 0;
}