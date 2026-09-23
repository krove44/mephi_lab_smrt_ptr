#include <gtest/gtest.h>
#include <chrono>
#include <memory>
#include <vector>
#include <iostream>
#include "../ptr/shared_ptr.h"
//control block
//move семантика
struct Payload {
    uint64_t data[4];
    Payload() : data{1, 2, 3, 4} {}
};

class SharedPointerBenchmark : public ::testing::Test {
protected:
    template <typename Func>
    void RunBenchmark(const std::string& name, size_t iterations, Func&& func) {
        auto start = std::chrono::high_resolution_clock::now();

        func(iterations);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> ms = end - start;

        std::cout << "[ Bench ] " << name << " | Objects: " << iterations
                  << " | Time: " << ms.count() << " ms\n";
    }
};

//Затраты по памяти
TEST_F(SharedPointerBenchmark, MemoryOverhead) {
    std::cout << "-------------     Memory Test     -------------\n";
    std::cout << "[ Memory ] shared_ptr size: " << sizeof(shared_ptr<Payload>) << " bytes\n";
    std::cout << "[ Memory ] std::shared_ptr size: " << sizeof(std::shared_ptr<Payload>) << " bytes\n";
    std::cout << "[ Memory ] pointer size: " << sizeof(Payload*) << " bytes\n";
}

//тестирование на малом числе объектов
TEST_F(SharedPointerBenchmark, SmallScaleAllocations) {
    std::cout << "-------------     Small Object Test     -------------\n";
    const size_t COUNT = 10'000;

    RunBenchmark("Pointers (Small)", COUNT, [](size_t count) {
        std::vector<Payload*> vec;
        vec.reserve(count);
        for (size_t i = 0; i < count; ++i) vec.push_back(new Payload());
        for (auto p : vec) delete p;
    });

    RunBenchmark("std::shared_ptr (Small)", COUNT, [](size_t count) {
        std::vector<std::shared_ptr<Payload>> vec;
        vec.reserve(count);
        for (size_t i = 0; i < count; ++i) vec.push_back(std::make_shared<Payload>());
    });

    RunBenchmark("My shared_ptr (Small)", COUNT, [](size_t count) {
        std::vector<shared_ptr<Payload>> vec;
        vec.reserve(count);
        for (size_t i = 0; i < count; ++i) vec.push_back(shared_ptr<Payload>(new Payload()));
    });
}

//тестирование на большом числе объектов
TEST_F(SharedPointerBenchmark, LargeScaleAllocations) {
    std::cout << "-------------     More Object Test     -------------\n";
    const size_t COUNT = 10'000'000;

    RunBenchmark("Pointers (Big)", COUNT, [](size_t count) {
        std::vector<Payload*> vec;
        vec.reserve(count);
        for (size_t i = 0; i < count; ++i) vec.push_back(new Payload());
        for (auto p : vec) delete p;
    });

    RunBenchmark("std::shared_ptr (Big)", COUNT, [](size_t count) {
        std::vector<std::shared_ptr<Payload>> vec;
        vec.reserve(count);
        for (size_t i = 0; i < count; ++i) vec.push_back(std::make_shared<Payload>());
    });

    RunBenchmark("My shared_ptr (Big)", COUNT, [](size_t count) {
        std::vector<shared_ptr<Payload>> vec;
        vec.reserve(count);
        for (size_t i = 0; i < count; ++i) vec.push_back(shared_ptr<Payload>(new Payload()));
    });
}

//тестирование накладных расходов на копирование
TEST_F(SharedPointerBenchmark, CopyOverhead) {
    std::cout << "-------------     Copy Test     -------------\n";
    const size_t COUNT = 1'000'000;

    RunBenchmark("std::shared_ptr Copy", COUNT, [](size_t count) {
        auto original = std::make_shared<Payload>();
        std::vector<std::shared_ptr<Payload>> vec;
        vec.reserve(count);
        for (size_t i = 0; i < count; ++i) vec.push_back(original);
    });

    RunBenchmark("My shared_ptr Copy", COUNT, [](size_t count) {
        shared_ptr<Payload> original(new Payload());
        std::vector<shared_ptr<Payload>> vec;
        vec.reserve(count);
        for (size_t i = 0; i < count; ++i) vec.push_back(original);
    });
}

TEST_F(SharedPointerBenchmark, UseCountCorrectness) {
    shared_ptr<Payload> a(new Payload());
    EXPECT_EQ(a.use_count(), 1);

    {
        shared_ptr<Payload> b = a;
        EXPECT_EQ(a.use_count(), 2);
        EXPECT_EQ(b.use_count(), 2);

        shared_ptr<Payload> c;
        c = b;
        EXPECT_EQ(a.use_count(), 3);
    }

    EXPECT_EQ(a.use_count(), 1);
}

