#include "Human.h"

Human::Human(int x, int y, World* world)
    : Animal(x, y, world, 5, 4, 'H') {
    baseStrength = 5;
    specialAbilityActive = false;
    specialAbilityCooldown = 0;
    specialAbilityDuration = 0;
    moveDirection = 0;
}

void Human::action() {
    if (moveDirection != 0) {
        int newX = x;
        int newY = y;

        switch (moveDirection) {
            case 1: newY--; break; // up
            case 2: newX++; break; // right
            case 3: newY++; break; // down
            case 4: newX--; break; // left
        }

        if (world->isPositionValid(newX, newY)) {
            world->getOrganismAt(x, y) = nullptr;
            x = newX;
            y = newY;
            world->getOrganismAt(x, y) = this;
        }
        moveDirection = 0;
    }

    // Update special ability status
    if (specialAbilityActive) {
        specialAbilityDuration--;
        if (specialAbilityDuration <= 0) {
            specialAbilityActive = false;
            specialAbilityCooldown = 5;
            strength = baseStrength; // Reset to original strength
            world->addMessage("Human's Magiczny Eliksir wears off! Strength returns to " + std::to_string(baseStrength));
        } else {
            strength = 10 - (5 - specialAbilityDuration); // Decrease strength by 1 each turn
            world->addMessage("Human's Magiczny Eliksir strength: " + std::to_string(strength));
        }
    } else if (specialAbilityCooldown > 0) {
        specialAbilityCooldown--;
    }
}

void Human::collision(Organism* other) {
    // Check if other is a plant
    if (std::islower(other->getSymbol())) {
        other->collision(this);
    } else {
        // Handle combat with other animals
        if (specialAbilityActive) {
            world->addMessage(std::string(1, getSymbol()) + " kills " + std::string(1, other->getSymbol()) + " (special ability)");
            world->removeOrganism(other);
        } else {
            // Normal fight
            if (strength >= other->getStrength()) {
                world->addMessage(std::string(1, getSymbol()) + " kills " + std::string(1, other->getSymbol()));
                world->removeOrganism(other);
            } else {
                world->addMessage(std::string(1, other->getSymbol()) + " kills " + std::string(1, getSymbol()));
                world->removeOrganism(this);
            }
        }
    }
}