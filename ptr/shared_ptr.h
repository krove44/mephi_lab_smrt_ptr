#pragma once
#include "control_block.h"
#include <type_traits>
#include <utility>
template <class X>
struct DefaultDelete {
    void operator()(X* p) const {
        delete p;
    }
};

template <typename T> class weak_ptr;

template <typename T>
class shared_ptr {
private:
    T* ptr_;
    base_control_block* block_;
    template <class U> friend class shared_ptr;
    template <class U> friend class weak_ptr;
    shared_ptr(T* ptr, base_control_block* b) : ptr_(ptr), block_(b) {};
public:
    shared_ptr() : ptr_(nullptr), block_(nullptr) {}

    template <typename X, typename = std::enable_if_t<std::is_convertible_v<X*, T*>>>
    shared_ptr(X* ptr) : ptr_(ptr), block_(new regular_control_block<X, DefaultDelete<X>>(ptr, DefaultDelete<X>{})){};

    template <typename X, typename Deleter, typename = std::enable_if_t<std::is_convertible_v<X*, T*>>>
    shared_ptr(X* ptr, Deleter del) : ptr_(ptr), block_(new regular_control_block<X, Deleter>(ptr, del)) {}

    template <typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
    shared_ptr(const shared_ptr<U>& other) : ptr_(other.ptr_), block_(other.block_) {
        if (block_) {
            block_->add_strong();
        };
    }
    shared_ptr(const shared_ptr<T>& other) : ptr_(other.ptr_), block_(other.block_){
        if (block_) {
            block_->add_strong();
        }
    }

    template <typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
    shared_ptr(shared_ptr<U>&& other) : ptr_(other.ptr_), block_(other.block_) {
        other.ptr_ = nullptr;
        other.block_ = nullptr;
    }
    shared_ptr(shared_ptr<T>&& other) : ptr_(other.ptr_), block_(other.block_) {
        other.ptr_ = nullptr;
        other.block_ = nullptr;
    }

    template <typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
    shared_ptr& operator=(const shared_ptr<U>& other) {
        return *this = shared_ptr<T>(other);
    };
    shared_ptr<T>& operator=(const shared_ptr<T>& other) {
        if (&other == this) return *this;
        ptr_ = other.ptr_;
        base_control_block* b = other.block_;
        if (b) b->add_strong();
        if (block_) block_->release_strong();
        block_ = b;
        return *this;
    }

    template <typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
    shared_ptr& operator=(shared_ptr<U>&& other) {
        return *this = shared_ptr<T>(std::move(other));
    };
    shared_ptr<T>& operator=(shared_ptr<T>&& other) {
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

    T& operator*() const {
        return *ptr_;
    }

    std::size_t use_count() const {
        if (block_) {
            return block_->use_count();
        }
        return 0;
    };

    std::size_t use_count_weak() const {
        if (block_) {
            return block_->use_count_weak();
        }
        return 0;
    }

    T* get() const {
        return ptr_;
    }

    T* operator->() const{
        return ptr_;
    }
};
