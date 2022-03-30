#ifndef CEPHALON_FRUSTUM_H_
#define CEPHALON_FRUSTUM_H_

#include "camera.h"
#include "plane.h"

namespace cephalon {
    class Frustum {
    public:
       Frustum(PerspectiveCamera cam);
    private:
        Plane planes_[6];
    };
}

#endif // CEPHALON_FRUSTUM_H_