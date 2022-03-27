#include "block.h"
using namespace cephalon;

std::map<std::string, const Block*> Block::blocks_;

Air blocks::air;
Dirt blocks::dirt;
Grass blocks::grass;
Sand blocks::sand;
Stone blocks::stone;
Wood blocks::wood;

void Block::init(Atlas& atlas) {
    blocks::dirt.init(atlas);
    blocks::grass.init(atlas);
    blocks::sand.init(atlas);
    blocks::stone.init(atlas);
    blocks::wood.init(atlas);
}