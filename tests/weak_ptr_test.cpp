#include "common.h"

struct Parent;
struct Child {
    weak_ptr<Parent> parent;
    Tracked t;
};
struct Parent {
    shared_ptr<Child> child;
    Tracked t;
};

TEST(WeakPtr, EmptyWeak) {
    weak_ptr<int> w;
    EXPECT_TRUE(w.expired());
    EXPECT_EQ(w.use_count(), 0u);
    EXPECT_EQ(w.lock().get(), nullptr);
    weak_ptr<int> w2 = w;                    // копия пустого
    weak_ptr<int> w3 = std::move(w2);
}

TEST(WeakPtr, WeakDoesNotAffectStrongCount) {
    shared_ptr<Payload> p(new Payload());
    EXPECT_EQ(p.use_count_weak(), 1u);
    {
        weak_ptr<Payload> w(p);
        EXPECT_EQ(p.use_count(), 1u);
        EXPECT_EQ(p.use_count_weak(), 2u);
    }
    EXPECT_EQ(p.use_count_weak(), 1u);
}

TEST(WeakPtr, LockWhileAlive) {
    shared_ptr<Payload> p(new Payload());
    weak_ptr<Payload> w(p);
    EXPECT_FALSE(w.expired());
    {
        shared_ptr<Payload> s = w.lock();
        EXPECT_EQ(s.get(), p.get());
        EXPECT_EQ(p.use_count(), 2u);
    }
    EXPECT_EQ(p.use_count(), 1u);
}

TEST(WeakPtr, ExpiresAfterLastSharedDies) {
    Tracked::reset();
    weak_ptr<Tracked> w;
    {
        shared_ptr<Tracked> p(new Tracked());
        w = weak_ptr<Tracked>(p);
        EXPECT_FALSE(w.expired());
    }
    EXPECT_TRUE(w.expired());
    EXPECT_EQ(Tracked::alive, 0);            // weak не удерживает объект
    EXPECT_EQ(w.lock().get(), nullptr);
    EXPECT_EQ(w.lock().use_count(), 0u);
}

TEST(WeakPtr, BlockOutlivesObject) {
    // блок должен жить, пока жив weak (под ASan поймает use-after-free)
    weak_ptr<Payload> w;
    {
        shared_ptr<Payload> p(new Payload());
        w = weak_ptr<Payload>(p);
    }
    EXPECT_EQ(w.use_count(), 0u);
    EXPECT_EQ(w.use_count_weak(), 1u);
}

TEST(WeakPtr, ResetSharedViaAssignment) {
    shared_ptr<Payload> d(new Payload());
    weak_ptr<Payload> w(d);
    d = shared_ptr<Payload>();
    EXPECT_TRUE(w.expired());
}

TEST(WeakPtr, CopyAndMove) {
    shared_ptr<int> p(new int(1));
    weak_ptr<int> w1(p);
    weak_ptr<int> w2 = w1;
    EXPECT_EQ(p.use_count_weak(), 3u);
    weak_ptr<int> w3 = std::move(w2);
    EXPECT_EQ(p.use_count_weak(), 3u);
    EXPECT_TRUE(w2.expired());               // перемещённый пуст
    weak_ptr<int> w4;
    w4 = w1;
    EXPECT_EQ(p.use_count_weak(), 4u);
    w4 = std::move(w3);
    EXPECT_EQ(p.use_count_weak(), 3u);
}

TEST(WeakPtr, SelfAssignment) {
    shared_ptr<int> p(new int(1));
    weak_ptr<int> w(p);
    weak_ptr<int>& ref = w;
    w = ref;
    EXPECT_EQ(p.use_count_weak(), 2u);
}

TEST(WeakPtr, ConvertingToBase) {
    NVDerived::destroyed = 0;
    weak_ptr<NVBase> w;
    {
        shared_ptr<NVDerived> d(new NVDerived());
        weak_ptr<NVDerived> wd(d);
        w = wd;                              // converting copy =
        weak_ptr<NVBase> w2(wd);             // converting ctor
        weak_ptr<NVBase> w3(d);              // из shared_ptr<Derived>
        EXPECT_EQ(d.use_count_weak(), 5u);
        shared_ptr<NVBase> locked = w.lock();
        EXPECT_EQ(locked->x, 1);
        EXPECT_EQ(d.use_count(), 2u);
    }
    EXPECT_EQ(NVDerived::destroyed, 1);
    EXPECT_TRUE(w.expired());
}

TEST(WeakPtr, BreaksOwnershipCycle) {
    Tracked::reset();
    {
        shared_ptr<Parent> p(new Parent());
        p->child = shared_ptr<Child>(new Child());
        p->child->parent = weak_ptr<Parent>(p);
        EXPECT_EQ(p.use_count(), 1u);        // child держит parent слабо
        EXPECT_EQ(p->child.use_count(), 1u);
        EXPECT_EQ(Tracked::alive, 2);
    }
    EXPECT_EQ(Tracked::alive, 0);            // оба объекта удалены, утечки нет
}