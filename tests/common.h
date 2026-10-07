#pragma once
#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include "../ptr/shared_ptr.h"
#include "../ptr/weak_ptr.h"

struct Payload {
    uint64_t data[4];
    Payload() : data{1, 2, 3, 4} {}
};


struct Tracked {
    static inline int alive = 0;
    static inline int destroyed = 0;
    Tracked()  { ++alive; }
    ~Tracked() { --alive; ++destroyed; }
    static void reset() { alive = 0; destroyed = 0; }
};

struct NVBase {
    int x = 1;
};
struct NVDerived : NVBase {
    static inline int destroyed = 0;
    int extra[4] = {};
    ~NVDerived() { ++destroyed; }
};

template <typename Func>
void RunBenchmark(const std::string& name, size_t iterations, Func&& func) {
    auto start = std::chrono::high_resolution_clock::now();
    func(iterations);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms = end - start;
    std::cout << "[ Bench ] " << name << " | Objects: " << iterations
              << " | Time: " << ms.count() << " ms\n";
}