#ifndef CEPHALON_AABB_H_
#define CEPHALON_AABB_H_

#include <optional>

#include "utils.h"

namespace cephalon {
    enum class Direction {
        kRight,
        kLeft,
        kTop,
        kBottom,
        kBack,
        kFront
    };

    struct AABB {
        AABB() = default;
        AABB(Vec3i min_, Vec3i max_) {
            min = min_;
            max = max_;
        }

        Vec3i min;
        Vec3i max;

        std::optional<Direction> intersect(bx::Vec3 pos, bx::Vec3 dir, float t_min, float t_max) const {
            for (int i = 0; i < 3; ++i) {
                auto invd = 1.0f / get(dir, i);
                auto t0 = (min[i] - get(pos, i)) * invd;
                auto t1 = (max[i] - get(pos, i)) * invd;
                if (invd < 0.0f)
                    std::swap(t0, t1);
                t_min = t0 > t_min ? t0 : t_min;
                t_max = t1 < t_max ? t1 : t_max;
                if (t_max <= t_min)
                    return std::nullopt;
            }
            
            auto point = bx::sub(bx::add(pos, bx::mul(dir, t_min)), bx::div(bx::add(min, max), 2.0f));
            if (std::abs(point.x) > std::abs(point.y) && std::abs(point.x) > std::abs(point.z)) {
                if (point.x > 0)
                    return Direction::kRight;
                else
                    return Direction::kLeft;
            }
            else if (std::abs(point.y) > std::abs(point.x) && std::abs(point.y) > std::abs(point.z)) {
                if (point.y > 0)
                    return Direction::kTop;
                else
                    return Direction::kBottom;
            }
            else {
                if (point.z > 0)
                    return Direction::kBack;
                else
                    return Direction::kFront;
            }
        }
    };
}

#endif // CEPHALON_AABB_H_