#include "common.h"

struct Node {
    shared_ptr<Node> next;
    Tracked t;
};

TEST(SharedPtrBasic, EmptyPointer) {
    shared_ptr<int> p;
    EXPECT_EQ(p.get(), nullptr);
    EXPECT_EQ(p.use_count(), 0u);
}

TEST(SharedPtrBasic, ConstructFromRaw) {
    shared_ptr<int> p(new int(42));
    EXPECT_EQ(*p, 42);
    EXPECT_EQ(p.use_count(), 1u);
    EXPECT_EQ(p.use_count_weak(), 1u);
}

TEST(SharedPtrBasic, ArrowOperator) {
    shared_ptr<Payload> p(new Payload());
    EXPECT_EQ(p->data[2], 3u);
}

TEST(SharedPtrBasic, CopyIncrementsCount) {
    shared_ptr<Payload> a(new Payload());
    {
        shared_ptr<Payload> b = a;
        EXPECT_EQ(a.use_count(), 2u);
        EXPECT_EQ(b.use_count(), 2u);
        EXPECT_EQ(a.get(), b.get());
        shared_ptr<Payload> c;
        c = b;
        EXPECT_EQ(a.use_count(), 3u);
    }
    EXPECT_EQ(a.use_count(), 1u);
}

TEST(SharedPtrBasic, CopyOfEmpty) {
    shared_ptr<int> a;
    shared_ptr<int> b = a;
    shared_ptr<int> c;
    c = a;
    EXPECT_EQ(b.get(), nullptr);
    EXPECT_EQ(c.use_count(), 0u);
}

TEST(SharedPtrBasic, MoveConstructorEmptiesSource) {
    shared_ptr<int> a(new int(7));
    shared_ptr<int> b = std::move(a);
    EXPECT_EQ(a.get(), nullptr);
    EXPECT_EQ(a.use_count(), 0u);
    EXPECT_EQ(*b, 7);
    EXPECT_EQ(b.use_count(), 1u);
}

TEST(SharedPtrBasic, MoveAssignmentEmptiesSource) {
    shared_ptr<int> a(new int(7));
    shared_ptr<int> b(new int(8));
    b = std::move(a);
    EXPECT_EQ(a.get(), nullptr);
    EXPECT_EQ(*b, 7);
    EXPECT_EQ(b.use_count(), 1u);
}

TEST(SharedPtrBasic, ObjectDestroyedExactlyOnce) {
    Tracked::reset();
    {
        shared_ptr<Tracked> a(new Tracked());
        shared_ptr<Tracked> b = a;
        shared_ptr<Tracked> c = std::move(b);
        EXPECT_EQ(Tracked::alive, 1);
    }
    EXPECT_EQ(Tracked::alive, 0);
    EXPECT_EQ(Tracked::destroyed, 1);
}

TEST(SharedPtrBasic, AssignmentReleasesOld) {
    Tracked::reset();
    shared_ptr<Tracked> a(new Tracked());
    shared_ptr<Tracked> b(new Tracked());
    EXPECT_EQ(Tracked::alive, 2);
    b = a;                               // старый объект b должен умереть
    EXPECT_EQ(Tracked::alive, 1);
    EXPECT_EQ(a.use_count(), 2u);
}

TEST(SharedPtrBasic, SelfAssignment) {
    Tracked::reset();
    shared_ptr<Tracked> a(new Tracked());
    shared_ptr<Tracked>& ref = a;
    a = ref;
    EXPECT_EQ(a.use_count(), 1u);
    a = std::move(ref);
    EXPECT_EQ(a.use_count(), 1u);
    EXPECT_EQ(Tracked::alive, 1);
}

TEST(SharedPtrBasic, AssignFromOwnMember) {
    // p = p->next: старый объект умирает прямо во время присваивания
    Tracked::reset();
    shared_ptr<Node> head(new Node());
    head->next = shared_ptr<Node>(new Node());
    EXPECT_EQ(Tracked::alive, 2);
    head = head->next;
    EXPECT_EQ(Tracked::alive, 1);
    EXPECT_EQ(head.use_count(), 1u);
}

TEST(SharedPtrBasic, MoveAssignFromOwnMember) {
    Tracked::reset();
    shared_ptr<Node> head(new Node());
    head->next = shared_ptr<Node>(new Node());
    head = std::move(head->next);
    EXPECT_EQ(Tracked::alive, 1);
    EXPECT_EQ(head.use_count(), 1u);
}

TEST(SharedPtrBasic, CustomDeleter) {
    int calls = 0;
    {
        shared_ptr<int> p(new int(5), [&calls](int* x) { ++calls; delete x; });
        shared_ptr<int> q = p;
        EXPECT_EQ(calls, 0);
    }
    EXPECT_EQ(calls, 1);
}

TEST(SharedPtrBasic, DeleterWithDifferentTypesSameSharedPtrType) {
    int a = 0, b = 0;
    shared_ptr<int> p1(new int(1), [&a](int* x) { ++a; delete x; });
    shared_ptr<int> p2(new int(2), [&b](int* x) { ++b; delete x; });
    p1 = p2; // тип один, deleter'ы разные
    EXPECT_EQ(a, 1);
    EXPECT_EQ(b, 0);
}

TEST(SharedPtrBasic, ConstCorrectness) {
    const shared_ptr<int> p(new int(3));
    EXPECT_EQ(*p, 3);
    EXPECT_NE(p.get(), nullptr);
    EXPECT_EQ(p.use_count(), 1u);
}