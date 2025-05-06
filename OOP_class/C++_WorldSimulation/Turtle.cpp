#include "Turtle.h"

Turtle::Turtle(int x, int y, World* world)
    : Animal(x, y, world, 2, 1, 'T') {}

void Turtle::action() {
    // 75% chance to stay in place
    if (rand() % 4 != 0) {
        return;
    }
    Animal::action();
}

void Turtle::collision(Organism* other) {
    // If same species, try to reproduce
    if (other->getSymbol() == this->getSymbol()) {
        auto positions = getNeighboringPositions();
        shufflePositions(positions);
        
        for (const auto& [newX, newY] : positions) {
            if (canMoveTo(newX, newY)) {
                reproduce(newX, newY);
                return;
            }
        }
    }
    // If diffspecies, handle combat
    else {
        Animal* otherAnimal = dynamic_cast<Animal*>(other);
        if (otherAnimal) {
            if (otherAnimal->getStrength() < 5) {
                int attackerX = otherAnimal->getX();
                int attackerY = otherAnimal->getY();
                otherAnimal->setX(attackerX);
                otherAnimal->setY(attackerY);
                world->getOrganismAt(x, y) = nullptr;
                world->getOrganismAt(attackerX, attackerY) = otherAnimal;
            } else {
                // Normal fight
                if (strength >= otherAnimal->getStrength()) {
                    world->removeOrganism(other);
                } else {
                    world->removeOrganism(this);
                }
            }
        }
    }
}

void Turtle::reproduce(int newX, int newY) {
    Turtle* newTurtle = new Turtle(newX, newY, world);
    world->addOrganism(newTurtle);
} 