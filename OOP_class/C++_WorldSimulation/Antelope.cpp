#include "Antelope.h"

Antelope::Antelope(int x, int y, World* world)
    : Animal(x, y, world, 4, 4, 'A') {}

void Antelope::action() {
    hasMoved = false;
    auto positions = getExtendedNeighboringPositions();
    shufflePositions(positions);
    
    for (const auto& [newX, newY] : positions) {
        if (canMoveTo(newX, newY)) {
            move(newX, newY);
            hasMoved = true;
            return;
        }
    }
}

void Antelope::collision(Organism* other) {
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
    // If diffspecies, combat
    else {
        // 50% chance to escape
        if (rand() % 2 == 0) {
            auto positions = getNeighboringPositions();
            shufflePositions(positions);
            
            for (const auto& [newX, newY] : positions) {
                if (canMoveTo(newX, newY)) {
                    world->getOrganismAt(x, y) = nullptr;
                    x = newX;
                    y = newY;
                    world->getOrganismAt(x, y) = this;
                    return;
                }
            }
        }
        
        // If couldn't escape normal fight
        Animal* otherAnimal = dynamic_cast<Animal*>(other);
        if (otherAnimal) {
            if (strength >= otherAnimal->getStrength()) {
                world->removeOrganism(other);
            } else {
                world->removeOrganism(this);
            }
        }
    }
}

void Antelope::reproduce(int newX, int newY) {
    Antelope* newAntelope = new Antelope(newX, newY, world);
    world->addOrganism(newAntelope);
}

std::vector<std::pair<int, int>> Antelope::getExtendedNeighboringPositions() const {
    std::vector<std::pair<int, int>> positions;
    for (int i = -2; i <= 2; i++) {
        for (int j = -2; j <= 2; j++) {
            if (i == 0 && j == 0) continue;
            int newX = x + i;
            int newY = y + j;
            if (world->isPositionValid(newX, newY)) {
                positions.emplace_back(newX, newY);
            }
        }
    }
    return positions;
} 