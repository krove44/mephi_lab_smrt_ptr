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

TEST(smart_ptr, base_pointer_test) {
    smart_ptr<int> ptr = new int(42);
    EXPECT_EQ(*ptr, 42);
}

TEST(smart_ptr, default_constructor_is_empty) {
    smart_ptr<int> ptr;
    EXPECT_EQ(ptr.use_count(), 0);
    EXPECT_EQ(ptr.get(), nullptr);
}

TEST(smart_ptr, copy_increases_counter) {
    smart_ptr<int> a = new int(10);
    EXPECT_EQ(a.use_count(), 1);

    smart_ptr<int> b = a;
    EXPECT_EQ(a.use_count(), 2);
    EXPECT_EQ(b.use_count(), 2);
    EXPECT_EQ(*b, 10);
}

TEST(smart_ptr, destructor_decreases_counter) {
    smart_ptr<int> a = new int(10);
    {
        smart_ptr<int> b = a;
        EXPECT_EQ(a.use_count(), 2);
    }
    EXPECT_EQ(a.use_count(), 1);
}

TEST(smart_ptr, assignment_releases_old_object) {
    smart_ptr<int> a = new int(1);
    smart_ptr<int> b = new int(2);
    smart_ptr<int> c = b;

    a = b;
    EXPECT_EQ(*a, 2);
    EXPECT_EQ(b.use_count(), 3);
}

TEST(smart_ptr, self_assignment_is_safe) {
    smart_ptr<int> a = new int(5);
    a = a;
    EXPECT_EQ(*a, 5);
    EXPECT_EQ(a.use_count(), 1);
}

TEST(smart_ptr, arrow_operator_works) {
    struct Point { int x = 1, y = 2; };
    smart_ptr<Point> p = new Point();
    EXPECT_EQ(p->x, 1);
    EXPECT_EQ(p->y, 2);
}




