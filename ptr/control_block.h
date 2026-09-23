#pragma once
#include <cstddef>


template <typename T>
class control_block {
private:
    T* ptr;
    size_t counter;
public:
    control_block() : ptr(nullptr), counter(0) {}
    control_block(T* ptr, size_t counter = 1) : ptr(ptr), counter(counter) {}
    control_block(const control_block&) = delete;
    control_block& operator=(const control_block&) = delete;
    ~control_block() {
        if (ptr) delete ptr;
        ptr = nullptr;
        counter = 0;
    };
    void operator++() {
        counter++;
    }
    bool operator--() {
        return --counter == 0;
    }
    T& operator*() {return *ptr;}
    T* get() {return ptr;};
    T* operator->() {return ptr;}
    size_t use_count() {return counter;}

};

template <typename T>
class control_block<T[]> {
private:
    T* ptr;
    size_t counter;
public:
    control_block() : ptr(nullptr), counter(0) {}
    control_block(T* ptr, size_t counter = 1) : ptr(ptr), counter(counter) {}
    control_block(const control_block&) = delete;
    control_block& operator=(const control_block&) = delete;
    ~control_block() {
        delete[] ptr;
        ptr = nullptr;
        counter = 0;
    }
    void operator++() {
        ++counter;
    }
    bool operator--() {
        return --counter == 0;
    }
    T& operator[](size_t i) { return ptr[i]; }
    T* get() const { return ptr; }
    size_t use_count() const { return counter; }
};