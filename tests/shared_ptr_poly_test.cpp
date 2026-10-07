#include "common.h"

struct A { int a = 1; virtual ~A() = default; };
struct B { int b = 2; virtual ~B() = default; };
struct C : A, B { int c = 3; };

TEST(SharedPtrPoly, DerivedDestroyedThroughNonVirtualBase) {
    NVDerived::destroyed = 0;
    {
        shared_ptr<NVBase> b(new NVDerived());
        shared_ptr<NVBase> b2 = b;
        EXPECT_EQ(b.use_count(), 2u);
    }
    EXPECT_EQ(NVDerived::destroyed, 1);  // ~NVDerived обязан вызваться
}

TEST(SharedPtrPoly, ConvertingCopyConstructor) {
    NVDerived::destroyed = 0;
    {
        shared_ptr<NVDerived> d(new NVDerived());
        shared_ptr<NVBase> b = d;
        EXPECT_EQ(d.use_count(), 2u);
        EXPECT_EQ(b.use_count(), 2u);
        EXPECT_EQ(b->x, 1);
    }
    EXPECT_EQ(NVDerived::destroyed, 1);
}

TEST(SharedPtrPoly, ConvertingMoveConstructor) {
    NVDerived::destroyed = 0;
    {
        shared_ptr<NVDerived> d(new NVDerived());
        shared_ptr<NVBase> b = std::move(d);
        EXPECT_EQ(d.get(), nullptr);
        EXPECT_EQ(b.use_count(), 1u);
    }
    EXPECT_EQ(NVDerived::destroyed, 1);
}

TEST(SharedPtrPoly, ConvertingAssignments) {
    NVDerived::destroyed = 0;
    {
        shared_ptr<NVDerived> d(new NVDerived());
        shared_ptr<NVBase> b;
        b = d;                               // converting copy =
        EXPECT_EQ(d.use_count(), 2u);

        shared_ptr<NVBase> b2;
        b2 = std::move(d);                   // converting move =
        EXPECT_EQ(d.get(), nullptr);
        EXPECT_EQ(b2.use_count(), 2u);
    }
    EXPECT_EQ(NVDerived::destroyed, 1);
}

TEST(SharedPtrPoly, AssignEmptyDerived) {
    shared_ptr<NVDerived> empty;
    shared_ptr<NVBase> b(new NVDerived());
    b = empty;
    EXPECT_EQ(b.get(), nullptr);
}

TEST(SharedPtrPoly, MultipleInheritanceAdjustsPointer) {
    C* raw = new C();
    shared_ptr<C> c(raw);
    shared_ptr<B> b = c;                     // адрес B-подобъекта сдвинут
    EXPECT_EQ(b.get(), static_cast<B*>(raw));
    EXPECT_EQ(b->b, 2);
    EXPECT_EQ(c.use_count(), 2u);
}

TEST(SharedPtrPoly, SharedBlockBetweenBases) {
    shared_ptr<C> c(new C());
    shared_ptr<A> a = c;
    shared_ptr<B> b = c;
    EXPECT_EQ(c.use_count(), 3u);
    EXPECT_EQ(a->a, 1);
    EXPECT_EQ(b->b, 2);
}

// Проверки на этапе компиляции (SFINAE)
static_assert(std::is_convertible_v<shared_ptr<NVDerived>, shared_ptr<NVBase>>);
static_assert(!std::is_convertible_v<shared_ptr<NVBase>, shared_ptr<NVDerived>>);
static_assert(!std::is_convertible_v<shared_ptr<int>, shared_ptr<std::string>>);
static_assert(!std::is_constructible_v<shared_ptr<NVDerived>, NVBase*>);