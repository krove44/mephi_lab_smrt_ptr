#include <iostream>
#include <memory>

#include "ptr/shared_ptr.h"
#include "ptr/uniq_ptr.h"
#include "ptr/weak_ptr.h"
// class First {
//     int x;
// public:
//     First(int x) : x(x) {}
// };
//
// class Second : public First {
// public:
//     Second(int x) : First(x){};
// };




int main() {
    // shared_ptr<int> p1 = new int(10);
    // shared_ptr<int> p2 = std::move(p1);
    // shared_ptr<int> p3 = std::move(p2);
    // std::cout << *p2 << "\n";
    // int y = 5;
    // auto s = "hello world";
    // int* x = new int(5);

    // shared_ptr<Second> p(new Second(42));
    // shared_ptr<First> p1 = p;
    // std::cout << p1.get() << std::endl;
    // std::cout << p.get() << std::endl;
    // std::cout << p.use_count() << std::endl;
    // shared_ptr<First> p3 = std::move(p);
    // std::cout << p3.get() << std::endl;
    // std::cout << p.get() << std::endl;
    // std::cout << p3.use_count() << std::endl;

    //

    // shared_ptr<int> p = new int(5);
    // std::cout << p.use_count() << std::endl;
    // weak_ptr<int> p1(p);
    // auto p2 = p1.lock();
    // std::cout << p1.use_count() << std::endl;

    // shared_ptr<Second> p = new Second(5);
    // {
    //     std::cout << p.use_count_weak() << '\n';
    //     weak_ptr<First> p1(p);
    //     std::cout << p.use_count_weak() << '\n';
    // }
    // std::cout << p.use_count_weak() << std::endl;

    // struct A {
    //     shared_ptr<A> p;
    //     // A(A* x) : p(x){};
    //
    // };
    //
    // {
    //     shared_ptr<A> p1(new A());
    //     shared_ptr<A> p2(new A());
    //     p1->p = p2;
    //     p2->p = p1;
    // }

    shared_ptr<int> p1 = new int(6);
    auto p2 = p1;



    return 0;
}