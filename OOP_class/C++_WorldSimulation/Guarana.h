#ifndef GUARANA_H
#define GUARANA_H

#include "Plant.h"

class Guarana : public Plant {
public:
    Guarana(int x, int y, World* world);
    virtual void collision(Organism* other) override;
    virtual Organism* createNew(int x, int y) override;
}; 
#endif