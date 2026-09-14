#pragma once


template <typename T>
class smart_ptr {
private:
    T* ptr;
    size_t* counter;
public:
    explicit smart_ptr() : ptr(nullptr), counter(nullptr) {}

    explicit smart_ptr(T* ptr) : ptr(ptr), counter(new size_t(1)) {}

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

    const smart_ptr<T>&  operator=(smart_ptr<T>& other) {
        if (this != &other) {
            this->~smart_ptr();
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

    friend std::ostream& operator<<(std::ostream& os, const smart_ptr<T>& sp) {
        os << sp.ptr;
        return os;
    }
    
};