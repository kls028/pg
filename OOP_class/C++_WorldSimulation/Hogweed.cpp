#include "Hogweed.h"
#include "Animal.h"

Hogweed::Hogweed(int x, int y, World* world)
    : Plant(x, y, world, 10, 'h') {}

void Hogweed::action() {
    //Trying to spread
    Plant::action();
    killAdjacentAnimals();
}

void Hogweed::collision(Organism* other) {
    world->addMessage(std::string(1, other->getSymbol()) + " dies trying to eat Hogweed!");
    // Kill both the attacker and this plant
    world->removeOrganism(other);
    world->removeOrganism(this);
}

Organism* Hogweed::createNew(int x, int y) {
    return new Hogweed(x, y, world);
}

void Hogweed::killAdjacentAnimals() {
    auto positions = getNeighboringPositions();
    for (const auto& pos : positions) {
        Organism* org = world->getOrganismAt(pos.first, pos.second);
        if (org != nullptr) {
            Animal* animal = dynamic_cast<Animal*>(org);
            if (animal != nullptr) {
                world->addMessage(std::string(1, animal->getSymbol()) + " dies from Hogweed's toxins!");
                world->removeOrganism(animal);
            }
        }
    }
} 