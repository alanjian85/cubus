#pragma once

namespace cephalon {
    class Flat : public Terrain {
    public:
        Flat()
             : Terrain("flat")
        {

        }

        virtual void setSeed(unsigned seed) override {

        }

        void genChunk(Chunk& chunk) const override {
            for (int x = 0; x < Chunk::kVolume.x; ++x) {
                for (int z = 0; z < Chunk::kVolume.z; ++z) {
                    chunk.setTerrainBlock(glm::ivec3(x, 0, z), blocks::stone);
                    for (int y = 1; y < 5; ++y) {
                        chunk.setTerrainBlock(glm::ivec3(x, y, z), blocks::dirt);   
                    }
                    chunk.setTerrainBlock(glm::ivec3(x, 5, z), blocks::grass);
                }
            }
        }
    };
}
