#include <iostream>
#include "smart_ptr.h"

int main() {
    int* p = new int(10);
    smart_ptr ptr(p);

    std::cout << ptr << std::endl;

    delete p;
    return 0;
}