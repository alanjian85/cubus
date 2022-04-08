#include "game.h"
using namespace cephalon;

#include <chrono>
#include <fstream>
#include <vector>

#include <glm/gtc/type_ptr.hpp>
#include <nlohmann/json.hpp>
#include <sndfile.h>
#include <spdlog/spdlog.h>

#include "input.h"

Game::Game(int width, int height) 
    : world_("save/world.db")
{
    camera_.pos = { 0.0f, 50.0f, 0.0f };
    camera_.pitch = -15.0f;
    camera_.aspect = static_cast<float>(width) / height;

    std::ifstream info_file("save/info.json");
    if (info_file.is_open()) {
        info_file >> info_;

        camera_.pos.x = info_["player"]["position"][0];
        camera_.pos.y = info_["player"]["position"][1];
        camera_.pos.z = info_["player"]["position"][2];
        camera_.pitch = info_["player"]["pitch"];
        camera_.yaw = info_["player"]["yaw"];
    } else {
        info_["terrain"] = "normal";
        info_["seed"] = std::chrono::system_clock::now().time_since_epoch().count();
    }
    world_.setTerrain(info_["terrain"]);
    world_.setSeed(info_["seed"]);

    SF_INFO info;
    SNDFILE* file = sf_open("assets/sounds/sound.wav", SFM_READ, &info);
    assert(file);

    std::vector<std::uint16_t> data;
    std::array<short, 4096> buffer;
    std::size_t size;
    while ((size = sf_read_short(file, buffer.data(), buffer.size())) != 0) {
        data.insert(data.cend(), buffer.cbegin(), buffer.cbegin() + size);
    }

    alGenBuffers(1, &buffer_);
    alBufferData(buffer_, 
        info.channels == 1? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16, 
        data.data(), data.size() * sizeof(std::uint16_t), 
        info.samplerate
    );


    alGenSources(1, &source_);
    alSourcei(source_, AL_BUFFER, buffer_);
}

Game::~Game() {
    std::ofstream info_file("save/info.json");
    info_["player"]["position"][0] = camera_.pos.x;
    info_["player"]["position"][1] = camera_.pos.y;
    info_["player"]["position"][2] = camera_.pos.z; 
    info_["player"]["pitch"] = camera_.pitch;
    info_["player"]["yaw"] = camera_.yaw;
    info_file << info_.dump(4);
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

    alListener3f(AL_POSITION, camera_.pos.x, camera_.pos.y, camera_.pos.z);

    intersected_ = world_.intersect(camera_, block_dir_, destroy_pos_);
    if (intersected_) {
        place_pos_ = destroy_pos_ + directionToVector(block_dir_);
        highlight_.update(destroy_pos_, block_dir_);
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
    if (intersected_) {
        world_.setBlock(destroy_pos_, blocks::air);
        spdlog::info("One block at {} is destroyed", fmt::format("({}, {}, {})", destroy_pos_.x, destroy_pos_.y, destroy_pos_.z));
    }
}

void Game::onMouseRightClick() {
    if (intersected_ && place_pos_.y >= 0 && place_pos_.y < Chunk::kVolume.y) {
        auto& block = blocks::wood;
        world_.setBlock(place_pos_, block);
        spdlog::info("One {} block at {} is placed", block.getName(), fmt::format("({}, {}, {})", place_pos_.x, place_pos_.y, place_pos_.z));

        alSource3f(source_, AL_POSITION, place_pos_.x, place_pos_.y, place_pos_.z);
        alSourcePlay(source_);
    }
}

void Game::screenShot() {
    const char* path = "screenshot.png";
    spdlog::info("Screenshot saved to file {}", path);
    bgfx::requestScreenShot(BGFX_INVALID_HANDLE, path);
}

void Game::render() {
    world_.render(camera_);

    if (intersected_) {
        highlight_.render(camera_);
    }
}