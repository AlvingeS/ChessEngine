#include <iostream>
#include <bitset>
#include "headers/bit_logic.h"

namespace utils {
    void print_bits(unsigned long long int n) {
        std::cout << std::bitset<64>(n) << std::endl;
    }
}
