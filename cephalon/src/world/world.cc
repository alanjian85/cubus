#include "world.h"
using namespace cephalon;

#include <thread>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

World::World()
    : thread_pool_(std::thread::hardware_concurrency())
{

}

World::~World() {
    thread_pool_.join();
}

void World::update(glm::vec3 player_pos) {
    auto player_region = getRegion(glm::ivec3(player_pos));

    std::lock_guard lock(chunks_mutex_);
    for (auto i = chunks_.begin(); i != chunks_.end();) {
        auto& [region, chunk] = *i;
        if (glm::distance(glm::vec2(region), glm::vec2(player_region)) > Config::viewDistance)
        {
            chunks_.erase(i++);
        } else {
            ++i;
        }
    }
    
    int load_count = 0;
    for (auto x = player_region.x - Config::viewDistance; x <= player_region.x + Config::viewDistance; ++x) {
        for (auto y = player_region.y - Config::viewDistance; y <= player_region.y + Config::viewDistance; ++y) {
            glm::ivec2 region(x, y);
            if (load_count < Config::chunkLoadLimit && glm::distance(glm::vec2(region), glm::vec2(player_region)) <= Config::viewDistance) {
                auto [it, created] = chunks_.emplace(region, std::make_shared<Chunk>(*this, region));
                if (created) {
                    auto chunk = it->second; 
                    boost::asio::post(thread_pool_, [this, chunk = std::move(chunk)]() {
                        generator_(*chunk);
                        std::lock_guard lock(blocks_mutex_);
                        for (auto [pos, block] : blocks_) {
                            if (getRegion(pos) == chunk->getRegion()) {
                                chunk->setBlock(getOffset(pos), *block);
                            }
                        }
                    });
                    ++load_count;
                }
            }
        }
    }
}

void World::render(PerspectiveCamera cam) {
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