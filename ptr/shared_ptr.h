#pragma once
#include "uniq_ptr.h"
#include <cstddef>

template <typename T>
class shared_ptr {
private:
    uniq_ptr<T>* ptr;
    size_t* counter;
    void clear() {
        if (counter && --*counter == 0) {
            delete ptr;
            delete counter;
        }
    }
public:
    shared_ptr() : ptr(nullptr), counter(nullptr) {}
    shared_ptr(uniq_ptr<T>* ptr) : ptr(ptr), counter(new size_t(1)) {}
    shared_ptr(const shared_ptr& other) : ptr(other.ptr), counter(other.counter) {
        if (counter) ++*counter;
    }
    ~shared_ptr() {
        clear();
        ptr = nullptr;
        counter = nullptr;
    }

    shared_ptr<T>&  operator=(const shared_ptr<T>& other) {
        if (this != &other || counter == other.counter) {
            clear();
            ptr = other.ptr;
            counter = other.counter;
            if (counter) ++*counter;
        }
        return *this;
    }

    T& operator*() {return *(*ptr);}
    uniq_ptr<T> operator->() {return *ptr;}
    uniq_ptr<T> get() const {return *ptr;}
    size_t* get_counter() const {return counter;}
    size_t use_count() const {return counter ? *counter : 0;}

};
