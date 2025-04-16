#include <cstdlib>
#include <ctime>
#include <iostream>
#include <unordered_set>
#include <vector>

std::vector<int> intersection(const std::vector<int> &a,
                              const std::vector<int> &b) {
    std::vector<int> c;

    for (int i = 0; i < a.size(); ++i) {
        for (int j = 0; j < b.size(); ++j) {
            if (a[i] == b[j]) {
                c.push_back(a[i]);
                break;
            }
        }
    }

    return c;
}

std::vector<int> intersection2(const std::vector<int> &a,
                               const std::vector<int> &b) {
    std::unordered_set<int> c;

    auto &smaller = a.size() < b.size() ? a : b;
    auto &larger = a.size() < b.size() ? b : a;

    for (const auto &i : smaller) {
        c.insert(i);
    }

    std::vector<int> intersection;

    for (const auto &i : larger) {
        if (c.find(i) != c.end()) {
            intersection.push_back(i);
        }
    }

    return intersection;
}

int main() {
    srand(time(NULL));

    int maxIterations = 100000000;

    std::vector<int> a;
    std::vector<int> b;

    for (int i = 0; i < maxIterations; ++i) {
        a.push_back(i);
        b.push_back(i);
    }

    // Time the execution of intersection2
    clock_t start = clock();
    auto result2 = intersection2(a, b);
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    double time_spen_ms = time_spent * 1000;
    std::cerr << "Intersection2 result: " << result2.size() << "\n";
    std::cerr << "Intersection2 took: " << time_spen_ms << " ms\n";

    return 0;
}
