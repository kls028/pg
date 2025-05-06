#include "Guarana.h"

Guarana::Guarana(int x, int y, World* world)
    : Plant(x, y, world, 0, 'g') {}

void Guarana::collision(Organism* other) {
    // Increase attacker's strength by 3
    other->setStrength(other->getStrength() + 3);
    world->addMessage(std::string(1, other->getSymbol()) + " eats Guarana and gains +3 strength! His current strength is: " + std::to_string(other->getStrength()));
    // Get eaten
    world->removeOrganism(this);
}

Organism* Guarana::createNew(int x, int y) {
    return new Guarana(x, y, world);
} 