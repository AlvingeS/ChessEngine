#pragma once

#include "model/types.h"

#include <vector>

namespace logic {

struct RayBitmasks {

    struct StraightRays {
        bitmask n;
        bitmask e;
        bitmask s;
        bitmask w;
    };
    
    struct DiagonalRays {
        bitmask ne;
        bitmask se;
        bitmask sw;
        bitmask nw;
    };

    static bitmask get_n_ray(int ind);
    static bitmask get_ne_ray(int ind);
    static bitmask get_e_ray(int ind);
    static bitmask get_se_ray(int ind);
    static bitmask get_s_ray(int ind);
    static bitmask get_sw_ray(int ind);
    static bitmask get_w_ray(int ind);
    static bitmask get_nw_ray(int ind);
    
    static StraightRays get_straight_rays(int ind);
    static DiagonalRays get_diagonal_rays(int ind);
    
    static std::vector<StraightRays> get_all_straight_ray_bitmasks();
    static std::vector<DiagonalRays> get_all_diagonal_ray_bitmasks();
};

} // namespace logic