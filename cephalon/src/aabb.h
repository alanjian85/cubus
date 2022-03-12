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
    };
}

#endif // CEPHALON_AABB_H_