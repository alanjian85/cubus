#ifndef CEPHALON_WORLD_H_
#define CEPHALON_WORLD_H_

#include <unordered_map>

#include "chunk.h"
#include "config.h"
#include "utils.h"

namespace cephalon {
    class World {
    public:
        void setGenerator(std::function<void(Vec3i, Chunk&)> generator) {
            generator_ = generator;
        } 

        static Vec3i getChunkOffset(Vec3i pos) {
            return { pos.x / Chunk::kChunkSize.x, pos.y / Chunk::kChunkSize.y, pos.z / Chunk::kChunkSize.z };
        }

        static Vec3i getChunkPos(Vec3i pos) {
            return { pos.x % Chunk::kChunkSize.x, pos.y % Chunk::kChunkSize.y, pos.z % Chunk::kChunkSize.z };
        }

        Chunk& getChunk(Vec3i pos) {
            return chunks_[getChunkOffset(pos)];
        }

        void setBlock(Vec3i pos, const Block& block) {
            getChunk(pos).setBlock(getChunkPos(pos), block);
        }

        const Block& getBlock(Vec3i pos) {
            return getChunk(pos).getBlock(getChunkPos(pos));
        }

        void update(bx::Vec3 playerPos) {
            auto playerOffset = getChunkOffset({
                static_cast<int>(playerPos.x), 
                static_cast<int>(playerPos.y), 
                static_cast<int>(playerPos.z)
            });
            for (auto i = chunks_.begin(); i != chunks_.end();) {
                auto& [pos, chunk] = *i;
                if (std::abs(playerOffset.x - pos.x) > Config::kViewDistance ||
                    std::abs(playerOffset.y - pos.y) > Config::kViewDistance ||
                    std::abs(playerOffset.z - pos.z) > Config::kViewDistance)
                {
                    chunks_.erase(i++);
                } else {
                    ++i;
                }
            }

            int loadCount = 0;
            for (int x = playerOffset.x - Config::kViewDistance; x <= playerOffset.x + Config::kViewDistance; ++x) {
                for (int y = playerOffset.y - Config::kViewDistance; y <= playerOffset.y + Config::kViewDistance; ++y) {
                    for (int z = playerOffset.z - Config::kViewDistance; z <= playerOffset.z + Config::kViewDistance; ++z) {
                        if (chunks_.find({x, y, z}) == chunks_.cend() &&
                            loadCount < Config::kMaxLoadCount) {
                            generator_({x, y, z}, chunks_[{x, y, z}]);
                            ++loadCount;
                        }
                    }
                }
            }

            for (auto& [_, chunk] : chunks_) {
                chunk.update();
            }
        }

        void render(bgfx::ProgramHandle program);
    private:
        std::unordered_map<Vec3i, Chunk> chunks_;
        std::function<void(Vec3i, Chunk&)> generator_;
    };
}

#endif // CEPHALON_WORLD_H_