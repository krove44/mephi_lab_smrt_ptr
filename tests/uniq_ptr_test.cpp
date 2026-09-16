#include <gtest/gtest.h>
#include <chrono>
#include <memory>
#include <vector>
#include <iostream>
#include "../ptr/uniq_ptr.h"


struct Payload {
    uint64_t data[4];
    Payload() : data{1, 2, 3, 4} {}
};

//данная вещь называется фикстурой, когда я пишу TEST_F срабатывает специальный макрос и
//генерируется новый скрытый класс, который наследуется от SmartPointerBenchmark
//после завершения обьект уничтожается и это гарантирует нам изолированные друг от друга тесты
class SmartPointerBenchmark : public ::testing::Test {
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
TEST_F(SmartPointerBenchmark, MemoryOverhead) {
    EXPECT_EQ(sizeof(uniq_ptr<Payload>), sizeof(Payload*));
    EXPECT_EQ(sizeof(std::unique_ptr<Payload>), sizeof(Payload*));
    std::cout << "[ Memory ] uniq_ptr size: " << sizeof(uniq_ptr<Payload>) << " bytes\n";
    std::cout << "[ Memory ] std::unique_ptr size: " << sizeof(std::unique_ptr<Payload>) << " bytes\n";
    std::cout << "[ Memory ] pointer size: " << sizeof(Payload*) << " bytes\n";
}

//тестирование на малом числе объектов
TEST_F(SmartPointerBenchmark, SmallScaleAllocations) {
    const size_t COUNT = 10'000;

    RunBenchmark("Pointers (Small)", COUNT, [](size_t count) {
        std::vector<Payload*> vec;
        vec.reserve(count);
        for (size_t i = 0; i < count; ++i) vec.push_back(new Payload());
        for (auto p : vec) delete p;
    });

    RunBenchmark("std::unique_ptr (Small)", COUNT, [](size_t count) {
        std::vector<std::unique_ptr<Payload>> vec;
        vec.reserve(count);
        for (size_t i = 0; i < count; ++i) vec.push_back(std::make_unique<Payload>());
    });

    RunBenchmark("My uniq_ptr (Small)", COUNT, [](size_t count) {
        std::vector<uniq_ptr<Payload>> vec;
        vec.reserve(count);
        for (size_t i = 0; i < count; ++i) vec.push_back(uniq_ptr<Payload>(new Payload()));
    });
}

//тестирование на большом числе объектов
TEST_F(SmartPointerBenchmark, LargeScaleAllocations) {
    const size_t COUNT = 10'000'000;

    RunBenchmark("Pointers (Big)", COUNT, [](size_t count) {
        std::vector<Payload*> vec;
        vec.reserve(count);
        for (size_t i = 0; i < count; ++i) vec.push_back(new Payload());
        for (auto p : vec) delete p;
    });

    RunBenchmark("std::unique_ptr (Big)", COUNT, [](size_t count) {
        std::vector<std::unique_ptr<Payload>> vec;
        vec.reserve(count);
        for (size_t i = 0; i < count; ++i) vec.push_back(std::make_unique<Payload>());
    });

    RunBenchmark("My uniq_ptr (Big)", COUNT, [](size_t count) {
        std::vector<uniq_ptr<Payload>> vec;
        vec.reserve(count);
        for (size_t i = 0; i < count; ++i) vec.push_back(uniq_ptr<Payload>(new Payload()));
    });
}