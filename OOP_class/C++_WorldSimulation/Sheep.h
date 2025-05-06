#ifndef SHEEP_H
#define SHEEP_H

#include "Animal.h"

class Sheep : public Animal {
public:
    Sheep(int x, int y, World* world);
    ~Sheep() = default;
    
    void reproduce(int newX, int newY) override;

    Animal* createNew(int x, int y) override {
        return new Sheep(x, y, world);
    }
};

#endif 