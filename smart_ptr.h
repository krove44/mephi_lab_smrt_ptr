#pragma once


template <typename T>
class smart_ptr {
private:
    T* ptr;

public:
    explicit smart_ptr(T* ptr) : ptr(ptr) {}
    ~smart_ptr() {
        delete ptr;
    }

    T& operator*() {
        return *ptr;
    }

    friend std::ostream& operator<<(std::ostream& os, const smart_ptr<T>& sp) {
        os << sp.ptr;
        return os;
    }

};