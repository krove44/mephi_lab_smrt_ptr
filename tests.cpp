#include <gtest/gtest.h>
#include "ptr/smart_ptr.h"
struct Base {
    virtual int value() const { return 1; }
    virtual ~Base() = default;
};

struct Derived : Base {
    int value() const override { return 2; }
};

TEST(smart_ptr, subtyping_conversion_works) {
    smart_ptr<Derived> d(new Derived());
    smart_ptr<Base> b = d;

    EXPECT_EQ(b->value(), 2);
    EXPECT_EQ(d.use_count(), 2);
    EXPECT_EQ(b.use_count(), 2);
    EXPECT_EQ(b.get(), d.get());
}

TEST(smart_ptr, subtyping_counter_drops_on_destruction) {
    smart_ptr<Derived> d(new Derived());
    {
        smart_ptr<Base> b = d;
        EXPECT_EQ(d.use_count(), 2);
    }
    EXPECT_EQ(d.use_count(), 1);
}
