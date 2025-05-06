#ifndef HOGWEED_H
#define HOGWEED_H

#include "Plant.h"

class Hogweed : public Plant {
public:
    Hogweed(int x, int y, World* world);
    virtual void action() override;
    virtual void collision(Organism* other) override;
    virtual Organism* createNew(int x, int y) override;
private:
    void killAdjacentAnimals();
}; 
#endif