#ifndef ANIMAL_H
#define ANIMAL_H

#include "Organism.h"
#include "World.h"
#include <vector>
#include <random>

class Animal : public Organism {
protected:
    World* world;
    bool hasMoved;

public:
    Animal(int x, int y, World* world, int strength, int initiative, char symbol);
    virtual ~Animal() = default;

    // Common methods for all animals
    virtual void action() override;
    virtual void collision(Organism* other) override;
    virtual void move(int newX, int newY);
    virtual bool canMoveTo(int x, int y) const;
    virtual void reproduce(int newX, int newY);
    
    // Getters
    bool getHasMoved() const { return hasMoved; }
    
    // Setters
    void setHasMoved(bool moved) { hasMoved = moved; }
    
    // Helper methods
    std::vector<std::pair<int, int>> getNeighboringPositions() const;
    std::pair<int, int> getRandomNeighboringPosition() const;
    void shufflePositions(std::vector<std::pair<int, int>>& positions) const;

    virtual Animal* createNew(int x, int y) = 0;
};

#endif 