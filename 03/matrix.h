#include <cstdlib>
#include <iostream>
#include <iomanip>

class Row
{
    size_t columns_;
    int *row_;

public:
    Row(int *matrix, size_t columns): row_(matrix), columns_(columns)
    {
    }

    int &operator [](int j)
    {
        if (j >= columns_) {
            throw std::out_of_range("Попытка выйти за пределы матрицы");
        }
        return row_[j];
    }
};

class Matrix
{
private:
    size_t rows_;
    size_t columns_;
    int **matrix_;

public:
    Matrix(size_t rows, size_t columns): rows_(rows), columns_(columns)
    {
        matrix_ = new int*[rows_];
        for (size_t i = 0; i < rows_; i++) {
            matrix_[i] = new int[columns_];
        }
    }

    Row operator [](size_t i)
    {
        if (i >= rows_) {
            throw std::out_of_range("Попытка выйти за пределы матрицы");
        }
        Row row(matrix_[i], columns_);
        return row;
    }

    size_t getRows() const
    {
        return rows_;
    }

    size_t getColumns() const
    {
        return columns_;
    }

    Matrix &operator =(Matrix m)
    {
        for (int i = 0; i < rows_; i++) {
            delete [] matrix_[i];
        }
        delete [] matrix_;
        rows_ = m.getRows();
        columns_ = m.getColumns();
        matrix_ = new int*[rows_];
        for (size_t i = 0; i < rows_; i++) {
            matrix_[i] = new int[columns_];
        }
        for (size_t i = 0; i < rows_; i++) {
            for (size_t j = 0; j < columns_; j++) {
                matrix_[i][j] = m[i][j];
            }
        }
        return *this;
    }

    Matrix &operator *=(int a)
    {
        for (size_t i = 0; i < rows_; i++) {
            for (size_t j = 0; j < columns_; j++) {
                matrix_[i][j] *= a;
            }
        }
        return *this;
    }

    Matrix operator +(Matrix &m)
    {
        if (m.getRows() != rows_ or m.getColumns() != columns_) {
            throw std::out_of_range("Операция сложения: не совпадают размеры матриц");
        }
        Matrix res(rows_, columns_);
        for (size_t i = 0; i < rows_; i++) {
            for (size_t j = 0; j < columns_; j++) {
                res[i][j] = matrix_[i][j] + m[i][j];
            }
        }
        return res;
    }

    bool operator ==(Matrix &m)
    {
        if (m.getRows() != rows_ or m.getColumns() != columns_) {
            throw std::out_of_range("Операция сравнения: не совпадают размеры матриц");
        }
        for (int i = 0; i < rows_; i++) {
            for (int j = 0; j < columns_; j++) {
                if (matrix_[i][j] != m[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }

    bool operator !=(Matrix &m)
    {
        if (m.getRows() != rows_ or m.getColumns() != columns_) {
            throw std::out_of_range("Операция сравнения: не совпадают размеры матриц");
        }
        for (int i = 0; i < rows_; i++) {
            for (int j = 0; j < columns_; j++) {
                if (matrix_[i][j] != m[i][j]) {
                    return true;
                }
            }
        }
        return false;
    }

    ~Matrix()
    {
        for (int i = 0; i < rows_; i++) {
            delete [] matrix_[i];
        }
        delete [] matrix_;
    }

    friend std::ostream &operator <<(std::ostream &oss, Matrix &m);
};

std::ostream &operator <<(std::ostream &os, Matrix &m)
{
    for (size_t i = 0; i < m.getRows(); i++) {
        for (size_t j = 0; j < m.getColumns() - 1; j++) {
            os << std::setw(7) << m[i][j] << ' ';
        }
        os << std::setw(7) << m[i][m.getColumns() - 1] << std::endl;
    }
    return os;
}
