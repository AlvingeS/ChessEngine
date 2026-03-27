#pragma once

#include <optional>

namespace engine::uci {

struct GoParams {
    std::optional<int>  wtime     = std::nullopt;
    std::optional<int>  btime     = std::nullopt;
    std::optional<int>  winc      = std::nullopt;
    std::optional<int>  binc      = std::nullopt;
    std::optional<int>  movestogo = std::nullopt;
    std::optional<int>  movetime  = std::nullopt;
    std::optional<int>  depth     = std::nullopt;
    std::optional<bool> infinite  = std::nullopt;
};

} // namespace engine::uci