#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <map>
#include <utility>

template<typename T, T DefaultValue>
class Matrix {
private:
    std::map<std::pair<int, int>, T> data;  // храним только занятые ячейки

public:
    Matrix() = default;

    // Прокси для доступа к ячейке
    class CellProxy {
    private:
        Matrix& matrix;
        int m_x;
        int m_y;

    public:
        CellProxy(Matrix& m, int x, int y) : matrix(m), m_x(x), m_y(y) {}

        // Присваивание значения ячейке
        CellProxy& operator=(const T& value) {
            auto key = std::make_pair(m_x, m_y);

            if (value == DefaultValue) {
                matrix.data.erase(key);  // если 0 - освобождаем
            }
            else {
                matrix.data[key] = value; 
            }
            return *this;
        }

        // Каноничная форма
        CellProxy& operator=(const CellProxy& other) {
            *this = static_cast<T>(other);
            return *this;
        }

        // Чтение значения ячейки
        operator T() const {
            auto it = matrix.data.find(std::make_pair(m_x, m_y));
            if (it != matrix.data.end()) {
                return it->second;
            }
            return DefaultValue;  // ячейка свободна - 0
        }
    };

    // Прокси для доступа к строке
    class RowProxy {
    private:
        Matrix& matrix;
        int m_x;

    public:
        RowProxy(Matrix& m, int x) : matrix(m), m_x(x) {}

        CellProxy operator[](int y) {
            return CellProxy(matrix, m_x, y);
        }
    };

    RowProxy operator[](int x) {
        return RowProxy(*this, x);
    }

    // Константный доступ по координатам
    T operator()(int x, int y) const {
        auto it = data.find(std::make_pair(x, y));
        if (it != data.end()) {
            return it->second;
        }
        return DefaultValue;
    }

    size_t size() const {
        return data.size();
    }

    bool exists(int x, int y) const {
        return data.find(std::make_pair(x, y)) != data.end();
    }

    // Очистка матрицы
    void clear() {
        data.clear();
    }

    // Итератор для обхода занятых ячеек
    using iterator = typename std::map<std::pair<int, int>, T>::const_iterator;
    using const_iterator = typename std::map<std::pair<int, int>, T>::const_iterator;

    iterator begin() const {
        return data.begin();
    }

    iterator end() const {
        return data.end();
    }

    const_iterator cbegin() const {
        return data.cbegin();
    }

    const_iterator cend() const {
        return data.cend();
    }
};

#endif // MATRIX_HPP