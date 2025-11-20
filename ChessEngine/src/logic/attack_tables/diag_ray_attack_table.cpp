#include "logic/attack_tables/diag_ray_attack_table.h"

#include "logic/movegen/utils/chess_utils.h"

namespace {

bitmask get_ne_ray(int sq_idx) 
{
    int rank = logic::ChessUtils::rank_from_bit_idx(sq_idx);
    int file = logic::ChessUtils::file_from_bit_idx(sq_idx);

    bitmask ne_ray = 0ULL;

    for (int i = 1; i < 8; i++) {
        if (rank + i < 8 && file - i >= 0) {
            ne_ray |= (1ULL << ((rank + i) * 8 + file - i));
        }
    }

    return ne_ray;
}

bitmask get_se_ray(int sq_idx) 
{
    int rank = logic::ChessUtils::rank_from_bit_idx(sq_idx);
    int file = logic::ChessUtils::file_from_bit_idx(sq_idx);

    bitmask se_ray = 0ULL;

    for (int i = 1; i < 8; i++) {
        if (rank - i >= 0 && file - i >= 0) {
            se_ray |= (1ULL << ((rank - i) * 8 + file - i));
        }
    }

    return se_ray;
}

bitmask get_sw_ray(int sq_idx) 
{
    int rank = logic::ChessUtils::rank_from_bit_idx(sq_idx);
    int file = logic::ChessUtils::file_from_bit_idx(sq_idx);

    bitmask sw_ray = 0ULL;

    for (int i = 1; i < 8; i++) {
        if (rank - i >= 0 && file + i < 8) {
            sw_ray |= (1ULL << ((rank - i) * 8 + file + i));
        }
    }

    return sw_ray;
}

bitmask get_nw_ray(int sq_idx) 
{
    int rank = logic::ChessUtils::rank_from_bit_idx(sq_idx);
    int file = logic::ChessUtils::file_from_bit_idx(sq_idx);

    bitmask north_west_ray = 0ULL;

    for (int i = 1; i < 8; i++) {
        if (rank + i < 8 && file + i < 8) {
            north_west_ray |= (1ULL << ((rank + i) * 8 + file + i));
        }
    }

    return north_west_ray;
}

void fill_ne_attack_table(ray_attack_table& diag_ray_attack_table)
{
    for (int sq_idx = 0; sq_idx < 64; sq_idx++) {
        diag_ray_attack_table[sq_idx][DiagDir::NE] = get_ne_ray(sq_idx);
    }
}

void fill_se_attack_table(ray_attack_table& diag_ray_attack_table)
{
    for (int sq_idx = 0; sq_idx < 64; sq_idx++) {
        diag_ray_attack_table[sq_idx][DiagDir::SE] = get_se_ray(sq_idx);
    }
}

void fill_sw_attack_table(ray_attack_table& diag_ray_attack_table)
{
    for (int sq_idx = 0; sq_idx < 64; sq_idx++) {
        diag_ray_attack_table[sq_idx][DiagDir::SW] = get_sw_ray(sq_idx);
    }
}

void fill_nw_attack_table(ray_attack_table& diag_ray_attack_table)
{
    for (int sq_idx = 0; sq_idx < 64; sq_idx++) {
        diag_ray_attack_table[sq_idx][DiagDir::NW] = get_nw_ray(sq_idx);
    }
}

} // namespace

namespace logic::attack_tables
{

void fill_diag_ray_attack_table(ray_attack_table& diag_ray_attack_table) 
{
    fill_ne_attack_table(diag_ray_attack_table);
    fill_se_attack_table(diag_ray_attack_table);
    fill_sw_attack_table(diag_ray_attack_table);
    fill_nw_attack_table(diag_ray_attack_table);
}

} // namespace logic::attack_tables