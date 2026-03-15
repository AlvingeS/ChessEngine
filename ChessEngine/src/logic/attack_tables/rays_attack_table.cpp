#include "logic/attack_tables/rays_attack_table.h"

#include "logic/utils.h"

namespace {

bitmask get_n_ray(sq_idx sq) 
{
    int rank = logic::utils::rank_from_sq(sq);
    int file = logic::utils::file_from_sq(sq);

    bitmask n_ray = 0ULL;

    for (int i = rank + 1; i < 8; i++) {
        n_ray |= (1ULL << (i * 8 + file));
    }

    return n_ray;
}


bitmask get_ne_ray(sq_idx sq) 
{
    int rank = logic::utils::rank_from_sq(sq);
    int file = logic::utils::file_from_sq(sq);

    bitmask ne_ray = 0ULL;

    for (int i = 1; i < 8; i++) {
        if (rank + i < 8 && file + i < 8) {
            ne_ray |= (1ULL << ((rank + i) * 8 + file + i));
        }
    }

    return ne_ray;
}

bitmask get_e_ray(sq_idx sq) 
{
    int rank = logic::utils::rank_from_sq(sq);
    int file = logic::utils::file_from_sq(sq);

    bitmask e_ray = 0ULL;

    for (int i = file + 1; i < 8; i++) {
        e_ray |= (1ULL << (rank * 8 + i));
    }

    return e_ray;
}

bitmask get_se_ray(sq_idx sq) 
{
    int rank = logic::utils::rank_from_sq(sq);
    int file = logic::utils::file_from_sq(sq);

    bitmask se_ray = 0ULL;

    for (int i = 1; i < 8; i++) {
        if (rank - i >= 0 && file + i < 8) {
            se_ray |= (1ULL << ((rank - i) * 8 + file + i));
        }
    }

    return se_ray;
}

bitmask get_s_ray(sq_idx sq) 
{
    int rank = logic::utils::rank_from_sq(sq);
    int file = logic::utils::file_from_sq(sq);

    bitmask s_ray = 0ULL;

    for (int i = rank - 1; i >= 0; i--) {
        s_ray |= (1ULL << (i * 8 + file));
    }

    return s_ray;
}

bitmask get_sw_ray(sq_idx sq) 
{
    int rank = logic::utils::rank_from_sq(sq);
    int file = logic::utils::file_from_sq(sq);

    bitmask sw_ray = 0ULL;

    for (int i = 1; i < 8; i++) {
        if (rank - i >= 0 && file - i >= 0) {
            sw_ray |= (1ULL << ((rank - i) * 8 + file - i));
        }
    }

    return sw_ray;
}

bitmask get_w_ray(sq_idx sq) 
{
    int rank = logic::utils::rank_from_sq(sq);
    int file = logic::utils::file_from_sq(sq);

    bitmask w_ray = 0ULL;

    for (int i = file - 1; i >= 0; i--) {
        w_ray |= (1ULL << (rank * 8 + i));
    }

    return w_ray;
}

bitmask get_nw_ray(sq_idx sq) 
{
    int rank = logic::utils::rank_from_sq(sq);
    int file = logic::utils::file_from_sq(sq);

    bitmask north_west_ray = 0ULL;

    for (int i = 1; i < 8; i++) {
        if (rank + i < 8 && file - i >= 0) {
            north_west_ray |= (1ULL << ((rank + i) * 8 + file - i));
        }
    }

    return north_west_ray;
}

void fill_n_attack_table(ray_attack_table& ray_attack_table)
{
    for (sq_idx sq = 0; sq < 64; sq++) {
        ray_attack_table[sq][Direction::N] = get_n_ray(sq);
    }
}

void fill_ne_attack_table(ray_attack_table& ray_attack_table)
{
    for (sq_idx sq = 0; sq < 64; sq++) {
        ray_attack_table[sq][Direction::NE] = get_ne_ray(sq);
    }
}

void fill_e_attack_table(ray_attack_table& ray_attack_table)
{
    for (sq_idx sq = 0; sq < 64; sq++) {
        ray_attack_table[sq][Direction::E] = get_e_ray(sq);
    }
}

void fill_se_attack_table(ray_attack_table& ray_attack_table)
{
    for (sq_idx sq = 0; sq < 64; sq++) {
        ray_attack_table[sq][Direction::SE] = get_se_ray(sq);
    }
}


void fill_s_attack_table(ray_attack_table& ray_attack_table)
{
    for (sq_idx sq = 0; sq < 64; sq++) {
        ray_attack_table[sq][Direction::S] = get_s_ray(sq);
    }
}

void fill_sw_attack_table(ray_attack_table& ray_attack_table)
{
    for (sq_idx sq = 0; sq < 64; sq++) {
        ray_attack_table[sq][Direction::SW] = get_sw_ray(sq);
    }
}

void fill_w_attack_table(ray_attack_table& ray_attack_table)
{
    for (sq_idx sq = 0; sq < 64; sq++) {
        ray_attack_table[sq][Direction::W] = get_w_ray(sq);
    }
}

void fill_nw_attack_table(ray_attack_table& ray_attack_table)
{
    for (sq_idx sq = 0; sq < 64; sq++) {
        ray_attack_table[sq][Direction::NW] = get_nw_ray(sq);
    }
}


} // namespace

namespace logic::attack_tables
{

void fill_ray_attack_table(ray_attack_table& ray_attack_table) 
{
    fill_n_attack_table(ray_attack_table);
    fill_ne_attack_table(ray_attack_table);
    fill_e_attack_table(ray_attack_table);
    fill_se_attack_table(ray_attack_table);
    fill_s_attack_table(ray_attack_table);
    fill_sw_attack_table(ray_attack_table);
    fill_w_attack_table(ray_attack_table);
    fill_nw_attack_table(ray_attack_table);
}

} // namespace logic::attack_tables