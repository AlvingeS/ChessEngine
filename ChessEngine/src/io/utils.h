#pragma once

#include "model/types.h"
#include "model/position/piece_type.h"

#include <string>

namespace io::utils
{

sq_t sq_from_notation(const std::string& notation);

} // namespace utils