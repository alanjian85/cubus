#include "world.h"
using namespace cephalon;

#include <thread>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

World::World(const char* save_path)
    : thread_pool_(std::thread::hardware_concurrency()),
      database_(save_path)
{

}

World::~World() {
    thread_pool_.join();
}

void World::setSeed(unsigned seed) {
    generator_.setSeed(seed);
}

void World::setChunkDirty(glm::ivec2 region, bool dirty) {
    std::shared_lock lock(mutex_);
    auto it = chunks_.find(region);
    if (it != chunks_.cend())
        it->second->setDirty(dirty);
}

void World::setBlock(glm::ivec3 pos, const Block& block) {
    database_.insertBlock(pos, block.getName().c_str());
    std::shared_lock lock(mutex_);
    auto it = chunks_.find(getRegion(pos));
    if (it != chunks_.cend()) {
        auto chunk = it->second;
        lock.unlock();
        chunk->setBlock(getOffset(pos), block);
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
                    boost::asio::post(thread_pool_, [this, chunk = std::move(chunk)]() {
                        generator_(*chunk);
                        database_.loadChunk(*chunk);
                    });
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
            if (chunk->isDirty()) {
                auto diff = getRegion(cam.pos) - region;
                if (diff.x >= -1 && diff.x <= 1 && diff.y >= -1 && diff.y <= 1) {
                    chunk->rebuild();
                } else {
                    auto new_chunk = chunk;
                    boost::asio::post(thread_pool_, [chunk = std::move(new_chunk)]() {
                        chunk->rebuild();
                    });
                }
                chunk->setDirty(false);
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