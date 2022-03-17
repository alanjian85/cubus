#include "game.h"
using namespace cephalon;

#include <glm/gtc/type_ptr.hpp>

#include "input.h"

Game::Game(int width, int height) {
    camera_.pos = { 0.0f, 3.0f, 0.0f };
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

    auto bounding_boxes = world_.getBoundingBoxes(AABB(
        camera_.pos - glm::vec3(Config::kDestroyDistance),
        camera_.pos + glm::vec3(Config::kDestroyDistance)
    ));
    auto nearest = Config::kDestroyDistance;
    intersect_ = false;
    for (auto [pos, bounding_box] : bounding_boxes) {
        auto dir = bounding_box.intersect(camera_.pos, camera_.dir, 0.1f, nearest);
        if (dir) {
            intersect_ = true;
            nearest = glm::length(glm::vec3(pos) - camera_.pos);
            intersect_pos_ = pos;
            intersect_dir_ = *dir;

            outline_.update(intersect_pos_, intersect_dir_);
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
    if (intersect_) {
        world_.setBlock(intersect_pos_, blocks::kAir);
    }
}

void Game::onMouseRightClick() {
    glm::ivec3 place_pos;
    switch (intersect_dir_) {
        case Direction::kRight:
            place_pos = intersect_pos_ + glm::ivec3( 1,  0,  0);
            break;
        case Direction::kLeft:
            place_pos = intersect_pos_ + glm::ivec3(-1,  0,  0);
            break;
        case Direction::kTop:
            place_pos = intersect_pos_ + glm::ivec3( 0,  1,  0);
            break;
        case Direction::kBottom:
            place_pos = intersect_pos_ + glm::ivec3( 0, -1,  0);
            break;
        case Direction::kBack:
            place_pos = intersect_pos_ + glm::ivec3( 0,  0,  1);
            break;
        case Direction::kFront:
            place_pos = intersect_pos_ + glm::ivec3( 0,  0, -1);
            break;
    }

    
    if (world_.getBlock(place_pos).isAir()) {
        world_.setBlock(place_pos, blocks::kWood);
    }
}

void Game::render() {
    bgfx::setViewTransform(0, glm::value_ptr(camera_.view), glm::value_ptr(camera_.proj));
    world_.render();

    if (intersect_) {
        outline_.render();
    }
}