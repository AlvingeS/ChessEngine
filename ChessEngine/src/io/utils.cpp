#include "io/utils.h"

namespace io::utils
{

sq_idx sq_from_notation(const std::string& notation)
{
    char file = notation[0];
    int row_idx = (notation[1] - '0') - 1;

    int file_idx;
    switch (file) {
        case 'a': file_idx = 0; break;
        case 'b': file_idx = 1; break;
        case 'c': file_idx = 2; break;
        case 'd': file_idx = 3; break;
        case 'e': file_idx = 4; break;
        case 'f': file_idx = 5; break;
        case 'g': file_idx = 6; break;
        case 'h': file_idx = 7; break;
        default:  file_idx = -1;
    }

    return 8 * row_idx + file_idx;
}

} // namespace utils