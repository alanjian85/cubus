#include "block.h"
using namespace cephalon;

std::map<std::string, const Block*> Block::blocks_;
std::vector<bimg::ImageContainer*> Block::images_;
bgfx::TextureHandle Block::texture_ = BGFX_INVALID_HANDLE;
glm::ivec2 Block::texture_size_;

Air blocks::air;
Dirt blocks::dirt;
Grass blocks::grass;
Sand blocks::sand;
Stone blocks::stone;
Wood blocks::wood;

TextureInfo Block::loadTexture(const std::string& name) {
    auto image = LoadImage("blocks/" + name);
    images_.push_back(image);
    return TextureInfo(images_.size() - 1, glm::ivec2(image->m_width, image->m_height));
}

void Block::init() {
    blocks::dirt.init();
    blocks::grass.init();
    blocks::sand.init();
    blocks::stone.init();
    blocks::wood.init();
    assert(bgfx::getCaps()->supported & BGFX_CAPS_TEXTURE_2D_ARRAY);
    for (auto image : images_) {
        texture_size_.x = std::max<int>(texture_size_.x, image->m_width);
        texture_size_.y = std::max<int>(texture_size_.y, image->m_height);
    }
    if (texture_size_.x != 0 && texture_size_.y != 0) {
        texture_ = bgfx::createTexture2D(
            texture_size_.x, texture_size_.y, false, 
            static_cast<std::uint16_t>(images_.size()), 
            bgfx::TextureFormat::RGBA8, 
            BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT
        );
        for (std::uint16_t i = 0; i < images_.size(); ++i) {
            auto image = images_[i];
            bgfx::updateTexture2D(texture_, i, 0, 0, 0, image->m_width, image->m_height, bgfx::makeRef(image->m_data, image->m_size));
        }
    }
}

void Block::deinit() {
    if (bgfx::isValid(texture_)) {
        bgfx::destroy(texture_);
    }
}