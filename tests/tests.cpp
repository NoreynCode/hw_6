#define BOOST_TEST_MODULE MatrixTests
#include <boost/test/included/unit_test.hpp>
#include <set>
#include "matrix.hpp"

// Базовый функционал
BOOST_AUTO_TEST_SUITE(BasicFunctionality)

// Пустаа матрица
BOOST_AUTO_TEST_CASE(NewMatrixIsEmpty) {
    Matrix<int, 0> matrix;
    BOOST_CHECK_EQUAL(matrix.size(), 0);
}

// Чтение из свободной ячейки
BOOST_AUTO_TEST_CASE(ReadFromFreeCellReturnsDefault) {
    Matrix<int, 0> matrix;
    BOOST_CHECK_EQUAL(matrix[0][0], 0);
    BOOST_CHECK_EQUAL(matrix.size(), 0);
}

// Присваивание
BOOST_AUTO_TEST_CASE(AssignmentCreatesCell) {
    Matrix<int, 0> matrix;
    matrix[100][100] = 314;
    BOOST_CHECK_EQUAL(matrix[100][100], 314);
    BOOST_CHECK_EQUAL(matrix.size(), 1);
}

// Присвоение 0
BOOST_AUTO_TEST_CASE(DefaultValueAssignmentRemovesCell) {
    Matrix<int, 0> matrix;
    matrix[100][100] = 314;
    BOOST_REQUIRE_EQUAL(matrix.size(), 1);

    matrix[100][100] = 0;
    BOOST_CHECK_EQUAL(matrix[100][100], 0);
    BOOST_CHECK_EQUAL(matrix.size(), 0);
}

BOOST_AUTO_TEST_SUITE_END()

// Каноничны operator=
BOOST_AUTO_TEST_SUITE(CanonicalForm)

// Цепочека присваивания
BOOST_AUTO_TEST_CASE(ChainedAssignment) {
    Matrix<int, 0> matrix;

    matrix[100][100] = 314;
    BOOST_REQUIRE_EQUAL(matrix[100][100], 314);
    BOOST_REQUIRE_EQUAL(matrix.size(), 1);

    // Из задания ((matrix[100][100] = 314) = 0) = 217
    ((matrix[100][100] = 314) = 0) = 217;
    BOOST_CHECK_EQUAL(matrix[100][100], 217);
    BOOST_CHECK_EQUAL(matrix.size(), 1);
}

// Цепочка с удалением
BOOST_AUTO_TEST_CASE(ChainedAssignmentWithCleanup) {
    Matrix<int, 0> matrix;

    ((matrix[100][100] = 314) = 0) = 217;
    BOOST_CHECK_EQUAL(matrix[100][100], 217);
    BOOST_CHECK_EQUAL(matrix.size(), 1);

    matrix[100][100] = 0; 
    BOOST_CHECK_EQUAL(matrix[100][100], 0);
    BOOST_CHECK_EQUAL(matrix.size(), 0);
}

BOOST_AUTO_TEST_SUITE_END()

// Заполненеи диагоналей
BOOST_AUTO_TEST_SUITE(DiagonalFilling)

// Количество занятых ячеек
BOOST_AUTO_TEST_CASE(DiagonalFillResultsIn18Cells) {
    Matrix<int, 0> matrix;

    for (int i = 0; i <= 9; ++i) {
        matrix[i][i] = i;
    }

    for (int i = 0; i <= 9; ++i) {
        matrix[i][9 - i] = 9 - i;
    }

    BOOST_CHECK_EQUAL(matrix.size(), 18);
}

// Существование ожидаемых ячеек
BOOST_AUTO_TEST_CASE(CheckAllExpectedCellsExist) {
    Matrix<int, 0> matrix;

    for (int i = 0; i <= 9; ++i) {
        matrix[i][i] = i;
        matrix[i][9 - i] = 9 - i;
    }

    // Все корме [0,0] и [9,0]
    std::set<std::pair<int, int>> expected_cells;
    for (int i = 0; i <= 9; ++i) {
        if (i != 0) expected_cells.insert({ i, i });
        if (i != 9) expected_cells.insert({ i, 9 - i });
    }
    expected_cells.insert({ 0, 9 });
    expected_cells.insert({ 9, 9 });

    BOOST_CHECK_EQUAL(expected_cells.size(), 18);

    for (const auto& cell : expected_cells) {
        BOOST_CHECK_MESSAGE(matrix.exists(cell.first, cell.second),
            "Ячейка [" << cell.first << "," << cell.second << "] должна существовать");
    }
}

// Ячейки с 0 удалены
BOOST_AUTO_TEST_CASE(CheckRemovedCells) {
    Matrix<int, 0> matrix;

    for (int i = 0; i <= 9; ++i) {
        matrix[i][i] = i;
        matrix[i][9 - i] = 9 - i;
    }

    BOOST_CHECK(!matrix.exists(0, 0));
    BOOST_CHECK(!matrix.exists(9, 0));
}

// Проверка значений на диагоналях
BOOST_AUTO_TEST_CASE(CheckDiagonalValues) {
    Matrix<int, 0> matrix;

    for (int i = 0; i <= 9; ++i) {
        matrix[i][i] = i;
        matrix[i][9 - i] = 9 - i;
    }

    // Главная
    BOOST_CHECK_EQUAL(matrix(1, 1), 1);
    BOOST_CHECK_EQUAL(matrix(2, 2), 2);
    BOOST_CHECK_EQUAL(matrix(8, 8), 8);
    BOOST_CHECK_EQUAL(matrix(9, 9), 9);

    // Второстепенная
    BOOST_CHECK_EQUAL(matrix(0, 9), 9);
    BOOST_CHECK_EQUAL(matrix(1, 8), 8);
    BOOST_CHECK_EQUAL(matrix(8, 1), 1);
}

BOOST_AUTO_TEST_SUITE_END()

// Константный доступ
BOOST_AUTO_TEST_SUITE(ConstAccess)

// Чтение
BOOST_AUTO_TEST_CASE(ConstAccessOperator) {
    Matrix<int, 0> matrix;
    matrix[5][5] = 42;

    const auto& const_matrix = matrix;
    BOOST_CHECK_EQUAL(const_matrix(5, 5), 42);
    BOOST_CHECK_EQUAL(const_matrix(10, 10), 0);
}

// Чтение после изменения
BOOST_AUTO_TEST_CASE(ConstReadAfterModification) {
    Matrix<int, 0> matrix;
    matrix[5][5] = 42;

    const auto& const_matrix = matrix;
    BOOST_CHECK_EQUAL(const_matrix(5, 5), 42);

    matrix[5][5] = 84;
    BOOST_CHECK_EQUAL(const_matrix(5, 5), 84);
}

BOOST_AUTO_TEST_SUITE_END()

// ПРоход по занятым ячейкам
BOOST_AUTO_TEST_SUITE(Iteration)

// По нескольким ячейкам
BOOST_AUTO_TEST_CASE(IterateOverOccupiedCells) {
    Matrix<int, 0> matrix;

    matrix[1][1] = 11;
    matrix[2][3] = 23;
    matrix[5][7] = 57;

    std::set<std::tuple<int, int, int>> expected = {
        {1, 1, 11},
        {2, 3, 23},
        {5, 7, 57}
    };

    std::set<std::tuple<int, int, int>> actual;
    for (const auto& [coords, value] : matrix) {
        actual.insert({ coords.first, coords.second, value });
    }

    BOOST_CHECK(actual == expected);
}

// По пустой матрице
BOOST_AUTO_TEST_CASE(IterateOverEmptyMatrix) {
    const Matrix<int, 0> matrix;
    BOOST_CHECK(matrix.begin() == matrix.end());
    BOOST_CHECK_EQUAL(std::distance(matrix.begin(), matrix.end()), 0);
}

// После изменений
BOOST_AUTO_TEST_CASE(IterateAfterModifications) {
    Matrix<int, 0> matrix;

    matrix[1][1] = 11;
    matrix[2][2] = 22;
    BOOST_CHECK_EQUAL(std::distance(matrix.begin(), matrix.end()), 2);

    matrix[2][2] = 0;
    BOOST_CHECK_EQUAL(std::distance(matrix.begin(), matrix.end()), 1);

    matrix[3][3] = 33; 
    BOOST_CHECK_EQUAL(std::distance(matrix.begin(), matrix.end()), 2);
}

BOOST_AUTO_TEST_SUITE_END()

// Очистка матрицы
BOOST_AUTO_TEST_SUITE(ClearMethod)

// С элементами
BOOST_AUTO_TEST_CASE(ClearEmptiesMatrix) {
    Matrix<int, 0> matrix;

    matrix[1][1] = 11;
    matrix[2][2] = 22;
    matrix[3][3] = 33;

    BOOST_REQUIRE_EQUAL(matrix.size(), 3);

    matrix.clear();
    BOOST_CHECK_EQUAL(matrix.size(), 0);
    BOOST_CHECK(matrix.begin() == matrix.end());
}

// Очистка и добавление новых
BOOST_AUTO_TEST_CASE(ClearThenAddNewCells) {
    Matrix<int, 0> matrix;

    matrix[1][1] = 11;
    matrix.clear();

    matrix[5][5] = 55;
    BOOST_CHECK_EQUAL(matrix.size(), 1);
    BOOST_CHECK_EQUAL(matrix[5][5], 55);
}

// Очистка пустой
BOOST_AUTO_TEST_CASE(ClearOnEmptyMatrix) {
    Matrix<int, 0> matrix;
    BOOST_REQUIRE_EQUAL(matrix.size(), 0);
    matrix.clear(); 
    BOOST_CHECK_EQUAL(matrix.size(), 0);
}

BOOST_AUTO_TEST_SUITE_END()

// Граничные случаи
BOOST_AUTO_TEST_SUITE(EdgeCases)

// Отрицательные индексы
BOOST_AUTO_TEST_CASE(NegativeIndices) {
    Matrix<int, 0> matrix;

    matrix[-5][-5] = 42;
    BOOST_CHECK_EQUAL(matrix[-5][-5], 42);
    BOOST_CHECK_EQUAL(matrix.size(), 1);

    matrix[-5][-5] = 0;
    BOOST_CHECK_EQUAL(matrix[-5][-5], 0);
    BOOST_CHECK_EQUAL(matrix.size(), 0);
}

// Большие индексы
BOOST_AUTO_TEST_CASE(LargeIndices) {
    Matrix<int, 0> matrix;

    matrix[1000000][1000000] = 123;
    BOOST_CHECK_EQUAL(matrix[1000000][1000000], 123);
    BOOST_CHECK_EQUAL(matrix.size(), 1);
}

// Перезапись существующей ячейки
BOOST_AUTO_TEST_CASE(OverwriteExistingCell) {
    Matrix<int, 0> matrix;

    matrix[5][5] = 42;
    BOOST_REQUIRE_EQUAL(matrix[5][5], 42);
    BOOST_REQUIRE_EQUAL(matrix.size(), 1);

    matrix[5][5] = 84;
    BOOST_CHECK_EQUAL(matrix[5][5], 84);
    BOOST_CHECK_EQUAL(matrix.size(), 1);
}

// Чтение после удаления
BOOST_AUTO_TEST_CASE(ReadAfterDelete) {
    Matrix<int, 0> matrix;

    matrix[5][5] = 42;
    matrix[5][5] = 0;

    BOOST_CHECK_EQUAL(matrix[5][5], 0);
    BOOST_CHECK_EQUAL(matrix.size(), 0);
}

BOOST_AUTO_TEST_SUITE_END()