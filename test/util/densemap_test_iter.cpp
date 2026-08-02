#include "src/util/densemap.hpp"

#include <string>
#include <iostream>
#include <vector>
#include <cassert>

const int N = 500000;

void test(nasal::util::densemap<std::string, int>& m) {
    std::vector<int> v;

    for (int i = 0; i < N; i++) {
        m.insert(std::to_string(i), i);
    }

    v.resize(N, 0);
    for (auto& [key, value] : m) {
        v[value] = 1;
    }
    for (int i = 0; i < N; i++) {
        assert(v[i] == 1);
    }

    v.resize(N, 0);
    for (const auto& [key, value] : m) {
        v[value] = 1;
    }
    for (int i = 0; i < N; i++) {
        assert(v[i] == 1);
    }
}

int main() {
    nasal::util::densemap<std::string, int> m;
    test(m);
    std::cout << "PASS" << std::endl;
    m.clear();
    test(m);
    std::cout << "PASS" << std::endl;
    return 0;
}