#pragma once


template <typename T>
class uniq_ptr {
private:
    T* ptr_;
public:
    explicit uniq_ptr(T* ptr = nullptr) : ptr_(ptr) {}

    uniq_ptr(const uniq_ptr&) = delete;
    uniq_ptr& operator=(const uniq_ptr&) = delete;

    uniq_ptr(uniq_ptr&& other) noexcept : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    ~uniq_ptr() {
        delete ptr_;
        ptr_ = nullptr;
    }

    uniq_ptr& operator=(uniq_ptr&& other) noexcept {
        if (this != &other) {
            delete ptr_;
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }

    T& operator*() const {return *ptr_;}
    T* operator->() const {return ptr_;}
    T* get() const {return ptr_;}

};