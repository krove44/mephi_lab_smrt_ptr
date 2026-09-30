#include <iostream>
#include "ptr/shared_ptr.h"
#include "ptr/uniq_ptr.h"

class First {
    int x;
public:
    First(int x) : x(x) {}
};

class Second : public First {
public:
    Second(int x) : First(x){};
};


int main() {
    // shared_ptr<int> p1 = new int(10);
    // shared_ptr<int> p2 = std::move(p1);
    // shared_ptr<int> p3 = std::move(p2);
    // std::cout << *p2 << "\n";
    // int y = 5;
    // auto s = "hello world";
    // int* x = new int(5);

    shared_ptr<Second> p(new Second(42));
    shared_ptr<First> p1 = p;


    return 0;
}