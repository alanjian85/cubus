#ifndef CEPHALON_AABB_H_
#define CEPHALON_AABB_H_

#include "utils.h"

namespace cephalon {
    struct AABB {
        Vec3i min;
        Vec3i max;
    };
}

#endif // CEPHALON_AABB_H_