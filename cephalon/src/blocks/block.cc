#include "block.h"
using namespace cephalon;

std::map<std::string, const Block*> Block::blocks_;
bgfx::TextureHandle Block::texture_;
std::vector<std::string> Block::texture_names_;

Air blocks::air;
Dirt blocks::dirt;
Grass blocks::grass;
Sand blocks::sand;
Stone blocks::stone;
Wood blocks::wood;

void Block::init() {
    blocks::dirt.init();
    blocks::grass.init();
    blocks::sand.init();
    blocks::stone.init();
    blocks::wood.init();
    assert(bgfx::getCaps()->supported & BGFX_CAPS_TEXTURE_2D_ARRAY);
    std::uint16_t width = 32;
    std::uint16_t height = 32;
    texture_ = bgfx::createTexture2D(
        width, height, false, 
        static_cast<std::uint16_t>(texture_names_.size()), 
        bgfx::TextureFormat::RGBA8, 
        BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT
    );
    for (std::uint16_t i = 0; i < texture_names_.size(); ++i) {
        auto image = LoadImage(texture_names_[i].c_str());
        assert(image->m_width <= width);
        assert(image->m_height <= height);
        bgfx::updateTexture2D(texture_, i, 0, 0, 0, image->m_width, image->m_height, bgfx::makeRef(image->m_data, image->m_size));
    }
}

void Block::deinit() {
    bgfx::destroy(texture_);
}

std::uint16_t Block::loadTile(const std::string& name) {
    texture_names_.push_back("blocks/" + name);
    return texture_names_.size() - 1;
}