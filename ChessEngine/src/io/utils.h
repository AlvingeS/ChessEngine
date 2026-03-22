#pragma once

#include "model/types.h"

#include <string>

namespace io::utils
{

sq_t sq_from_notation(const std::string& notation);
PieceType get_type_from_char(char c);
char get_char_from_type(PieceType type);

} // namespace utils