#include "Organism.h"

Organism::Organism(int x, int y, char symbol)
    : x(x), y(y), symbol(symbol), strength(0), initiative(0), age(0) {}

int Organism::getX() const { return x; }
int Organism::getY() const { return y; }
char Organism::getSymbol() const { return symbol; }
int Organism::getStrength() const { return strength; }
int Organism::getInitiative() const { return initiative; }
int Organism::getAge() const { return age; }

void Organism::setX(int newX) { x = newX; }
void Organism::setY(int newY) { y = newY; }
void Organism::setStrength(int newStrength) { strength = newStrength; }
void Organism::setInitiative(int newInitiative) { initiative = newInitiative; }
void Organism::incrementAge() { age++; } 