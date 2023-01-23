#ifndef CEPHALON_TERRAINS_SINE_H_
#define CEPHALON_TERRAINS_SINE_H_

namespace cephalon {
    class Sine : public Terrain {
    public:
        Sine()
             : Terrain("sine")
        {

        }

        virtual void setSeed(unsigned seed) override {

        }

        void genChunk(Chunk& chunk) const override {
            for (int x = 0; x < Chunk::kVolume.x; ++x) {
                for (int z = 0; z < Chunk::kVolume.z; ++z) {
                    auto s = std::sin(x / 4.0f) * std::sin(z / 4.0f) * 0.5f + 0.5f;
                    auto h = static_cast<int>(s * 10);
                    chunk.setTerrainBlock(glm::ivec3(x, 0, z), blocks::stone);
                    for (int y = 1; y < h; ++y) {
                        chunk.setTerrainBlock(glm::ivec3(x, y, z), blocks::dirt);
                    }
                    chunk.setTerrainBlock(glm::ivec3(x, h, z), blocks::grass);
                }
            }
        }
    };
}

#endif // CEPHALON_TERRAINS_SINE_H_
