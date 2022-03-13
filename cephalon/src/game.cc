#include "game.h"
using namespace cephalon;

#include <iostream>

#include "input.h"

Game::Game(int width, int height) {
    camera_.pos = { 0.0f, 3.0f, 0.0f };
    camera_.pitch = -15.0f;
    camera_.aspect = static_cast<float>(width) / height;

    chunks_program_ = LoadProgram("vs_chunks", "fs_chunks");

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
    auto intersect = false;
    Vec3i lookat_block;
    for (auto [pos, bounding_box] : bounding_boxes) {
        if (bounding_box.intersect(camera_.pos, camera_.dir, 0.1f, nearest)) {
            intersect = true;
            nearest = bx::length(bx::sub(bx::Vec3(pos.x, pos.y, pos.z), camera_.pos));
            lookat_block = pos;
        }
    }

    if (intersect && Input::getMouseLeft()) {
        world_.setBlock(lookat_block, blocks::kAir);
    }
}

void Game::render() {
    bgfx::setViewTransform(0, camera_.view, camera_.proj);
    world_.render(chunks_program_);
}

Game::~Game() {
    bgfx::destroy(chunks_program_);
}