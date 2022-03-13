#include "game.h"
using namespace cephalon;

#include <iostream>

#include "input.h"

Game::Game(int width, int height) {
    camera_.pos = { 0.0f, 3.0f, 0.0f };
    camera_.pitch = -15.0f;
    camera_.aspect = static_cast<float>(width) / height;

    chunks_program_ = LoadProgram("vs_chunks", "fs_chunks");
    outline_program_ = LoadProgram("vs_outline", "fs_outline");

    outline_layout_.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
    .end();

    float vertices[] = {
        1, 0, 0,
        1, 0, 1,
        1, 1, 0,
        1, 1, 1,

        0, 0, 0,
        0, 0, 1,
        0, 1, 0,
        0, 1, 1,                           
        
        0, 1, 0,
        0, 1, 1,
        1, 1, 0,
        1, 1, 1,

        0, 0, 0,
        0, 0, 1,
        1, 0, 0,
        1, 0, 1,

        0, 0, 1,
        0, 1, 1,
        1, 0, 1,
        1, 1, 1,
        
        0, 0, 0,
        0, 1, 0,
        1, 0, 0,
        1, 1, 0
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

    outline_vertex_buffer_ = bgfx::createVertexBuffer(bgfx::copy(vertices, sizeof(vertices)), outline_layout_);
    outline_index_buffer_ = bgfx::createIndexBuffer(bgfx::copy(indices, sizeof(indices)));

    world_.setGenerator([](Vec3i pos, Chunk& chunk) {
        for (int x = 0; x < Chunk::kChunkSize.x; ++x) {
            for (int z = 0; z < Chunk::kChunkSize.x; ++z) {
                chunk.setBlock(Vec3i(x, 1, z), blocks::kGrass);
                chunk.setBlock(Vec3i(x, 0, z), blocks::kDirt);
            }
        }
        chunk.setBlock(Vec3i(0, 2, 0), blocks::kStone);
    });
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

    const auto mouseSensitivity = 0.3f;

    auto xoffset = Input::getRelativeMouseX() * mouseSensitivity;
    auto yoffset = Input::getRelativeMouseY() * mouseSensitivity;

    camera_.yaw -= xoffset;
    camera_.pitch -= yoffset;

    if (camera_.pitch > 89.0f)
        camera_.pitch = 89.0f;
    if (camera_.pitch < -89.0f)
        camera_.pitch = -89.0f;

    world_.update(camera_.pos);

    auto bounding_boxes = world_.getBoundingBoxes(AABB(
        camera_.pos - Vec3i(Config::kDestroyDistance),
        camera_.pos + Vec3i(Config::kDestroyDistance)
    ));
    auto nearest = Config::kDestroyDistance;
    intersect_ = false;
    for (auto [pos, bounding_box] : bounding_boxes) {
        if (bounding_box.intersect(camera_.pos, camera_.dir, 0.1f, nearest)) {
            intersect_ = true;
            nearest = bx::length(bx::sub(bx::Vec3(pos.x, pos.y, pos.z), camera_.pos));
            lookat_pos_ = pos;
        }
    }

    if (intersect_ && Input::getMouseLeft()) {
        world_.setBlock(lookat_pos_, blocks::kAir);
    }
}

void Game::render() {
    bgfx::setViewTransform(0, camera_.view, camera_.proj);
    world_.render(chunks_program_);

    if (intersect_) {
        float transform[16];
        bx::mtxSRT(transform,
            1.1f, 1.1f, 1.1f,
            0, 0, 0,
            lookat_pos_.x, lookat_pos_.y, lookat_pos_.z
        );
        bgfx::setTransform(transform);
        bgfx::setVertexBuffer(0, outline_vertex_buffer_);
        bgfx::setIndexBuffer(outline_index_buffer_);
        bgfx::submit(0, outline_program_);
    }
}

Game::~Game() {
    bgfx::destroy(chunks_program_);
    bgfx::destroy(outline_program_);
    bgfx::destroy(outline_vertex_buffer_);
    bgfx::destroy(outline_index_buffer_);
}