#include "outline.h"
using namespace cephalon;

#include "timer.h"

Outline::Outline() {
    program_ = LoadProgram("vs_outline", "fs_outline");

    layout_.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
    .end();

    float vertices[] = {
         0.525f, -0.525f, -0.525f,
         0.525f, -0.525f,  0.525f,
         0.525f,  0.525f, -0.525f,
         0.525f,  0.525f,  0.525f,

        -0.525f, -0.525f, -0.525f,
        -0.525f, -0.525f,  0.525f,
        -0.525f,  0.525f, -0.525f,
        -0.525f,  0.525f,  0.525f,                       

        -0.525f,  0.525f, -0.525f,
        -0.525f,  0.525f,  0.525f,
         0.525f,  0.525f, -0.525f,
         0.525f,  0.525f,  0.525f,

        -0.525f, -0.525f, -0.525f,
        -0.525f, -0.525f,  0.525f,
         0.525f, -0.525f, -0.525f,
         0.525f, -0.525f,  0.525f,

        -0.525f, -0.525f,  0.525f,
        -0.525f,  0.525f,  0.525f,
         0.525f, -0.525f,  0.525f,
         0.525f,  0.525f,  0.525f,

        -0.525f, -0.525f, -0.525f,
        -0.525f,  0.525f, -0.525f,
         0.525f, -0.525f, -0.525f,
         0.525f,  0.525f, -0.525f
    };

    std::uint16_t indices[] = {
        1, 3, 2,
        1, 2, 0,

        4, 6, 7,
        4, 7, 5,

        10, 11, 9,
        10, 9, 8,

        15, 14, 12,
        15, 12, 13,

        17, 19, 18,
        16, 17, 18,

        22, 23, 21,
        22, 21, 20
    };

    vertex_buffer_ = bgfx::createVertexBuffer(bgfx::copy(vertices, sizeof(vertices)), layout_);
    index_buffer_ = bgfx::createIndexBuffer(bgfx::copy(indices, sizeof(indices)));

    u_color_ = bgfx::createUniform("u_color", bgfx::UniformType::Vec4);
}

Outline::~Outline() noexcept {
    bgfx::destroy(program_);
    bgfx::destroy(vertex_buffer_);
    bgfx::destroy(index_buffer_);
    bgfx::destroy(u_color_);
}

void Outline::update(Vec3i pos) {
    bx::mtxTranslate(transform_, pos.x, pos.y, pos.z);
}

void Outline::render() {
    auto a = (std::sin(Timer::getTime() * 5) * 0.5f + 0.5f) * 0.8f;
    float color[4] = { 1.0f, 1.0f, 1.0f, a };
    bgfx::setUniform(u_color_, color);

    bgfx::setTransform(transform_);
    bgfx::setState(
        BGFX_STATE_WRITE_RGB       | 
        BGFX_STATE_DEPTH_TEST_LESS |
        BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA)
    );
    bgfx::setVertexBuffer(0, vertex_buffer_);
    bgfx::setIndexBuffer(index_buffer_);
    bgfx::submit(0, program_);
}