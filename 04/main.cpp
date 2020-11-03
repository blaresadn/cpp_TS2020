#include <sstream>

#include "bigint.h"

int main() {
    BigInt a("11");
    BigInt b("12");

    BigInt c1(a + b);
    BigInt c2 = 23;
    if (c1 != c2) {
        throw std::runtime_error("Оператор != возвращает true для равных чисел");
    }

    std::ostringstream oss1;
    oss1 << a;
    if (oss1.str() != "11") {
        throw std::runtime_error("Оператор << работает неверно");
    }

    BigInt d = a * 2 + b;
    std::ostringstream oss2;
    oss2 << -d;
    if (oss2.str() != "-34") {
        throw std::runtime_error("Арифметические операции возвращают неверное значение");
    }

    a = -b;
    if (a + b != 0) {
        throw std::runtime_error("Оператор != возвращает true для равных чисел");
    }

    BigInt big_n("999999999999999999999999999999"); // 30 девяток
    big_n = big_n + 1;
    std::ostringstream oss3;
    oss3 << big_n;
    if (oss3.str() != "1000000000000000000000000000000") { // 1 и 30 нулей
        throw std::runtime_error("Арифметические операции возвращают неверное значение");
    }

    a = 1000;
    b = -1000;
    b = a * b + 190 - (-BigInt(40)) * 6;
    if (b != -999570) {
        throw std::runtime_error("Оператор != возвращает true для равных чисел");
    }

    a = -700;
    a = a * 3 - 12 + (-a);
    if (a != -1412) {
        throw std::runtime_error("Оператор != возвращает true для равных чисел");
    }

    if (b > a) { // b = -999570, a = -1412
        throw std::runtime_error("Оператор > возвращает неверное значение");
    }
    if (big_n <= 1234567890) {
        throw std::runtime_error("Оператор <= возвращает неверное значение");
    }
    if (BigInt(10) == 11) {
        throw std::runtime_error("Оператор == возвращает true для неравных чисел");
    }
    if (a >= 0) {
        throw std::runtime_error("Оператор >= возвращает неверное значение");
    }

    return 0;
}
