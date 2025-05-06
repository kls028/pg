#ifndef PLANT_H
#define PLANT_H

#include "Organism.h"
#include "World.h"

class Plant : public Organism {
protected:
    World* world;
    static constexpr double SPREAD_CHANCE = 0.03;

public:
    Plant(int x, int y, World* world, int strength, char symbol);
    virtual ~Plant() = default;

    // Override Organism methods
    virtual void action() override;
    virtual void collision(Organism* other) override;

    // Plant specific methods
    virtual Organism* createNew(int x, int y) = 0;
    std::vector<std::pair<int, int>> getNeighboringPositions() const;
    bool tryToSpread();
}; 
#endif