#include "generator.h"
using namespace cephalon;

#include <cmath>

#include <noise.h>

#include "world.h"

void Generator::operator()(World& world, glm::ivec2 region) const {
    for (int x = 0; x < Chunk::kVolume.x; ++x) {
        for (int z = 0; z < Chunk::kVolume.z; ++z) {
            auto pos = World::getPosition(region, glm::ivec3(x, 0, z));
            auto f = simplex2(pos.x * 0.01f, pos.z * 0.01f, 4, 0.5f, 2);
            auto g = simplex2(-pos.x * 0.01f, -pos.z * 0.01f, 2, 0.9f, 2);
            int mh = g * 32 + 16;
            int h = f * mh;
            int w = 1;
            int t = 12;
            if (h <= t) {
                h = t;
                w = 2;
            }
            
            for (pos.y = 0; pos.y < h; ++pos.y) {
                if (w == 1)
                    world.setBlock(pos, blocks::grass);
                else
                    world.setBlock(pos, blocks::sand);
            }
        }
    }
}