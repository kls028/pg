#ifndef ANTELOPE_H
#define ANTELOPE_H

#include "Animal.h"

class Antelope : public Animal {
public:
    Antelope(int x, int y, World* world);
    ~Antelope() = default;
    
    void action() override;
    void collision(Organism* other) override;
    void reproduce(int newX, int newY) override;
    Animal* createNew(int x, int y) override { return new Antelope(x, y, world); }
    
private:
    std::vector<std::pair<int, int>> getExtendedNeighboringPositions() const;
};

#endif 