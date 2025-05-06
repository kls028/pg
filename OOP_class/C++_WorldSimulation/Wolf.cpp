#include "Wolf.h"

Wolf::Wolf(int x, int y, World* world)
    : Animal(x, y, world, 9, 5, 'W') {}

void Wolf::reproduce(int newX, int newY) {
    Wolf* newWolf = new Wolf(newX, newY, world);
    world->addOrganism(newWolf);
} 