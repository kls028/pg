#ifndef FOX_H
#define FOX_H

#include "Animal.h"

class Fox : public Animal {
public:
    Fox(int x, int y, World* world);
    ~Fox() = default;
    
    void action() override;
    void reproduce(int newX, int newY) override;
    bool canMoveTo(int x, int y) const override;

    Animal* createNew(int x, int y) override {
        return new Fox(x, y, world);
    }
};

#endif 