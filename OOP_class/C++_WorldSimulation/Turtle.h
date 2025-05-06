#ifndef TURTLE_H
#define TURTLE_H

#include "Animal.h"

class Turtle : public Animal {
public:
    Turtle(int x, int y, World* world);
    ~Turtle() = default;
    
    void action() override;
    void collision(Organism* other) override;
    void reproduce(int newX, int newY) override;

    Animal* createNew(int x, int y) override {
        return new Turtle(x, y, world);
    }
};

#endif 