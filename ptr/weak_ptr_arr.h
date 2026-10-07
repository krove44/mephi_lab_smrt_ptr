#pragma once
#include "weak_ptr.h"
#include "shared_ptr_arr.h"
#include <utility>

template <typename T>
class weak_ptr<T[]> {
    T* ptr_;
    base_control_block* block_;
public:
    weak_ptr() : ptr_(nullptr), block_(nullptr) {}

    explicit weak_ptr(const shared_ptr<T[]>& other) : ptr_(other.ptr_), block_(other.block_) {
        if (block_) block_->add_weak();
    }

    weak_ptr(const weak_ptr<T[]>& other) : ptr_(other.ptr_), block_(other.block_) {
        if (block_) block_->add_weak();
    }
    weak_ptr(weak_ptr<T[]>&& other) : ptr_(other.ptr_), block_(other.block_) {
        other.ptr_ = nullptr;
        other.block_ = nullptr;
    }

    weak_ptr& operator=(const weak_ptr<T[]>& other) {
        if (&other == this) return *this;
        ptr_ = other.ptr_;
        base_control_block* b = other.block_;
        if (b) b->add_weak();
        if (block_) block_->release_weak();
        block_ = b;
        return *this;
    }
    weak_ptr& operator=(weak_ptr<T[]>&& other) {
        if (&other == this) return *this;
        ptr_ = other.ptr_;
        base_control_block* b = other.block_;
        other.ptr_ = nullptr;
        other.block_ = nullptr;
        if (block_) block_->release_weak();
        block_ = b;
        return *this;
    }

    ~weak_ptr() {
        if (block_) block_->release_weak();
    }

    bool expired() const {
        return block_ == nullptr || block_->use_count() == 0;
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

    shared_ptr<T[]> lock() const {
        if (block_ && block_->try_add_strong()) {
            return shared_ptr<T[]>(ptr_, block_);
        }
        return shared_ptr<T[]>();
    }
};