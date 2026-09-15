#pragma once


template <typename T>
class smart_ptr {
private:
    T* ptr;
    size_t* counter;
public:
    explicit smart_ptr() : ptr(nullptr), counter(nullptr) {}

    smart_ptr(T* ptr) : ptr(ptr), counter(new size_t(1)) {}

    template <typename U>
    smart_ptr(const smart_ptr<U>& other) : ptr(other.get()), counter(other.get_counter()) {
        if (counter) {
            ++*counter;
        }
    }

    smart_ptr(smart_ptr<T>& other) : ptr(other.ptr), counter(other.counter) {
        if (counter) {
            ++*counter;
        }
    }

    ~smart_ptr() {
        if (counter && --*counter == 0) {
            delete ptr;
            delete counter;
        }
        ptr = nullptr;
        counter = nullptr;
    }

    smart_ptr<T>&  operator=(smart_ptr<T>& other) {
        if (this != &other) {
            this->~smart_ptr();
            this->ptr = other.ptr;
            this->counter = other.counter;
            if (counter) ++*counter;

        }
        return *this;
    }

    template <typename U>
    smart_ptr<T>& operator=(smart_ptr<U>& other) {
        this->~smart_ptr();
        ptr = other.get();
        counter = other.get_counter();
        if (counter) ++*counter;
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

    friend std::ostream& operator<<(std::ostream& os, const smart_ptr<T>& sp) {
        os << sp.ptr;
        return os;
    }

};