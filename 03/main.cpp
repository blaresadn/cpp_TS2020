#include <sstream>

#include "matrix.h"

int main() {
    Matrix m1(5, 7);

    if (m1.getRows() != 5 || m1.getColumns() != 7) {
        throw std::runtime_error("Ошибка: размеры матрицы были инициализировны неверно");
    }

    for (int i = 0; i < m1.getRows(); i++) {
        for (int j = 0; j < m1.getColumns(); j++) {
            m1[i][j] = (i + 1) * (j + 1) * 2; //заполняем матрицу m1 значениями 2 4 6 ... 70
        }
    }

    int a = m1[0][0];
    if (a != 2) {
        throw std::runtime_error("Ошибка: матрица была инициализирована неверно");
    }

    Matrix m2(5, 7);
    for (int i = 0; i < m2.getRows(); i++) {
        for (int j = 0; j < m2.getColumns(); j++) {
            m2[i][j] = (i + 1) * (j + 1); //заполняем матрицу m2 значениями 1 2 3 ... 35
        }
    }
    m2 *= 2; //умножаем матрицу m2 на число 2, чтобы сравнить ее с матрицей m1

    if (m1 != m2) {
        throw std::runtime_error("Ошибка: оператор != вернул true для равных матриц");
    }

    Matrix m3(5, 7);
    for (int i = 0; i < m3.getRows(); i++) {
        for (int j = 0; j < m3.getColumns(); j++) {
            m3[i][j] = 5; //создаем матрицу из пятерок
        }
    }
    m1 = m1 + m3; //теперь элементы матрицы m1 равны 7 9 11 ... 75

    for (int i = 0; i < m1.getRows(); i++) {
        for (int j = 0; j < m1.getColumns(); j++) {
            if (m1[i][j] != (i + 1) * (j + 1) * 2 + 5) { //сравниваем элементы матрицы m1 со значениями 7 9 11 ... 75
                throw std::runtime_error("Ошибка: неверный результат работы оператора +");
            }
        }
    }

    if (m1 == m2) { //матрицы m1 и m2 больше не равны
        throw std::runtime_error("Ошибка: оператор == вернул true для неравных матриц");
    }

    std::ostringstream oss;
    oss << m1 << m2;
    std::string s = oss.str();
    if (s[6] != '7' || s[5] != ' ') { //проверяем работу оператора <<
        throw std::runtime_error("Ошибка: неверный результат работы оператора <<");
    }

    return 0;
}
