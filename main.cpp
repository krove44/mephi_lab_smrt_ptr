#include <iostream>
#include "ptr/shared_ptr.h"
#include "ptr/uniq_ptr.h"


int main() {
    shared_ptr<int> p1 = new int(10);
    shared_ptr<int> p2 = std::move(p1);
    shared_ptr<int> p3 = std::move(p2);
    std::cout << *p2 << "\n";
    return 0;
}