#include "Sheep.h"

Sheep::Sheep(int x, int y, World* world)
    : Animal(x, y, world, 4, 4, 'S') {}

void Sheep::reproduce(int newX, int newY) {
    Sheep* newSheep = new Sheep(newX, newY, world);
    world->addOrganism(newSheep);
} 