#include "game.h"
using namespace cephalon;

#include <glm/gtc/type_ptr.hpp>

#include "input.h"

Game::Game(int width, int height) {
    camera_.pos = { 0.0f, 8.0f, 0.0f };
    camera_.pitch = -15.0f;
    camera_.aspect = static_cast<float>(width) / height;
}

void Game::update(float delta) {
    camera_.update();
    const auto camera_speed = 15.0f;
    if (Input::getKey(Key::kW))
        camera_.pos = camera_.pos + camera_.dir * camera_speed * delta;
    if (Input::getKey(Key::kS))
        camera_.pos = camera_.pos - camera_.dir * camera_speed * delta;
    if (Input::getKey(Key::kA))
        camera_.pos = camera_.pos - camera_.right * camera_speed * delta;
    if (Input::getKey(Key::kD))
        camera_.pos = camera_.pos + camera_.right * camera_speed * delta;

    world_.update(camera_.pos);

    intersected_ = world_.intersect(camera_, block_dir_, destroy_pos_);
    if (intersected_) {
        place_pos_ = destroy_pos_ + directionToVector(block_dir_);
        outline_.update(destroy_pos_, block_dir_);
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
    if (intersected_)
        world_.setBlock(destroy_pos_, blocks::air);
}

void Game::onMouseRightClick() {
    if (place_pos_.y >= 0 && place_pos_.y < Chunk::kVolume.y)
        world_.setBlock(place_pos_, blocks::wood);
}

void Game::render() {
    world_.render(camera_);

    if (intersected_) {
        outline_.render(camera_);
    }
}