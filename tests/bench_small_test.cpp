#include <vector>
#include <memory>
#include "common.h"

TEST(BenchSmall, Allocations) {
    std::cout << "-------------     Small Object Test     -------------\n";
    const size_t COUNT = 10'000;

    RunBenchmark("Raw pointers", COUNT, [](size_t n) {
        std::vector<Payload*> v; v.reserve(n);
        for (size_t i = 0; i < n; ++i) v.push_back(new Payload());
        for (auto p : v) delete p;
    });
    RunBenchmark("std::shared_ptr (make_shared)", COUNT, [](size_t n) {
        std::vector<std::shared_ptr<Payload>> v; v.reserve(n);
        for (size_t i = 0; i < n; ++i) v.push_back(std::make_shared<Payload>());
    });
    RunBenchmark("std::shared_ptr (new)", COUNT, [](size_t n) {
        std::vector<std::shared_ptr<Payload>> v; v.reserve(n);
        for (size_t i = 0; i < n; ++i) v.push_back(std::shared_ptr<Payload>(new Payload()));
    });
    RunBenchmark("My shared_ptr", COUNT, [](size_t n) {
        std::vector<shared_ptr<Payload>> v; v.reserve(n);
        for (size_t i = 0; i < n; ++i) v.push_back(shared_ptr<Payload>(new Payload()));
    });
    std::cout << "\n";
}