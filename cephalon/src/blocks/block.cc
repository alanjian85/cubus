#include "block.h"
using namespace cephalon;

Air blocks::air;
Dirt blocks::dirt;
Grass blocks::grass;
Stone blocks::stone;
Wood blocks::wood;

void Block::init(Atlas& atlas) {
    blocks::dirt.init(atlas);
    blocks::grass.init(atlas);
    blocks::stone.init(atlas);
    blocks::wood.init(atlas);
}

void Block::cleanup() {
    blocks::dirt.destroy();
    blocks::grass.destroy();
    blocks::stone.destroy();
    blocks::wood.destroy();
    
}