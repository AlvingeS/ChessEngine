#include <iostream>
#include <bitset>

namespace utils {
    void print_bits(unsigned long long int n) {
        std::cout << std::bitset<64>(n) << std::endl;
    }
}
