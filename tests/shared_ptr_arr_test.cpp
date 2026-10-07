#include "common.h"
#include "../ptr/shared_ptr_arr.h"

TEST(SharedPtrArr, EmptyPointer) {
    shared_ptr<int[]> p;
    EXPECT_EQ(p.get(), nullptr);
    EXPECT_EQ(p.use_count(), 0u);
}

TEST(SharedPtrArr, ConstructFromRaw) {
    shared_ptr<int[]> p(new int[3]{1, 2, 3});
    EXPECT_EQ(p.use_count(), 1u);
    EXPECT_EQ(p.use_count_weak(), 1u);
}

TEST(SharedPtrArr, IndexOperator) {
    shared_ptr<int[]> p(new int[3]{10, 20, 30});
    EXPECT_EQ(p[0], 10);
    EXPECT_EQ(p[2], 30);
    p[1] = 99;
    EXPECT_EQ(p[1], 99);
}

TEST(SharedPtrArr, CopyIncrementsCount) {
    shared_ptr<int[]> a(new int[2]{1, 2});
    {
        shared_ptr<int[]> b = a;
        EXPECT_EQ(a.use_count(), 2u);
        EXPECT_EQ(a.get(), b.get());
        shared_ptr<int[]> c;
        c = b;
        EXPECT_EQ(a.use_count(), 3u);
    }
    EXPECT_EQ(a.use_count(), 1u);
}

TEST(SharedPtrArr, MoveEmptiesSource) {
    shared_ptr<int[]> a(new int[2]{5, 6});
    shared_ptr<int[]> b = std::move(a);
    EXPECT_EQ(a.get(), nullptr);
    EXPECT_EQ(a.use_count(), 0u);
    EXPECT_EQ(b[1], 6);
    EXPECT_EQ(b.use_count(), 1u);

    shared_ptr<int[]> c(new int[1]{7});
    c = std::move(b);
    EXPECT_EQ(b.get(), nullptr);
    EXPECT_EQ(c[1], 6);
}

TEST(SharedPtrArr, AllElementsDestroyed) {
    Tracked::reset();
    {
        shared_ptr<Tracked[]> a(new Tracked[5]);
        shared_ptr<Tracked[]> b = a;
        EXPECT_EQ(Tracked::alive, 5);
    }
    EXPECT_EQ(Tracked::alive, 0);
    EXPECT_EQ(Tracked::destroyed, 5);
}

TEST(SharedPtrArr, AssignmentReleasesOld) {
    Tracked::reset();
    shared_ptr<Tracked[]> a(new Tracked[2]);
    shared_ptr<Tracked[]> b(new Tracked[3]);
    EXPECT_EQ(Tracked::alive, 5);
    b = a;                               // старый массив b должен умереть
    EXPECT_EQ(Tracked::alive, 2);
    EXPECT_EQ(a.use_count(), 2u);
}

TEST(SharedPtrArr, SelfAssignment) {
    Tracked::reset();
    shared_ptr<Tracked[]> a(new Tracked[2]);
    shared_ptr<Tracked[]>& ref = a;
    a = ref;
    EXPECT_EQ(a.use_count(), 1u);
    a = std::move(ref);
    EXPECT_EQ(a.use_count(), 1u);
    EXPECT_EQ(Tracked::alive, 2);
}

TEST(SharedPtrArr, CustomDeleter) {
    int calls = 0;
    {
        shared_ptr<int[]> p(new int[3], [&calls](int* x) { ++calls; delete[] x; });
        shared_ptr<int[]> q = p;
        EXPECT_EQ(calls, 0);
    }
    EXPECT_EQ(calls, 1);
}