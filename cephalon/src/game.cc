#include "game.h"
using namespace cephalon;

#include <iostream>

#include "input.h"

Game::Game(int width, int height) {
    camera_.pos = { 0.0f, 3.0f, 0.0f };
    camera_.pitch = -15.0f;
    camera_.aspect = static_cast<float>(width) / height;

    chunks_program_ = LoadProgram("vs_chunks", "fs_chunks");
    focus_program_ = LoadProgram("vs_outline", "fs_outline");

    focus_layout_.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
    .end();

    float vertices[] = {
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f,                       
        
        -0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,

        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        
        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f
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

    focus_vertex_buffer_ = bgfx::createVertexBuffer(bgfx::copy(vertices, sizeof(vertices)), focus_layout_);
    focus_index_buffer_ = bgfx::createIndexBuffer(bgfx::copy(indices, sizeof(indices)));
}

void Game::update(float delta) {
    camera_.update();
    const auto camera_speed = 15.0f;
    if (Input::getKey(Key::kW))
        camera_.pos = bx::add(camera_.pos, bx::mul(camera_.dir, camera_speed * delta));
    if (Input::getKey(Key::kS))
        camera_.pos = bx::sub(camera_.pos, bx::mul(camera_.dir, camera_speed * delta));
    if (Input::getKey(Key::kA))
        camera_.pos = bx::sub(camera_.pos, bx::mul(camera_.right, camera_speed * delta));
    if (Input::getKey(Key::kD))
        camera_.pos = bx::add(camera_.pos, bx::mul(camera_.right, camera_speed * delta));

    world_.update(camera_.pos);

    auto bounding_boxes = world_.getBoundingBoxes(AABB(
        camera_.pos - Vec3i(Config::kDestroyDistance),
        camera_.pos + Vec3i(Config::kDestroyDistance)
    ));
    auto nearest = Config::kDestroyDistance;
    focus_ = false;
    for (auto [pos, bounding_box] : bounding_boxes) {
        if (bounding_box.intersect(camera_.pos, camera_.dir, 0.1f, nearest)) {
            focus_ = true;
            nearest = bx::length(bx::sub(bx::Vec3(pos.x, pos.y, pos.z), camera_.pos));
            focus_pos_ = pos;
        }
    }
}

void Game::onCursorMove(float relative_x, float relative_y) {
    const auto mouseSensitivity = 0.3f;

    auto xoffset = relative_x * mouseSensitivity;
    auto yoffset = relative_y * mouseSensitivity;

    camera_.yaw -= xoffset;
    camera_.pitch -= yoffset;

    if (camera_.pitch > 89.0f)
        camera_.pitch = 89.0f;
    if (camera_.pitch < -89.0f)
        camera_.pitch = -89.0f;
}

void Game::onMouseLeftClick() {
    if (focus_) {
        world_.setBlock(focus_pos_, blocks::kAir);
    }
}

void Game::render() {
    bgfx::setViewTransform(0, camera_.view, camera_.proj);
    world_.render(chunks_program_);

    if (focus_) {
        float transform[16];
        bx::mtxSRT(transform,
            1.001f, 1.001f, 1.001f,
            0.0f, 0.0f, 0.0f,
            focus_pos_.x, focus_pos_.y, focus_pos_.z
        );
        bgfx::setTransform(transform);
        bgfx::setVertexBuffer(0, focus_vertex_buffer_);
        bgfx::setIndexBuffer(focus_index_buffer_);
        bgfx::setState(
            BGFX_STATE_WRITE_RGB       | 
            BGFX_STATE_WRITE_Z         | 
            BGFX_STATE_DEPTH_TEST_LESS |
            BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA)
        );
        bgfx::submit(0, focus_program_);
    }
}

Game::~Game() {
    bgfx::destroy(chunks_program_);
    bgfx::destroy(focus_program_);
    bgfx::destroy(focus_vertex_buffer_);
    bgfx::destroy(focus_index_buffer_);
}