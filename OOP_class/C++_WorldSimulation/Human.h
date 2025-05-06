#ifndef HUMAN_H
#define HUMAN_H

#include "Animal.h"

class Human : public Animal {
private:
    bool specialAbilityActive;
    int specialAbilityCooldown;
    int specialAbilityDuration;
    int moveDirection; // 0: none, 1: up, 2: right, 3: down, 4: left
    int baseStrength;

public:
    Human(int x, int y, World* world);
    ~Human() = default;
    
    void action() override;
    void collision(Organism* other) override;
    void reproduce(int newX, int newY) override;
    Animal* createNew(int x, int y) override { return new Human(x, y, world); }

    // Getters and setters
    bool isSpecialAbilityActive() const { return specialAbilityActive; }
    int getSpecialAbilityCooldown() const { return specialAbilityCooldown; }
    int getSpecialAbilityDuration() const { return specialAbilityDuration; }
    int getBaseStrength() const { return baseStrength; }

    void setSpecialAbilityActive(bool active) { specialAbilityActive = active; }
    void setSpecialAbilityCooldown(int cooldown) { specialAbilityCooldown = cooldown; }
    void setSpecialAbilityDuration(int duration) { specialAbilityDuration = duration; }
    void setBaseStrength(int strength) { baseStrength = strength; }

    void setMoveDirection(int direction) {
        moveDirection = direction;
    }

    void activateSpecialAbility() {
        if (!specialAbilityActive && specialAbilityCooldown == 0) {
            specialAbilityActive = true;
            specialAbilityDuration = 5;
            strength = 10;
            world->addMessage("Human activates Magiczny Eliksir! Strength increased to 10!");
        }
    }
};

#endif 