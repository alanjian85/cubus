#include "block.h"
using namespace cephalon;

std::optional<Atlas> Block::atlas_;
std::map<std::string, const Block*> Block::blocks_;

Air blocks::air;
Dirt blocks::dirt;
Grass blocks::grass;
Sand blocks::sand;
Stone blocks::stone;
Wood blocks::wood;

void Block::init() {
    atlas_.emplace(256, 32);
    blocks::dirt.init();
    blocks::grass.init();
    blocks::sand.init();
    blocks::stone.init();
    blocks::wood.init();
}

void Block::deinit() {
    atlas_.reset();
}