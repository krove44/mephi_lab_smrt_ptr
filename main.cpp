#include <iostream>
#include "smart_ptr.h"

int main() {
    smart_ptr<int> ptr(new int(10));
    smart_ptr<int> ptr2 = ptr;
    std::cout << *ptr2 << std::endl;
    return 0;
}