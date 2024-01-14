#include "our_project/tools/tools.h"
#include <iostream>

namespace tools {
    void print_hello() {
        std::cout << "Hello, world!!" << std::endl;
    }
        
    int add(int a, int b) {
        return a + b;
    }

    int sub(int a, int b) {
        return a - b;
    }
}
