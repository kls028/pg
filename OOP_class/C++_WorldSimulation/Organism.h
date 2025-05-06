#ifndef ORGANISM_H
#define ORGANISM_H

class Organism {
protected:
    int x;
    int y;
    char symbol;
    int strength;
    int initiative;
    int age;

public:
    Organism(int x, int y, char symbol);
    virtual ~Organism() = default;

    virtual void action() = 0;
    virtual void collision(Organism* other) = 0;

    // Getters
    int getX() const;
    int getY() const;
    char getSymbol() const;
    int getStrength() const;
    int getInitiative() const;
    int getAge() const;

    // Setters
    void setX(int newX);
    void setY(int newY);
    void setStrength(int newStrength);
    void setInitiative(int newInitiative);
    void incrementAge();
};

#endif
