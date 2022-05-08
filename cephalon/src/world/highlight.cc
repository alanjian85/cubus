#include "highlight.h"
using namespace cephalon;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "game/timer.h"
#include "utils/assets.h"

Highlight::Highlight() {
    program_ = LoadProgram("vs_highlight", "fs_highlight");

    layout_.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
    .end();

    float vertices[] = {
         0.525f, -0.475f, -0.475f,
         0.525f, -0.475f,  0.475f,
         0.525f,  0.475f, -0.475f,
         0.525f,  0.475f,  0.475f,

        -0.525f, -0.475f, -0.475f,
        -0.525f, -0.475f,  0.475f,
        -0.525f,  0.475f, -0.475f,
        -0.525f,  0.475f,  0.475f,                       

        -0.475f,  0.525f, -0.475f,
        -0.475f,  0.525f,  0.475f,
         0.475f,  0.525f, -0.475f,
         0.475f,  0.525f,  0.475f,

        -0.475f, -0.525f, -0.475f,
        -0.475f, -0.525f,  0.475f,
         0.475f, -0.525f, -0.475f,
         0.475f, -0.525f,  0.475f,

        -0.475f, -0.475f,  0.525f,
        -0.475f,  0.475f,  0.525f,
         0.475f, -0.475f,  0.525f,
         0.475f,  0.475f,  0.525f,

        -0.475f, -0.475f, -0.525f,
        -0.475f,  0.475f, -0.525f,
         0.475f, -0.475f, -0.525f,
         0.475f,  0.475f, -0.525f
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
    first_index_ = 0;
}

Highlight::~Highlight() noexcept {
    bgfx::destroy(program_);
    bgfx::destroy(vertex_buffer_);
    bgfx::destroy(index_buffer_);
    bgfx::destroy(u_color_);
}

void Highlight::update(glm::ivec3 pos, Direction dir) {
    switch (dir) {
        case Direction::kRight:
            first_index_ = 0;
            break;
        case Direction::kLeft:
            first_index_ = 6;
            break;
        case Direction::kTop:
            first_index_ = 12;
            break;
        case Direction::kBottom:
            first_index_ = 18;
            break;
        case Direction::kBack:
            first_index_ = 24;
            break;
        case Direction::kFront:
            first_index_ = 30;
            break;
    }

    transform_ = glm::translate(glm::mat4(1.0f), glm::vec3(pos));
}

void Highlight::render(PerspectiveCamera cam) {
    bgfx::setViewTransform(0, glm::value_ptr(cam.view), glm::value_ptr(cam.proj));

    auto a = (std::sin(Timer::getTime() * 5) * 0.5f + 0.5f) * 0.5f + 0.25f;
    float color[4] = { 1.0f, 1.0f, 1.0f, a };
    bgfx::setUniform(u_color_, color);

    bgfx::setTransform(glm::value_ptr(transform_));
    bgfx::setState(
        BGFX_STATE_WRITE_RGB       | 
        BGFX_STATE_DEPTH_TEST_LESS |
        BGFX_STATE_MSAA            |
        BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA)
    );
    bgfx::setVertexBuffer(0, vertex_buffer_);
    bgfx::setIndexBuffer(index_buffer_, first_index_, 6);
    bgfx::submit(0, program_);
}