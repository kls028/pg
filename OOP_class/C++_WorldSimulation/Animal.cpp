#include "Animal.h"
#include "World.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>

Animal::Animal(int x, int y, World* world, int strength, int initiative, char symbol)
    : Organism(x, y, symbol), world(world), hasMoved(false) {
    this->strength = strength;
    this->initiative = initiative;
}

void Animal::shufflePositions(std::vector<std::pair<int, int>>& positions) const {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(positions.begin(), positions.end(), g);
}

void Animal::action() {
    hasMoved = false;
    auto [newX, newY] = getRandomNeighboringPosition();
    
    if (canMoveTo(newX, newY)) {
        move(newX, newY);
        hasMoved = true;
    }
}

void Animal::collision(Organism* other) {
    // If same species, try to reproduce
    if (getSymbol() == other->getSymbol()) {
        std::vector<std::pair<int, int>> neighbors = getNeighboringPositions();

        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(neighbors.begin(), neighbors.end(), g);
        
        // Reproducing in first empty neighboring position
        for (const auto& pos : neighbors) {
            if (!world->isPositionOccupied(pos.first, pos.second)) {
                Organism* newOrg = createNew(pos.first, pos.second);
                if (newOrg) {
                    world->addOrganism(newOrg);
                    world->addMessage("New " + std::string(1, getSymbol()) + " born!");
                    break;
                }
            }
        }
    } else {
        if (std::islower(other->getSymbol())) {
            other->collision(this);
        } else {
            // Battle between diff species
            if (getStrength() > other->getStrength()) {
                world->addMessage(std::string(1, getSymbol()) + " kills " + std::string(1, other->getSymbol()));
                world->removeOrganism(other);
            } else if (getStrength() < other->getStrength()) {
                world->addMessage(std::string(1, other->getSymbol()) + " kills " + std::string(1, getSymbol()));
                world->removeOrganism(this);
            } else {
                // Equal strength - attacker wins
                if (this->getAge() > other->getAge()) {
                    world->addMessage(std::string(1, getSymbol()) + " kills " + std::string(1, other->getSymbol()) + " (age advantage)");
                    world->removeOrganism(other);
                } else {
                    world->addMessage(std::string(1, other->getSymbol()) + " kills " + std::string(1, getSymbol()));
                    world->removeOrganism(this);
                }
            }
        }
    }
}

void Animal::move(int newX, int newY) {
    if (world->isPositionValid(newX, newY)) {
        world->getOrganismAt(x, y) = nullptr;
        x = newX;
        y = newY;
        world->getOrganismAt(x, y) = this;
    }
}

bool Animal::canMoveTo(int x, int y) const {
    return world->isPositionValid(x, y) && !world->isPositionOccupied(x, y);
}

void Animal::reproduce(int newX, int newY) {
    // to override
}

std::vector<std::pair<int, int>> Animal::getNeighboringPositions() const {
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

std::pair<int, int> Animal::getRandomNeighboringPosition() const {
    auto positions = getNeighboringPositions();
    if (positions.empty()) return {x, y};
    return positions[rand() % positions.size()];
} 