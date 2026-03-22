#pragma once

#include "model/types.h"
namespace constants 
{

static constexpr int MAX_LEGAL_MOVES = 218;

static constexpr sq_t W_KSIDE_ROOK_START_SQ = 7;
static constexpr sq_t W_QSIDE_ROOK_START_SQ = 0;
static constexpr sq_t B_KSIDE_ROOK_START_SQ = 63;
static constexpr sq_t B_QSIDE_ROOK_START_SQ = 56;
static constexpr sq_t W_KING_START_SQ = 4;
static constexpr sq_t B_KING_START_SQ = 60;

static constexpr sq_t W_KSIDE_KING_CASTLE_TO_SQ = 6;
static constexpr sq_t W_QSIDE_KING_CASTLE_TO_SQ = 2;
static constexpr sq_t B_KSIDE_KING_CASTLE_TO_SQ = 62;
static constexpr sq_t B_QSIDE_KING_CASTLE_TO_SQ = 58;
static constexpr sq_t W_KSIDE_ROOK_CASTLE_TO_SQ = 5;
static constexpr sq_t W_QSIDE_ROOK_CASTLE_TO_SQ = 3;
static constexpr sq_t B_KSIDE_ROOK_CASTLE_TO_SQ = 61;
static constexpr sq_t B_QSIDE_ROOK_CASTLE_TO_SQ = 59;

} // namespace constants

