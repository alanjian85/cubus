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

    std::unique_lock lock(chunks_mutex_);
    for (auto i = chunks_.begin(); i != chunks_.end();) {
        auto& [region, chunk] = *i;
        if (std::abs(player_region.x - region.x) > Config::viewDistance ||
            std::abs(player_region.y - region.y) > Config::viewDistance)
        {
            chunks_.erase(i++);
        } else {
            ++i;
        }
    }
    
    int load_count = 0;
    for (auto x = player_region.x - Config::viewDistance; x <= player_region.x + Config::viewDistance; ++x) {
        for (auto y = player_region.y - Config::viewDistance; y <= player_region.y + Config::viewDistance; ++y) {
            if (load_count < Config::chunkLoadLimit) {
                glm::ivec2 region(x, y);
                auto [it, created] = chunks_.emplace(region, Chunk(*this, region));
                auto& chunk = it->second;
                if (created) {
                    boost::asio::post(thread_pool_, [this, &chunk]() {
                        generator_(chunk);
                        for (auto [pos, block] : blocks_) {
                            if (getRegion(pos) == chunk.getRegion()) {
                                chunk.setBlock(getOffset(pos), *block);
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
        if (chunk.inbound(cam)) {
            if (chunk.isDirty()) {
                if (region == getRegion(cam.pos)) {
                    chunk.rebuild();
                } else {
                    boost::asio::post(thread_pool_, std::bind(&Chunk::rebuild, &chunk));
                }
                chunk.setDirty(false);
            }

            chunk.render(cam);
        }
    }
}

bool World::intersect(PerspectiveCamera cam, Direction& dir, glm::ivec3& pos) const {
    bool intersected = false;
    auto dmax = static_cast<float>(Config::destroyDistance);
    for (auto& [region, chunk] : chunks_) {
        glm::ivec3 offset;
        if (chunk.inbound(cam) && chunk.intersect(Ray(cam.pos, cam.dir), cam.near, dmax, dir, offset, dmax)) {
            intersected = true;
            pos = getPosition(chunk.getRegion(), offset);
        }
    }
    return intersected;
}