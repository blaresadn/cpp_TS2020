#include <iostream>

#include "vector.h"

int main() {
    Vector<int> v1(5, 5);
    assert(v1.size() == 5);
    assert(v1.capacity() == 5);

    for (int i = 0; i < 10; i++) {
        v1.push_back(i + 6);
    }
    assert(v1[5] == 6);
    assert(v1[10] == 11);

    v1.clear();
    assert(v1.empty());

    Vector<std::string> v2 = {"Ufa", "is", "the", "capital", "of"};
    v2.emplace_back("Bashkortostan");
    v2.emplace_back("republic.");
    assert(v2[6] == "republic.");

    Vector<std::string> v3 = v2;
    v3.emplace_back("Moscow");
    v3.emplace_back("is");
    v3.pop_back();
    v3.pop_back();
    assert(v3.size() == v2.size());
    assert(*(v3.end() - 2) == "Bashkortostan");
    assert(v3.begin() < v3.end());
    assert(*(v3.rend() - 1) == "Ufa");

    Vector<int> v4 {1, 2, 3, 4, 5};
    v4.resize(2, 1);
    assert(v4.size() == 2);
    v4.resize(10, 1);
    assert(v4[9] == 1);
    assert(v4.size() == 10);
    v4.reserve(30);
    assert(v4.capacity() == 30);

    Vector<int> v5;
    for (int i = 10; i < 20; i++) {
        v5.push_back(i);
    }
    for (auto it = v5.begin(); it < v5.end(); it++) {
        assert(*it == (it - v5.begin() + 10));
    }
    for (int i = 0; i < 5; i++) {
        v5.pop_back();
    }
    assert(v5.capacity() == 10);
    assert(v5.size() == 5);

    Vector<int> v6 = Vector({1, 2, 3, 7, 8});
    Vector<int> v7(v5);
    assert(v6.capacity() == 5);
    assert(v7.size() == v5.size());

    return 0;
}