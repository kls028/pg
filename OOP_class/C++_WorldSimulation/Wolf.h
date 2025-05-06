#ifndef WOLF_H
#define WOLF_H

#include "Animal.h"

class Wolf : public Animal {
public:
    Wolf(int x, int y, World* world);
    ~Wolf() = default;
    
    void reproduce(int newX, int newY) override;

    Animal* createNew(int x, int y) override {
        return new Wolf(x, y, world);
    }
};

#endif 