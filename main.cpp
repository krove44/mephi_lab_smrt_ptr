#include <iostream>
#include "ptr/shared_ptr.h"
#include "ptr/uniq_ptr.h"

int main() {
    uniq_ptr<int> ptr(new int(42));
    uniq_ptr<int> ptr2(std::move(ptr));
    std::cout << *ptr2.get() << std::endl;
    return 0;
}