#include "Plant.h"
#include <random>
#include <algorithm>

Plant::Plant(int x, int y, World* world, int strength, char symbol)
    : Organism(x, y, symbol), world(world) {
    this->strength = strength;
    this->initiative = 0;
}

void Plant::action() {
    tryToSpread();
}

void Plant::collision(Organism* other) {
    // Default plant behavior(being eaten)
    world->addMessage(std::string(1, other->getSymbol()) + " eats " + std::string(1, symbol) + "!");
    world->removeOrganism(this);
}

bool Plant::tryToSpread() {
    if ((rand() % 100) < (SPREAD_CHANCE * 100)) {
        auto positions = getNeighboringPositions();
        if (!positions.empty()) {
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(positions.begin(), positions.end(), g);
            
            // Try to spread to first empty position
            for (const auto& pos : positions) {
                if (!world->isPositionOccupied(pos.first, pos.second)) {
                    Organism* newPlant = createNew(pos.first, pos.second);
                    if (newPlant) {
                        world->addOrganism(newPlant);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

std::vector<std::pair<int, int>> Plant::getNeighboringPositions() const {
    std::vector<std::pair<int, int>> positions;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
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