#include "alloc.h"

#include <iostream>

void check(const char *p, const std::string &s, bool f = true) {
    if (f and p == nullptr){
        throw std::runtime_error(s);
    }
    if (!f and p != nullptr) {
        throw std::runtime_error(s);
    }
}

// тесты
int main() {
    size_t sizes[3] = {5, 10, 100}; //создадим три объекта класса Аллокатор с разными макс. размерами

    // size = 5
    Allocator alloc1;
    alloc1.makeAllocator(sizes[0]);
    char *p1 = alloc1.alloc(0); //попробуем аллоцировать 0 чаров
    check(p1, "Reserved size = 5 \n Allocation size = 0 \n Returned: nullptr \n"); //если не вышло (вернулся nullptr), то выкинем исключение
    p1 = alloc1.alloc(1); //попробуем аллоцировать 1 чар
    check(p1, "Reserved size = 5 \n Allocation size = 1 \n Returned: nullptr \n");
    alloc1.reset(); //попробуем использовать память заново
    p1 = alloc1.alloc(6); //попробуем аллоцировать 6 чаров с макс. размером = 5
    check(p1, "Reserved size = 5 \n Allocation size = 6 \n Returned: non-nullptr \n", false); //если вернулся не nullptr, выкинем исключение


    // size = 10
    Allocator alloc2;
    alloc2.makeAllocator(sizes[1]);
    char *p2 = alloc2.alloc(7); //попробуем аллоцировать 7 чаров
    check(p2, "Reserved size = 10 \n Allocated size = 7 \n Returned: nullptr \n");
    p2 = alloc2.alloc(3); //попробуем аллоцировать еще 3 чара
    check(p2, "Reserved size = 10 \n Allocated size = 10 \n Returned: nullptr \n");
    p2 = alloc2.alloc(1); //попробуем аллоцировать еще 1 чар
    check(p2, "Reserved size = 10 \n Allocated size = 11 \n Returned: non-nullptr \n", false);


    // size = 100
    Allocator alloc3;
    alloc3.makeAllocator(sizes[2]);
    char *p3 = alloc3.alloc(54); //попробуем аллоцировать 54 чара
    check(p3, "Reserved size = 100 \n Allocated size = 54 \n Returned: nullptr \n");
    p3 = alloc3.alloc(8); //еще 8 чаров
    check(p3, "Reserved size = 100 \n Allocated size = 62 \n Returned: nullptr \n");
    alloc3.reset(); //используем память заново
    p3 = alloc3.alloc(40); //аллоцируем 40 чаров
    check(p3, "Reserved size = 100 \n Allocated size = 40 \n Returned: nullptr \n");
    p3 = alloc3.alloc(61); //еще 61
    check(p3, "Reserved size = 100 \n Allocated size = 101 \n Returned: non-nullptr \n", false);
}
