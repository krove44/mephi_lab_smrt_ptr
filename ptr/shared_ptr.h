#pragma once

template <typename T>
class shared_ptr {
private:
    T* ptr;
    size_t* counter;
    void clear() {
        if (counter && --*counter == 0) {
            delete ptr;
            delete counter;
        }
    }
public:
    explicit shared_ptr() : ptr(nullptr), counter(nullptr) {}
    shared_ptr(T* ptr) : ptr(ptr), counter(new size_t(1)) {}
    shared_ptr(const shared_ptr<T>& other) : ptr(other.ptr), counter(other.counter) {
        if (counter) ++*counter;
    }

    ~shared_ptr() {
        clear();
        ptr = nullptr;
        counter = nullptr;
    }

    shared_ptr<T>&  operator=(const shared_ptr<T>& other) {
        if (this != &other) {
            clear();
            this->ptr = other.ptr;
            this->counter = other.counter;
            if (counter) ++*counter;

        }
        return *this;
    }

    T& operator*() {
        return *ptr;
    }

    const T& operator*() const {
        return *ptr;
    }

    T* operator->() {
        return ptr;
    }

    T* get() const {
        return ptr;
    }

    size_t* get_counter() const {
        return counter;
    }

    size_t use_count() const {
        return counter ? *counter : 0;
    }

    friend std::ostream& operator<<(std::ostream& os, const shared_ptr<T>& sp) {
        os << sp.ptr;
        return os;
    }

};