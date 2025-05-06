#ifndef GRASS_H
#define GRASS_H

#include "Plant.h"

class Grass : public Plant {
public:
    Grass(int x, int y, World* world);
    virtual Organism* createNew(int x, int y) override;
}; 
#endif