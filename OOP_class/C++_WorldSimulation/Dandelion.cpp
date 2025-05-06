#include "Dandelion.h"

Dandelion::Dandelion(int x, int y, World* world)
    : Plant(x, y, world, 0, 'd') {}

void Dandelion::action() {
    // Trying to spread three times
    for (int i = 0; i < 3; i++) {
        tryToSpread();
    }
}

Organism* Dandelion::createNew(int x, int y) {
    return new Dandelion(x, y, world);
} 