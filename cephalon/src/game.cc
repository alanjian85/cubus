#include "game.h"
using namespace cephalon;

#include "input.h"

Game::Game(int width, int height) {
    camera_.setPosition(bx::Vec3(0.0f, 0.0f, -3.0f));
    camera_.setAspect(static_cast<float>(width) / height);

    chunks_program_ = LoadProgram("vs_chunks", "fs_chunks");

    for (int x = 0; x < 50; ++x) {
        for (int y = 0; y < 50; ++y) {
            for (int z = 0; z < 50; ++z) {
                if (x % 2 == 0 && y % 2 == 0 && z % 2 == 0)
                    world_.setBlock({x, y, z}, blocks::kDirt);
                else
                    world_.setBlock({x, y, z}, blocks::kStone);
            }
        }
    }
}

void Game::update(float delta) {
    camera_.update();
    const auto camera_speed = 5.0f;
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

    world_.update();
}

void Game::render() {
    bgfx::setViewTransform(0, camera_.getView(), camera_.getProj());
    world_.render(chunks_program_);
}

Game::~Game() {
    bgfx::destroy(chunks_program_);
}