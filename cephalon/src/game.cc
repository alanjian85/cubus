#include "game.h"
using namespace cephalon;

#include "input.h"

void Game::init(int width, int height) {
    camera_.setPosition(bx::Vec3(0.0f, 0.0f, -3.0f));
    camera_.setAspect(static_cast<float>(width) / height);

    chunks_program_ = LoadProgram("vs_chunks", "fs_chunks");
    chunk_.setBlock(0, 0, 0, blocks::kDirt);
}

void Game::update(float delta) {
    camera_.update();
    const auto camera_speed = 2.5f;
    if (Input::getKey(Key::kW))
        camera_.setPosition(bx::add(camera_.getPosition(), bx::mul(camera_.getDirection(), camera_speed * delta)));
    if (Input::getKey(Key::kS))
        camera_.setPosition(bx::sub(camera_.getPosition(), bx::mul(camera_.getDirection(), camera_speed * delta)));
    if (Input::getKey(Key::kA))
        camera_.setPosition(bx::sub(camera_.getPosition(), bx::mul(camera_.getRight(), camera_speed * delta)));
    if (Input::getKey(Key::kD))
        camera_.setPosition(bx::add(camera_.getPosition(), bx::mul(camera_.getRight(), camera_speed * delta)));

    const auto mouseSensitivity = 0.2f;

    auto xoffset = Input::getRelativeMouseX() * mouseSensitivity;
    auto yoffset = Input::getRelativeMouseY() * mouseSensitivity;

    camera_.setYaw(camera_.getYaw() - xoffset);
    camera_.setPitch(camera_.getPitch() - yoffset);

    if (camera_.getPitch() > 89.0f)
        camera_.setPitch(89.0f);
    if (camera_.getPitch() < -89.0f)
        camera_.setPitch(-89.0f);

    chunk_.update();
}

void Game::render() {
    bgfx::setViewTransform(0, camera_.getView(), camera_.getProj());
    bgfx::setVertexBuffer(0, chunk_.getVertexBuffer());
    bgfx::setIndexBuffer(chunk_.getIndexBuffer());
    bgfx::submit(0, chunks_program_);
}