#include "Fox.h"

Fox::Fox(int x, int y, World* world)
    : Animal(x, y, world, 3, 7, 'F') {}

void Fox::action() {
    hasMoved = false;
    auto positions = getNeighboringPositions();
    shufflePositions(positions);
    
    for (const auto& [newX, newY] : positions) {
        if (canMoveTo(newX, newY)) {
            move(newX, newY);
            hasMoved = true;
            return;
        }
    }
}

bool Fox::canMoveTo(int x, int y) const {
    if (!world->isPositionValid(x, y)) return false;
    
    Organism* target = world->getOrganismAt(x, y);
    //move to empty positions or positions with weaker organisms
    return !target || target->getStrength() <= strength;
}

void Fox::reproduce(int newX, int newY) {
    Fox* newFox = new Fox(newX, newY, world);
    world->addOrganism(newFox);
} 