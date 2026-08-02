#include "src/util/densemap.hpp"

#include <string>
#include <unordered_map>
#include <iostream>
#include <chrono>
#include <cassert>
#include <vector>
#include <algorithm>
#include <random>

constexpr int N = 500000;

template<typename F>
auto time_ms(F&& f) {
    auto t1 = std::chrono::high_resolution_clock::now();
    f();
    auto t2 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
}

std::vector<int> shuffled_indices(int n) {
    std::vector<int> v(n);
    for (int i = 0; i < n; i++) v[i] = i;
    std::mt19937 rng(42);
    std::shuffle(v.begin(), v.end(), rng);
    return v;
}

std::vector<std::string> keys(int n) {
    std::vector<std::string> k;
    k.reserve(n);
    for (int i = 0; i < n; i++) k.push_back(std::to_string(i));
    return k;
}

void test_random_insert() {
    auto ks = keys(N);
    auto order = shuffled_indices(N);

    // correctness reference
    std::unordered_map<std::string, int> ref;

    auto t_stl = time_ms([&] {
        for (int i : order) ref.insert({ks[i], i});
    });

    nasal::util::densemap<std::string, int> dm;
    auto t_dense = time_ms([&] {
        for (int i : order) dm.insert(ks[i], i);
    });

    // verify
    for (int i = 0; i < N; i++) {
        auto it = ref.find(ks[i]);
        assert(it != ref.end() && it->second == i);
    }
    for (int i = 0; i < N; i++) {
        assert(dm.at(ks[i]) == i);
    }

    std::cout << "random insert (" << N << " keys):\n";
    std::cout << "  unordered_map: " << t_stl << " ms\n";
    std::cout << "  densemap:      " << t_dense << " ms\n\n";
}

void test_random_lookup() {
    auto ks = keys(N);
    auto order = shuffled_indices(N);

    std::unordered_map<std::string, int> ref;
    nasal::util::densemap<std::string, int> dm;
    for (int i = 0; i < N; i++) {
        ref.insert({ks[i], i});
        dm.insert(ks[i], i);
    }

    // correctness + timing
    volatile int sink = 0;

    auto t_stl = time_ms([&] {
        for (int idx : order) sink += ref.at(ks[idx]);
    });

    auto t_dense = time_ms([&] {
        for (int idx : order) sink += dm.at(ks[idx]);
    });

    (void)sink;

    std::cout << "random lookup (" << N << " keys):\n";
    std::cout << "  unordered_map: " << t_stl << " ms\n";
    std::cout << "  densemap:      " << t_dense << " ms\n\n";
}

void test_mixed_workload() {
    auto ks = keys(N);
    auto order = shuffled_indices(N);

    std::unordered_map<std::string, int> ref;
    nasal::util::densemap<std::string, int> dm;

    // avoid compiler optimizing away the calculations
    volatile int sink = 0;

    auto t_stl = time_ms([&] {
        // insert 80%
        for (int i = 0; i < N * 0.8; i++) {
            int idx = order[i];
            ref.insert({ks[idx], idx});
        }
        // lookup 20% (existing + missing)
        for (int i = 0; i < N * 0.2; i++) {
            int idx = order[N - 1 - i];
            auto it = ref.find(ks[idx]);
            if (it != ref.end()) sink += it->second;
        }
    });

    auto t_dense = time_ms([&] {
        for (int i = 0; i < N * 0.8; i++) {
            int idx = order[i];
            dm.insert(ks[idx], idx);
        }
        for (int i = 0; i < N * 0.2; i++) {
            int idx = order[N - 1 - i];
            if (dm.contains(ks[idx])) sink += dm.at(ks[idx]);
        }
    });

    (void)sink;

    std::cout << "mixed 80% insert / 20% lookup (" << N << " keys):\n";
    std::cout << "  unordered_map: " << t_stl << " ms\n";
    std::cout << "  densemap:      " << t_dense << " ms\n\n";
}

void test_operator_bracket() {
    auto ks = keys(N);
    auto order = shuffled_indices(N);

    std::unordered_map<std::string, int> ref;
    auto t_stl = time_ms([&] {
        for (int i : order) ref[ks[i]] = i;
    });

    nasal::util::densemap<std::string, int> dm;
    auto t_dense = time_ms([&] {
        for (int i : order) dm[ks[i]] = i;
    });

    // verify
    for (int i = 0; i < N; i++) {
        assert(ref[ks[i]] == i);
    }
    for (int i = 0; i < N; i++) {
        assert(dm.at(ks[i]) == i);
    }

    std::cout << "random operator[] insert (" << N << " keys):\n";
    std::cout << "  unordered_map: " << t_stl << " ms\n";
    std::cout << "  densemap:      " << t_dense << " ms\n\n";
}

void test_erase() {
    auto ks = keys(N);
    auto order = shuffled_indices(N);
    std::unordered_map<std::string, int> ref;
    nasal::util::densemap<std::string, int> dm;

    for (int i = 0; i < N; i++) {
        ref.insert({ks[i], i});
        dm.insert(ks[i], i);
    }

    auto t_stl = time_ms([&] {
        for (int i : order) ref.erase(ks[i]);
        ref.erase("foo");

        assert(ref.size() == 0);
    });
    auto t_dense = time_ms([&] {
        for (int i : order) dm.erase(ks[i]);
        dm.erase("foo");

        assert(dm.size() == 0);
    });

    std::cout << "erase (" << N << " keys):\n";
    std::cout << "  unordered_map: " << t_stl << " ms\n";
    std::cout << "  densemap:      " << t_dense << " ms\n\n";
}

void test_erase_insert() {
    auto ks = keys(N);
    auto order = shuffled_indices(N);
    std::unordered_map<std::string, int> ref;
    nasal::util::densemap<std::string, int> dm;

    for (int i = 0; i < N; i++) {
        ref.insert({ks[i], i});
        dm.insert(ks[i], i);
    }

    auto t_stl = time_ms([&] {
        for (int i : order) {
            ref.erase(ks[i]);
            ref.insert({ks[i], i});
        }

        assert(ref.size() == order.size());
    });
    auto t_dense = time_ms([&] {
        for (int i : order) {
            dm.erase(ks[i]);
            dm.insert(ks[i], i);
        }

        assert(dm.size() == order.size());
    });

    std::cout << "erase and insert (" << N << " keys):\n";
    std::cout << "  unordered_map: " << t_stl << " ms\n";
    std::cout << "  densemap:      " << t_dense << " ms\n\n";
}

int main() {
    test_random_insert();
    test_random_lookup();
    test_mixed_workload();
    test_operator_bracket();
    test_erase();
    test_erase_insert();
    return 0;
}
