#include "src/util/densemap.hpp"

#include <string>
#include <iostream>
#include <cassert>

void test_duplicate_insert(nasal::util::densemap<std::string, int>& m) {
    m.insert("a", 1);
    m.insert("b", 2);
    m.insert("a", 100);  // duplicate key: overwrite, not add a second entry

    assert(m.size() == 2);
    assert(m.at("a") == 100);
    assert(m.at("b") == 2);

    // erase the overwritten key: no dangling duplicate left behind
    m.erase("a");
    assert(m.size() == 1);
    assert(!m.contains("a"));
    assert(m.at("b") == 2);

    // re-insert then erase the last element (entry == back case)
    m.insert("a", 7);
    assert(m.at("a") == 7);
    m.erase("a");
    assert(m.size() == 1);
    assert(m.contains("b"));

    // iteration must see exactly the live entries
    int count = 0;
    for (auto& [key, value] : m) {
        (void)key;
        (void)value;
        ++count;
    }
    assert(count == 1);
}

int main() {
    nasal::util::densemap<std::string, int> m;
    test_duplicate_insert(m);
    std::cout << "PASS" << std::endl;
    m.clear();
    test_duplicate_insert(m);
    std::cout << "PASS" << std::endl;
    return 0;
}
