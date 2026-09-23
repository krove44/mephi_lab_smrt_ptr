#pragma once
#include "control_block.h"

template <typename T>
class shared_ptr {
private:
    control_block<T>* block_;
public:
    explicit shared_ptr() : block_() {}
    shared_ptr(T* ptr) : block_(new control_block<T>(ptr)) {}
    shared_ptr(const shared_ptr<T>& other) : block_(other.block_) {
        block_->operator++();
    }
    shared_ptr(shared_ptr<T>&& other) : block_(other.block_) {
        other.block_ = nullptr;
    }

    ~shared_ptr() {
        if (block_ && block_->operator--()) delete block_;
    }

    shared_ptr<T>&  operator=(const shared_ptr<T>& other) {
        if (this != &other) {
            delete block_;
            block_ = other.block_;
            block_->operator++();
        }
        return *this;
    }

    T& operator*() {
        return block_->operator*();
    }

    T* operator->() {
        return block_->operator->();
    }

    T* get() const {
        return block_->get();
    }

    size_t use_count() const {
        return block_->use_count();
    }

    friend std::ostream& operator<<(std::ostream& os, const shared_ptr<T>& sp) {
        os << sp.block_->operator*();
        return os;
    }

};


// template <typename T>
// class shared_ptr<T[]> {
// private:
//     T* ptr;
//     size_t* counter;
//     void clear() {
//         if (counter && --*counter == 0) {
//             delete[] ptr;
//             delete counter;
//         }
//     }
// public:
//     explicit shared_ptr() : ptr(nullptr), counter(nullptr) {}
//     shared_ptr(T* ptr) : ptr(ptr), counter(new size_t(1)) {}
//     shared_ptr(const shared_ptr<T[]>& other) : ptr(other.ptr), counter(other.counter) {
//         if (counter) ++*counter;
//     }
//
//     ~shared_ptr() {
//         clear();
//         ptr = nullptr;
//         counter = nullptr;
//     }
//
//     shared_ptr<T[]>& operator=(const shared_ptr<T[]>& other) {
//         if (this != &other) {
//             clear();
//             this->ptr = other.ptr;
//             this->counter = other.counter;
//             if (counter) ++*counter;
//         }
//         return *this;
//     }
//
//     T& operator[](size_t i) { return ptr[i]; }
//     const T& operator[](size_t i) const { return ptr[i]; }
//
//     T* get() const { return ptr; }
//     size_t* get_counter() const { return counter; }
//     size_t use_count() const { return counter ? *counter : 0; }
//
//     friend std::ostream& operator<<(std::ostream& os, const shared_ptr<T[]>& sp) {
//         os << sp.ptr;
//         return os;
//     }
// };