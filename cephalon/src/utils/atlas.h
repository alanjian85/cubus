#ifndef CEPHALON_UTILS_ATLAS_H_
#define CEPHALON_UTILS_ATLAS_H_

#include <cassert>
#include <cstdint>

#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

#include <bgfx/bgfx.h>

namespace cephalon {
    struct Region {
        Region() = default;

        Region(glm::vec2 min_, glm::vec2 max_) {
            min = min_;
            max = max_;
        }

        glm::vec2 min;
        glm::vec2 max;
    };

    class Atlas {
    public:
        static const bgfx::TextureFormat::Enum kFormat = bgfx::TextureFormat::RGBA8;

        Atlas(std::uint16_t width, std::uint16_t height) {
            size_.x = width;
            size_.y = height;
            texture_ = bgfx::createTexture2D(width, height, false, 1, kFormat, BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT);
            if (!bgfx::isValid(texture_)) {
                spdlog::error("Couldn't create atlas texture");
            } else {
                spdlog::info("Atlas texture created successfully");
            }
        }

        ~Atlas() {
            bgfx::destroy(texture_);
            spdlog::info("Atlas texture destroyed");
        }

        Region add(bimg::ImageContainer* image) {
            assert(image->m_format == static_cast<bimg::TextureFormat::Enum>(kFormat));

            if (size_.x - insert_pos_.x < image->m_width) {
                insert_pos_.x = 0;
                insert_pos_.y += image->m_height;
            }
            if (insert_pos_.y >= size_.y) {
                return Region();
            }
            bgfx::updateTexture2D(
                texture_, 0, 0, 
                insert_pos_.x, insert_pos_.y, 
                image->m_width, image->m_height, 
                bgfx::copy(image->m_data, image->m_width * image->m_height * 4)
            );
            auto min = insert_pos_;
            auto max = insert_pos_ + glm::ivec2(image->m_width - 1, image->m_height - 1);
            insert_pos_.x += image->m_width;
            return Region(glm::vec2(min) / glm::vec2(size_ - 1), glm::vec2(max) / glm::vec2(size_ - 1));
        }

        bgfx::TextureHandle getTexture() const {
            return texture_;
        }

        glm::ivec2 getSize() const {
            return size_;
        }

        bgfx::TextureHandle getHandle() const {
            return texture_;
        }
    private:
        bgfx::TextureHandle texture_;
        glm::ivec2 size_;
        glm::ivec2 insert_pos_;
    };
}

#endif // CEPHALON_UTILS_ATLAS_H_