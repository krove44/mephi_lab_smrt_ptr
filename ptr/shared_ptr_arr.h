#pragma once
#include "shared_ptr.h"
#include <utility>

template <class X>
struct DefaultDeleteArr {
    void operator()(X* p) const {
        delete[] p;
    }
};

template <typename T>
class shared_ptr<T[]> {
private:
    T* ptr_;
    base_control_block* block_;
    template <class U> friend class weak_ptr;
    shared_ptr(T* ptr, base_control_block* b) : ptr_(ptr), block_(b) {}
public:
    shared_ptr() : ptr_(nullptr), block_(nullptr) {}

    shared_ptr(T* ptr) : ptr_(ptr), block_(new regular_control_block<T, DefaultDeleteArr<T>>(ptr, DefaultDeleteArr<T>{})) {}

    template <typename Deleter>
    shared_ptr(T* ptr, Deleter del) : ptr_(ptr), block_(new regular_control_block<T, Deleter>(ptr, del)) {}

    shared_ptr(const shared_ptr<T[]>& other) : ptr_(other.ptr_), block_(other.block_) {
        if (block_) {
            block_->add_strong();
        }
    }

    shared_ptr(shared_ptr<T[]>&& other) : ptr_(other.ptr_), block_(other.block_) {
        other.ptr_ = nullptr;
        other.block_ = nullptr;
    }

    shared_ptr<T[]>& operator=(const shared_ptr<T[]>& other) {
        if (&other == this) return *this;
        ptr_ = other.ptr_;
        base_control_block* b = other.block_;
        if (b) b->add_strong();
        if (block_) block_->release_strong();
        block_ = b;
        return *this;
    }

    shared_ptr<T[]>& operator=(shared_ptr<T[]>&& other) {
        if (&other == this) return *this;
        ptr_ = other.ptr_;
        base_control_block* b = other.block_;
        other.ptr_ = nullptr;
        other.block_ = nullptr;
        if (block_) block_->release_strong();
        block_ = b;
        return *this;
    }

    ~shared_ptr() {
        if (block_) {
            block_->release_strong();
        }
    }

    T& operator[](std::size_t i) const {
        return ptr_[i];
    }

    std::size_t use_count() const {
        if (block_) {
            return block_->use_count();
        }
        return 0;
    }

    std::size_t use_count_weak() const {
        if (block_) {
            return block_->use_count_weak();
        }
        return 0;
    }

    T* get() const {
        return ptr_;
    }
};