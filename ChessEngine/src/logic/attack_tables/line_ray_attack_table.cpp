#include "logic/attack_tables/line_ray_attack_table.h"

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

bitmask get_e_ray(sq_idx sq) 
{
    int rank = logic::utils::rank_from_sq(sq);
    int file = logic::utils::file_from_sq(sq);

    bitmask e_ray = 0ULL;

    for (int i = file - 1; i >= 0; i--) {
        e_ray |= (1ULL << (rank * 8 + i));
    }

    return e_ray;
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

bitmask get_w_ray(sq_idx sq) 
{
    int rank = logic::utils::rank_from_sq(sq);
    int file = logic::utils::file_from_sq(sq);

    bitmask w_ray = 0ULL;

    for (int i = file + 1; i < 8; i++) {
        w_ray |= (1ULL << (rank * 8 + i));
    }

    return w_ray;
}

void fill_n_attack_table(ray_attack_table& line_ray_attack_table)
{
    for (sq_idx sq = 0; sq < 64; sq++) {
        line_ray_attack_table[sq][LineDir::N] = get_n_ray(sq);
    }
}

void fill_e_attack_table(ray_attack_table& line_ray_attack_table)
{
    for (sq_idx sq = 0; sq < 64; sq++) {
        line_ray_attack_table[sq][LineDir::E] = get_e_ray(sq);
    }
}

void fill_s_attack_table(ray_attack_table& line_ray_attack_table)
{
    for (sq_idx sq = 0; sq < 64; sq++) {
        line_ray_attack_table[sq][LineDir::S] = get_s_ray(sq);
    }
}

void fill_w_attack_table(ray_attack_table& line_ray_attack_table)
{
    for (sq_idx sq = 0; sq < 64; sq++) {
        line_ray_attack_table[sq][LineDir::W] = get_w_ray(sq);
    }
}

} // namespace

namespace logic::attack_tables
{

void fill_line_ray_attack_table(ray_attack_table& line_ray_attack_table) 
{
    fill_n_attack_table(line_ray_attack_table);
    fill_e_attack_table(line_ray_attack_table);
    fill_s_attack_table(line_ray_attack_table);
    fill_w_attack_table(line_ray_attack_table);
}

} // namespace logic::attack_tables