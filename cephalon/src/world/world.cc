#include "world.h"
using namespace cephalon;

#include <thread>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "terrains/terrain.h"

World::World(const char* save_path)
    : rebuild_thread_pool_(std::thread::hardware_concurrency()),
      database_(save_path)
{
    terrain_ = nullptr;
}

World::~World() {
    rebuild_thread_pool_.join();
}

void World::setTerrain(const std::string& name) {
    terrain_ = Terrain::getTerrain(name);
}

void World::setSeed(unsigned seed) {
    terrain_->setSeed(seed);
}

void World::setBlock(glm::ivec3 pos, const Block& block) {
    database_.insertBlock(pos, block.getName().c_str());
    auto region = getRegion(pos);
    std::shared_lock lock(mutex_);
    auto it = chunks_.find(region);
    if (it != chunks_.cend()) {
        auto flags = it->second->setBlock(getOffset(pos), block);
        if ((flags & NeighborChunk::kLeft) != NeighborChunk::kNone)
            setChunkDirty(region + glm::ivec2(-1,  0), true);
        if ((flags & NeighborChunk::kRight) != NeighborChunk::kNone)
            setChunkDirty(region + glm::ivec2( 1,  0), true);
        if ((flags & NeighborChunk::kDown) != NeighborChunk::kNone)
            setChunkDirty(region + glm::ivec2( 0, -1), true);
        if ((flags & NeighborChunk::kUp) != NeighborChunk::kNone)
            setChunkDirty(region + glm::ivec2( 0,  1), true);
        if ((flags & NeighborChunk::kLowerLeft) != NeighborChunk::kNone)
            setChunkDirty(region + glm::ivec2(-1, -1), true);
        if ((flags & NeighborChunk::kUpperLeft) != NeighborChunk::kNone)
            setChunkDirty(region + glm::ivec2(-1,  1), true);
        if ((flags & NeighborChunk::kLowerRight) != NeighborChunk::kNone)
            setChunkDirty(region + glm::ivec2( 1, -1), true);
        if ((flags & NeighborChunk::kUpperRight) != NeighborChunk::kNone)
            setChunkDirty(region + glm::ivec2( 1,  1), true);
    }
}

const Block* World::getBlock(glm::ivec3 pos) const {
    std::shared_lock lock(mutex_);
    auto it = chunks_.find(getRegion(pos));
    if (it != chunks_.cend())
        return &it->second->getBlock(getOffset(pos));
    return nullptr;
}

void World::update(glm::vec3 player_pos) {
    auto player_region = getRegion(glm::ivec3(player_pos));

    std::lock_guard lock(mutex_);
    for (auto i = chunks_.begin(); i != chunks_.end();) {
        auto& [region, chunk] = *i;
        if (glm::distance(glm::vec2(region), glm::vec2(player_region)) > Config::viewDistance + 1)
        {
            chunks_.erase(i++);
        } else {
            ++i;
        }
    }
    
    int load_count = 0;
    for (auto x = player_region.x - Config::viewDistance - 1; x <= player_region.x + Config::viewDistance + 1; ++x) {
        for (auto y = player_region.y - Config::viewDistance - 1; y <= player_region.y + Config::viewDistance + 1; ++y) {
            glm::ivec2 region(x, y);
            if (load_count < Config::chunkLoadLimit && glm::distance(glm::vec2(region), glm::vec2(player_region)) <= Config::viewDistance + 1) {
                auto [it, created] = chunks_.emplace(region, std::make_shared<Chunk>(*this, region));
                if (created) {
                    auto chunk = it->second; 
                    terrain_->genChunk(*chunk);
                    database_.loadChunk(*chunk);
                    for (int x = -1; x <= 1; ++x) {
                        for (int y = -1; y <= 1; ++y) {
                            setChunkDirty(chunk->getRegion() + glm::ivec2(x, y), true);
                        }
                    }
                    ++load_count;
                }
            }
        }
    }
}

void World::render(PerspectiveCamera cam) {
    std::shared_lock lock(mutex_);
    for (auto& [region, chunk] : chunks_) {
        if (chunk->inbound(cam)) {
            if (chunk->isDirty() && !chunk->isRebuilding()) {
                auto diff = getRegion(cam.pos) - region;
                chunk->setRebuilding(true);
                if (diff.x >= -1 && diff.x <= 1 && diff.y >= -1 && diff.y <= 1) {
                    chunk->rebuild();
                } else {
                    auto new_chunk = chunk;
                    boost::asio::post(rebuild_thread_pool_, [chunk = std::move(new_chunk)]() {
                        chunk->rebuild();
                    });
                }
            }

            chunk->render(cam);
        }
    }
}

bool World::intersect(PerspectiveCamera cam, Direction& dir, glm::ivec3& pos) const {
    std::shared_lock lock(mutex_);
    bool intersected = false;
    auto dmax = static_cast<float>(Config::destroyDistance);
    for (auto& [region, chunk] : chunks_) {
        glm::ivec3 offset;
        if (chunk->inbound(cam) && chunk->intersect(Ray(cam.pos, cam.dir), cam.near, dmax, dir, offset, dmax)) {
            intersected = true;
            pos = getPosition(chunk->getRegion(), offset);
        }
    }
    return intersected;
}

void World::setChunkDirty(glm::ivec2 region, bool dirty) {
    auto it = chunks_.find(region);
    if (it != chunks_.cend())
        it->second->setDirty(dirty);
}