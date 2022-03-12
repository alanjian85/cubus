#ifndef CEPHALON_AABB_H_
#define CEPHALON_AABB_H_

#include "utils.h"

namespace cephalon {
    struct AABB {
        AABB() = default;
        AABB(Vec3i min_, Vec3i max_) {
            min = min_;
            max = max_;
        }

        Vec3i min;
        Vec3i max;

        bool intersect(bx::Vec3 pos, bx::Vec3 dir, double t_min, double t_max) const {
            for (int i = 0; i < 3; ++i) {
                auto invd = 1.0f / get(dir, i);
                auto t0 = (min[i] - get(pos, i)) * invd;
                auto t1 = (max[i] - get(pos, i)) * invd;
                if (invd < 0.0f)
                    std::swap(t0, t1);
                t_min = t0 > t_min ? t0 : t_min;
                t_max = t1 < t_max ? t1 : t_max;
                if (t_max <= t_min)
                    return false;
            }
            return true;
        }
    };
}

#endif // CEPHALON_AABB_H_