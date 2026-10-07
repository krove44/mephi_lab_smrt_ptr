#include <vector>
#include <memory>
#include "common.h"

TEST(BenchCopy, CopyOverhead) {
    const size_t COUNT = 1'000'000;

    RunBenchmark("std::shared_ptr copy", COUNT, [](size_t n) {
        auto original = std::make_shared<Payload>();
        std::vector<std::shared_ptr<Payload>> v; v.reserve(n);
        for (size_t i = 0; i < n; ++i) v.push_back(original);
    });
    RunBenchmark("My shared_ptr copy", COUNT, [](size_t n) {
        shared_ptr<Payload> original(new Payload());
        std::vector<shared_ptr<Payload>> v; v.reserve(n);
        for (size_t i = 0; i < n; ++i) v.push_back(original);
    });
    RunBenchmark("My shared_ptr move into vector", COUNT, [](size_t n) {
        std::vector<shared_ptr<Payload>> v; v.reserve(n);
        for (size_t i = 0; i < n; ++i) v.push_back(shared_ptr<Payload>(new Payload()));
    });
}