#ifndef DANDELION_H
#define DANDELION_H

#include "Plant.h"

class Dandelion : public Plant {
public:
    Dandelion(int x, int y, World* world);
    virtual void action() override;  //three spread attempts
    virtual Organism* createNew(int x, int y) override;
}; 
#endif