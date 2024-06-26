#pragma once

#include <noise.h>

#include "world/world.h"

namespace cephalon {
    class Normal : public Terrain {
    public:
        Normal()
             : Terrain("normal")
        {

        }

        virtual void setSeed(unsigned seed) override {
            ::seed(seed);
        }

        void genChunk(Chunk& chunk) const override {
            for (int x = 0; x < Chunk::kVolume.x; ++x) {
                for (int z = 0; z < Chunk::kVolume.z; ++z) {
                    auto pos = World::getPosition(chunk.getRegion(), glm::ivec3(x, 0, z));
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

                    chunk.setTerrainBlock(glm::ivec3(x, 0, z), blocks::stone);
                    for (int y = 1; y < h; ++y) {
                        if (w == 1)
                            chunk.setTerrainBlock(glm::ivec3(x, y, z), blocks::dirt);
                        else
                            chunk.setTerrainBlock(glm::ivec3(x, y, z), blocks::sand);
                    }
                    if (w == 1) {
                        chunk.setTerrainBlock(glm::ivec3(x, h, z), blocks::grass);
                    }
                }
            }
        }
    };
}
