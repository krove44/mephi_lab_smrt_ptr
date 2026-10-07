#include "common.h"
#include "../ptr/weak_ptr_arr.h"

TEST(WeakPtrArr, EmptyWeak) {
    weak_ptr<int[]> w;
    EXPECT_TRUE(w.expired());
    EXPECT_EQ(w.use_count(), 0u);
    EXPECT_EQ(w.lock().get(), nullptr);
    weak_ptr<int[]> w2 = w;
    weak_ptr<int[]> w3 = std::move(w2);
}

TEST(WeakPtrArr, WeakDoesNotAffectStrongCount) {
    shared_ptr<int[]> p(new int[3]);
    EXPECT_EQ(p.use_count_weak(), 1u);
    {
        weak_ptr<int[]> w(p);
        EXPECT_EQ(p.use_count(), 1u);
        EXPECT_EQ(p.use_count_weak(), 2u);
    }
    EXPECT_EQ(p.use_count_weak(), 1u);
}

TEST(WeakPtrArr, LockWhileAlive) {
    shared_ptr<int[]> p(new int[3]{1, 2, 3});
    weak_ptr<int[]> w(p);
    EXPECT_FALSE(w.expired());
    {
        shared_ptr<int[]> s = w.lock();
        EXPECT_EQ(s.get(), p.get());
        EXPECT_EQ(s[2], 3);
        EXPECT_EQ(p.use_count(), 2u);
    }
    EXPECT_EQ(p.use_count(), 1u);
}

TEST(WeakPtrArr, ExpiresAfterLastSharedDies) {
    Tracked::reset();
    weak_ptr<Tracked[]> w;
    {
        shared_ptr<Tracked[]> p(new Tracked[4]);
        w = weak_ptr<Tracked[]>(p);
        EXPECT_FALSE(w.expired());
    }
    EXPECT_TRUE(w.expired());
    EXPECT_EQ(Tracked::alive, 0);            // weak не удерживает массив
    EXPECT_EQ(w.lock().get(), nullptr);
}

TEST(WeakPtrArr, BlockOutlivesArray) {
    weak_ptr<int[]> w;
    {
        shared_ptr<int[]> p(new int[3]);
        w = weak_ptr<int[]>(p);
    }
    EXPECT_EQ(w.use_count(), 0u);
    EXPECT_EQ(w.use_count_weak(), 1u);
}

TEST(WeakPtrArr, CopyAndMove) {
    shared_ptr<int[]> p(new int[2]);
    weak_ptr<int[]> w1(p);
    weak_ptr<int[]> w2 = w1;
    EXPECT_EQ(p.use_count_weak(), 3u);
    weak_ptr<int[]> w3 = std::move(w2);
    EXPECT_EQ(p.use_count_weak(), 3u);
    EXPECT_TRUE(w2.expired());
    weak_ptr<int[]> w4;
    w4 = w1;
    EXPECT_EQ(p.use_count_weak(), 4u);
    w4 = std::move(w3);
    EXPECT_EQ(p.use_count_weak(), 3u);
}

TEST(WeakPtrArr, SelfAssignment) {
    shared_ptr<int[]> p(new int[2]);
    weak_ptr<int[]> w(p);
    weak_ptr<int[]>& ref = w;
    w = ref;
    EXPECT_EQ(p.use_count_weak(), 2u);
}