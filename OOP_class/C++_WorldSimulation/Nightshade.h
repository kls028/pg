#ifndef NIGHTSHADE_H
#define NIGHTSHADE_H

#include "Plant.h"

class Nightshade : public Plant {
public:
    Nightshade(int x, int y, World* world);
    virtual void collision(Organism* other) override;  // Override to kill the attacker
    virtual Organism* createNew(int x, int y) override;
}; 
#endif