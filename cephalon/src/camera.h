#ifndef CEPHALON_CAMERA_H_
#define CEPHALON_CAMERA_H_

#include <bx/math.h>

namespace cephalon {
    class PerspectiveCamera {
    public:
        PerspectiveCamera(bx::Vec3 position = bx::Vec3(0.0f), float yaw = 0.0f, float pitch = 0.0f)
            : position_(position)
        {
            yaw_ = yaw;
            pitch_ = pitch;
        }

        void setPosition(bx::Vec3 position) {
            position_ = position;
        }

        bx::Vec3 getPosition() const {
            return position_;
        }

        void setYaw(float yaw) {
            yaw_ = yaw;
        }

        float getYaw() const {
            return yaw_;
        }

        void setPitch(float pitch) {
            pitch_ = pitch;
        }

        float getPitch() const {
            return pitch_;
        }
    private:
        bx::Vec3 position_;

        float yaw_;
        float pitch_;
    };
}

#endif // CEPHALON_CAMERA_H_