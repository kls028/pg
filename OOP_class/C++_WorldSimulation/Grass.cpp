#include "Grass.h"

Grass::Grass(int x, int y, World* world)
    : Plant(x, y, world, 0, 't') {}

Organism* Grass::createNew(int x, int y) {
    return new Grass(x, y, world);
} 