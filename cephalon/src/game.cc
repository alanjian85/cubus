#include "game.h"
using namespace cephalon;

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
        camera_.pos = bx::add(camera_.pos, bx::mul(camera_.dir, camera_speed * delta));
    if (Input::getKey(Key::kS))
        camera_.pos = bx::sub(camera_.pos, bx::mul(camera_.dir, camera_speed * delta));
    if (Input::getKey(Key::kA))
        camera_.pos = bx::sub(camera_.pos, bx::mul(camera_.right, camera_speed * delta));
    if (Input::getKey(Key::kD))
        camera_.pos = bx::add(camera_.pos, bx::mul(camera_.right, camera_speed * delta));

    world_.update(camera_.pos);

    auto bounding_boxes = world_.getBoundingBoxes(AABB(
        Vec3i(camera_.pos) - Vec3i(Config::kDestroyDistance),
        Vec3i(camera_.pos) + Vec3i(Config::kDestroyDistance)
    ));
    auto nearest = Config::kDestroyDistance;
    intersect_ = false;
    for (auto [pos, bounding_box] : bounding_boxes) {
        auto dir = bounding_box.intersect(camera_.pos, camera_.dir, 0.1f, nearest);
        if (dir) {
            intersect_ = true;
            nearest = bx::length(bx::sub(bx::Vec3(pos.x, pos.y, pos.z), camera_.pos));
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
    Vec3i place_pos;
    switch (intersect_dir_) {
        case Direction::kRight:
            place_pos = intersect_pos_ + Vec3i( 1,  0,  0);
            break;
        case Direction::kLeft:
            place_pos = intersect_pos_ + Vec3i(-1,  0,  0);
            break;
        case Direction::kTop:
            place_pos = intersect_pos_ + Vec3i( 0,  1,  0);
            break;
        case Direction::kBottom:
            place_pos = intersect_pos_ + Vec3i( 0, -1,  0);
            break;
        case Direction::kBack:
            place_pos = intersect_pos_ + Vec3i( 0,  0,  1);
            break;
        case Direction::kFront:
            place_pos = intersect_pos_ + Vec3i( 0,  0, -1);
            break;
    }

    if (&world_.getBlock(place_pos) == &blocks::kAir) {
        world_.setBlock(place_pos, blocks::kStone);
    }
}

void Game::render() {
    bgfx::setViewTransform(0, camera_.view, camera_.proj);
    world_.render();

    if (intersect_) {
        outline_.render();
    }
}