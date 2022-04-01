#include "terrain.h"
using namespace cephalon;

std::map<std::string, Terrain*> Terrain::terrains_;

Flat terrains::flat;
Normal terrains::normal;
Sine terrains::sine;