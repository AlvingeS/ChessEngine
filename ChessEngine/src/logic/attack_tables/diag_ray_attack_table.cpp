#include "logic/attack_tables/diag_ray_attack_table.h"

#include "logic/utils.h"

namespace {

bitmask get_ne_ray(sq_idx sq) 
{
    int rank = logic::utils::rank_from_sq(sq);
    int file = logic::utils::file_from_sq(sq);

    bitmask ne_ray = 0ULL;

    for (int i = 1; i < 8; i++) {
        if (rank + i < 8 && file - i >= 0) {
            ne_ray |= (1ULL << ((rank + i) * 8 + file - i));
        }
    }

    return ne_ray;
}

bitmask get_se_ray(sq_idx sq) 
{
    int rank = logic::utils::rank_from_sq(sq);
    int file = logic::utils::file_from_sq(sq);

    bitmask se_ray = 0ULL;

    for (int i = 1; i < 8; i++) {
        if (rank - i >= 0 && file - i >= 0) {
            se_ray |= (1ULL << ((rank - i) * 8 + file - i));
        }
    }

    return se_ray;
}

bitmask get_sw_ray(sq_idx sq) 
{
    int rank = logic::utils::rank_from_sq(sq);
    int file = logic::utils::file_from_sq(sq);

    bitmask sw_ray = 0ULL;

    for (int i = 1; i < 8; i++) {
        if (rank - i >= 0 && file + i < 8) {
            sw_ray |= (1ULL << ((rank - i) * 8 + file + i));
        }
    }

    return sw_ray;
}

bitmask get_nw_ray(sq_idx sq) 
{
    int rank = logic::utils::rank_from_sq(sq);
    int file = logic::utils::file_from_sq(sq);

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
    for (sq_idx sq = 0; sq < 64; sq++) {
        diag_ray_attack_table[sq][DiagDir::NE] = get_ne_ray(sq);
    }
}

void fill_se_attack_table(ray_attack_table& diag_ray_attack_table)
{
    for (sq_idx sq = 0; sq < 64; sq++) {
        diag_ray_attack_table[sq][DiagDir::SE] = get_se_ray(sq);
    }
}

void fill_sw_attack_table(ray_attack_table& diag_ray_attack_table)
{
    for (sq_idx sq = 0; sq < 64; sq++) {
        diag_ray_attack_table[sq][DiagDir::SW] = get_sw_ray(sq);
    }
}

void fill_nw_attack_table(ray_attack_table& diag_ray_attack_table)
{
    for (sq_idx sq = 0; sq < 64; sq++) {
        diag_ray_attack_table[sq][DiagDir::NW] = get_nw_ray(sq);
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