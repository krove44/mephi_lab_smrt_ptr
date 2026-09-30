#pragma once
#include <cstddef>

struct base_control_block {
    size_t strong{1};
    size_t weak{1};
    virtual ~base_control_block() = default;
    virtual void destroy_obj() = 0;
    virtual void destroy_block()  = 0;

    void add_strong() {
        ++strong;
    }

    void add_weak() {
        ++weak;
    }

    void release_strong() {
        strong--;
        if (strong == 0) {
            destroy_obj();
            release_weak();
        }
    }
    void release_weak() {
        weak--;
        if (weak == 0) {
            destroy_block();
        }
    }
    bool try_add_strong() {
        if (strong == 0) {
            return false;
        }
        ++strong;
        return true;
    }

    std::size_t use_count() const {
        return strong;
    }

};


template<typename U, typename Deleter>
struct regular_control_block : public base_control_block {
    U* ptr;
    Deleter deleter;
    regular_control_block(U* ptr, Deleter del) : ptr(ptr), deleter(del) {}
    void destroy_obj() override {
        deleter(ptr);
    }
    void destroy_block() override {
        delete this;
    }
};