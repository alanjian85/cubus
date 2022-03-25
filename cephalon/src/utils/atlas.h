#ifndef CEPHALON_UTILS_ATLAS_H_
#define CEPHALON_UTILS_ATLAS_H_

#include <cassert>
#include <cstdint>

#include <glm/glm.hpp>

#include <bgfx/bgfx.h>

namespace cephalon {
    struct Region {
        Region() = default;

        Region(glm::ivec2 min_, glm::ivec2 max_) {
            min = min_;
            max = max_;
        }

        glm::ivec2 min;
        glm::ivec2 max;
    };

    class Atlas {
    public:
        static const bgfx::TextureFormat::Enum kFormat = bgfx::TextureFormat::RGBA8;

        void init(std::uint16_t width, std::uint16_t height) {
            size_.x = width;
            size_.y = height;
            texture_ = bgfx::createTexture2D(width, height, false, 1, kFormat);
        }

        void destroy() {
            bgfx::destroy(texture_);
        }

        Region add(bimg::ImageContainer* image) {
            assert(image->m_format == static_cast<bimg::TextureFormat::Enum>(kFormat));

            if (size_.x - 1 - insert_pos_.x < image->m_width) {
                insert_pos_.x = 0;
                insert_pos_ += image->m_height;
                if (insert_pos_.y >= image->m_height) {
                    return Region();
                }
            }
            bgfx::updateTexture2D(
                texture_, 0, 0, 
                insert_pos_.x, insert_pos_.y, 
                image->m_width, image->m_height, 
                bgfx::copy(image->m_data, image->m_width * image->m_height * 4)
            );
            Region region(insert_pos_, insert_pos_ + glm::ivec2(image->m_width, image->m_height));
            insert_pos_.x += image->m_width;
            return region;
        }

        bgfx::TextureHandle getTexture() const {
            return texture_;
        }

        glm::ivec2 getSize() const {
            return size_;
        }
    private:
        bgfx::TextureHandle texture_;
        glm::ivec2 size_;
        glm::ivec2 insert_pos_;
    };
}

#endif // CEPHALON_UTILS_ATLAS_H_