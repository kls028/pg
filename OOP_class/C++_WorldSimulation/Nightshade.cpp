#include "Nightshade.h"

Nightshade::Nightshade(int x, int y, World* world)
    : Plant(x, y, world, 99, 'n') {}

void Nightshade::collision(Organism* other) {
    world->addMessage(std::string(1, other->getSymbol()) + " dies eating deadly Nightshade!");
    world->removeOrganism(other);
    world->removeOrganism(this);
}

Organism* Nightshade::createNew(int x, int y) {
    return new Nightshade(x, y, world);
} 