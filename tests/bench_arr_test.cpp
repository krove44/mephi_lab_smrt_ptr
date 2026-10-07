#include <vector>
#include <memory>
#include "common.h"
#include "../ptr/shared_ptr_arr.h"

const size_t ARR_SIZE = 2;

TEST(BenchArrSmall, Allocations) {
    const size_t COUNT = 10'000;

    RunBenchmark("Raw arrays", COUNT, [](size_t n) {
        std::vector<Payload*> v; v.reserve(n);
        for (size_t i = 0; i < n; ++i) v.push_back(new Payload[ARR_SIZE]);
        for (auto p : v) delete[] p;
    });
    RunBenchmark("std::shared_ptr<T[]>", COUNT, [](size_t n) {
        std::vector<std::shared_ptr<Payload[]>> v; v.reserve(n);
        for (size_t i = 0; i < n; ++i) v.push_back(std::shared_ptr<Payload[]>(new Payload[ARR_SIZE]));
    });
    RunBenchmark("My shared_ptr<T[]>", COUNT, [](size_t n) {
        std::vector<shared_ptr<Payload[]>> v; v.reserve(n);
        for (size_t i = 0; i < n; ++i) v.push_back(shared_ptr<Payload[]>(new Payload[ARR_SIZE]));
    });
}

TEST(BenchArrLarge, Allocations) {
    const size_t COUNT = 1'000'000;

    RunBenchmark("Raw arrays", COUNT, [](size_t n) {
        std::vector<Payload*> v; v.reserve(n);
        for (size_t i = 0; i < n; ++i) v.push_back(new Payload[ARR_SIZE]);
        for (auto p : v) delete[] p;
    });
    RunBenchmark("std::shared_ptr<T[]>", COUNT, [](size_t n) {
        std::vector<std::shared_ptr<Payload[]>> v; v.reserve(n);
        for (size_t i = 0; i < n; ++i) v.push_back(std::shared_ptr<Payload[]>(new Payload[ARR_SIZE]));
    });
    RunBenchmark("My shared_ptr<T[]>", COUNT, [](size_t n) {
        std::vector<shared_ptr<Payload[]>> v; v.reserve(n);
        for (size_t i = 0; i < n; ++i) v.push_back(shared_ptr<Payload[]>(new Payload[ARR_SIZE]));
    });
}

TEST(BenchArrCopy, CopyOverhead) {
    const size_t COUNT = 1'000'000;

    RunBenchmark("std::shared_ptr<T[]> copy", COUNT, [](size_t n) {
        std::shared_ptr<Payload[]> original(new Payload[ARR_SIZE]);
        std::vector<std::shared_ptr<Payload[]>> v; v.reserve(n);
        for (size_t i = 0; i < n; ++i) v.push_back(original);
    });
    RunBenchmark("My shared_ptr<T[]> copy", COUNT, [](size_t n) {
        shared_ptr<Payload[]> original(new Payload[ARR_SIZE]);
        std::vector<shared_ptr<Payload[]>> v; v.reserve(n);
        for (size_t i = 0; i < n; ++i) v.push_back(original);
    });
    RunBenchmark("My shared_ptr<T[]> move into vector", COUNT, [](size_t n) {
        std::vector<shared_ptr<Payload[]>> v; v.reserve(n);
        for (size_t i = 0; i < n; ++i) v.push_back(shared_ptr<Payload[]>(new Payload[ARR_SIZE]));
    });
}