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
        if (block_) block_->operator++();
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


template <typename T>
class shared_ptr<T[]> {
private:
    control_block<T[]>* block_;

    void release() {
        if (block_ && block_->operator--()) delete block_;
        block_ = nullptr;
    }
public:
    shared_ptr() : block_(nullptr) {}
    explicit shared_ptr(T* ptr) : block_(ptr ? new control_block<T[]>(ptr) : nullptr) {}
    shared_ptr(const shared_ptr<T[]>& other) : block_(other.block_) {
        if (block_) block_->operator++();
    }
    shared_ptr(shared_ptr<T[]>&& other) noexcept : block_(other.block_) {
        other.block_ = nullptr;
    }

    ~shared_ptr() { release(); }

    shared_ptr<T[]>& operator=(const shared_ptr<T[]>& other) {
        if (this != &other) {
            release();
            block_ = other.block_;
            if (block_) block_->operator++();
        }
        return *this;
    }

    shared_ptr<T[]>& operator=(shared_ptr<T[]>&& other) noexcept {
        if (this != &other) {
            release();
            block_ = other.block_;
            other.block_ = nullptr;
        }
        return *this;
    }

    T& operator[](size_t i) { return (*block_)[i]; }
    T* get() const { return block_ ? block_->get() : nullptr; }
    size_t use_count() const { return block_ ? block_->use_count() : 0; }
};