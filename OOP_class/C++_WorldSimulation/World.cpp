#include "World.h"
#include <algorithm>
#include <ncurses.h>
#include <ctime>
#include <string>
#include <set>
#include <fstream>
#include "Human.h"
#include "Wolf.h"
#include "Sheep.h"
#include "Fox.h"
#include "Turtle.h"
#include "Antelope.h"
#include "Plant.h"
#include "Grass.h"
#include "Dandelion.h"
#include "Guarana.h"
#include "Nightshade.h"
#include "Hogweed.h"

World::World() : simulationEnded(false) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            board[i][j] = nullptr;
        }
    }
    for (int i = 0; i < 6; i++) {
        originalCounts[i] = 0;
    }
}

World::~World() {
    for (auto organism : organisms) {
        delete organism;
    }
}

bool World::checkSimulationEnd() {
    std::set<char> animalTypes;
    
    for (auto* org : organisms) {
        char symbol = org->getSymbol();
        if (std::isupper(symbol)) {
            animalTypes.insert(symbol);
        }
    }

    // If only one type of animal remains
    if (animalTypes.size() == 1) {
        char lastAnimal = *animalTypes.begin();
        std::string winner;
        switch(lastAnimal) {
            case 'H': winner = "Human"; break;
            case 'W': winner = "Wolf"; break;
            case 'S': winner = "Sheep"; break;
            case 'F': winner = "Fox"; break;
            case 'T': winner = "Turtle"; break;
            case 'A': winner = "Antelope"; break;
            default: winner = "Unknown"; break;
        }
        addMessage("Simulation ended!");
        addMessage(winner + " species wins!");
        simulationEnded = true;
        return true;
    }
    
    // If no animals left at all
    if (animalTypes.empty()) {
        addMessage("Simulation ended!");
        addMessage("All animals have died!");
        simulationEnded = true;
        return true;
    }

    return false;
}

void World::executeRound() {
    if (simulationEnded) {
        return;
    }

    // Sorting organisms by initiative (descending) and age (descending)
    std::sort(organisms.begin(), organisms.end(),
        [](const Organism* a, const Organism* b) {
            if (a->getInitiative() != b->getInitiative()) {
                return a->getInitiative() > b->getInitiative();
            }
            return a->getAge() > b->getAge();
        });

    std::vector<Organism*> organismsCopy = organisms;
    std::vector<std::pair<Organism*, Organism*>> collisions;

    for (auto organism : organismsCopy) {
        if (std::find(organisms.begin(), organisms.end(), organism) == organisms.end()) {
            continue;
        }

        // Skip plants when checking for collisions
        if (!std::isupper(organism->getSymbol())) {
            continue;
        }
        int x = organism->getX();
        int y = organism->getY();

        // Check for collisions with adjacent organisms
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                if (dx == 0 && dy == 0) continue;
                
                int newX = x + dx;
                int newY = y + dy;
                
                // Check if position is within bounds
                if (newX >= 0 && newX < width && newY >= 0 && newY < height) {
                    Organism* other = getOrganismAt(newX, newY);
                    if (other != nullptr && other != organism) {
                        // Add to collisions list if not already there
                        bool alreadyExists = false;
                        for (const auto& collision : collisions) {
                            if ((collision.first == organism && collision.second == other) ||
                                (collision.first == other && collision.second == organism)) {
                                alreadyExists = true;
                                break;
                            }
                        }
                        if (!alreadyExists) {
                            collisions.push_back({organism, other});
                        }
                    }
                }
            }
        }
    }

    for (auto organism : organismsCopy) {
        if (std::find(organisms.begin(), organisms.end(), organism) == organisms.end()) {
            continue;
        }

        organism->action();
        organism->incrementAge();
    }

    for (const auto& collision : collisions) {
        Organism* org1 = collision.first;
        Organism* org2 = collision.second;
        
        // Check if both organisms still exist
        if (std::find(organisms.begin(), organisms.end(), org1) != organisms.end() &&
            std::find(organisms.begin(), organisms.end(), org2) != organisms.end()) {
            org1->collision(org2);
        }
    }

    checkSimulationEnd();
}

int World::getSpeciesCount(char symbol) const {
    int count = 0;
    for (const auto& organism : organisms) {
        if (organism->getSymbol() == symbol) {
            count++;
        }
    }
    return count;
}

int World::getOriginalCount(char symbol) const {
    switch(symbol) {
        case 'H': return originalCounts[0];
        case 'W': return originalCounts[1];
        case 'S': return originalCounts[2];
        case 'F': return originalCounts[3];
        case 'T': return originalCounts[4];
        case 'A': return originalCounts[5];
        default: return 0;
    }
}

int World::getRunningTime() const {
    return (time(nullptr) - startTime);
}

void World::drawStatusBar() {
    mvprintw(33, 0, "Piotr Skierka | 203246");

    const char symbols[] = {'H', 'W', 'S', 'F', 'T', 'A'};
    int x = 30;
    
    for (char symbol : symbols) {
        int current = getSpeciesCount(symbol);
        int original = getOriginalCount(symbol);
        

        attron(COLOR_PAIR(1));
        mvprintw(22, x, "%c:%d", symbol, original);

        mvprintw(22, x + 3, "(%d)", current);
        attroff(COLOR_PAIR(1));
        
        mvprintw(22, x + 7, " ");
        x += 8;
    }
    
    mvprintw(22, x, "| ");

    int seconds = getRunningTime();
    mvprintw(22, x + 2, "Time: %02d:%02d:%02d",
        seconds / 3600,
        (seconds % 3600) / 60,
        seconds % 60);
}

void World::drawWorld() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (board[y][x] != nullptr) {
                char symbol = board[y][x]->getSymbol();
                int colorPair;

                switch (symbol) {
                    case 'H': colorPair = 1; break;  // Human
                    case 'W': colorPair = 2; break;  // Wolf
                    case 'S': colorPair = 3; break;  // Sheep
                    case 'F': colorPair = 4; break;  // Fox
                    case 'T': colorPair = 5; break;  // Turtle
                    case 'A': colorPair = 6; break;  // Antelope
                    case 't': colorPair = 7; break;  // Grass (trawa)
                    case 'd': colorPair = 8; break;  // Dandelion (mlecz)
                    case 'g': colorPair = 9; break;  // Guarana
                    case 'n': colorPair = 10; break; // Nightshade (wilcze jagody)
                    case 'h': colorPair = 11; break; // Hogweed (barszcz)
                    default: colorPair = 1;
                }
                
                attron(COLOR_PAIR(colorPair));
                mvprintw(y + 1, x + 1, "%c", symbol);
                attroff(COLOR_PAIR(colorPair));
            } else {
                mvprintw(y + 1, x + 1, " ");
            }
        }
    }

    drawMessageLog();

    drawStatusBar();
}

void World::addOrganism(Organism* organism) {
    if (organism == nullptr) return;
    
    int x = organism->getX();
    int y = organism->getY();
    
    if (x >= 0 && x < width && y >= 0 && y < height && board[y][x] == nullptr) {
        board[y][x] = organism;
        organisms.push_back(organism);

        char symbol = organism->getSymbol();
        switch(symbol) {
            case 'H': originalCounts[0]++; break;
            case 'W': originalCounts[1]++; break;
            case 'S': originalCounts[2]++; break;
            case 'F': originalCounts[3]++; break;
            case 'T': originalCounts[4]++; break;
            case 'A': originalCounts[5]++; break;
        }
    } else {
        delete organism;
    }
}

void World::removeOrganism(Organism* organism) {
    if (organism == nullptr) return;

    board[organism->getY()][organism->getX()] = nullptr;

    auto it = std::find(organisms.begin(), organisms.end(), organism);
    if (it != organisms.end()) {
        organisms.erase(it);
    }

    delete organism;
}

bool World::isPositionValid(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

bool World::isPositionOccupied(int x, int y) const {
    return isPositionValid(x, y) && board[y][x] != nullptr;
}

Organism* World::getOrganismAt(int x, int y) const {
    return isPositionValid(x, y) ? board[y][x] : nullptr;
}

Organism*& World::getOrganismAt(int x, int y) {
    static Organism* null_org = nullptr;
    return isPositionValid(x, y) ? board[y][x] : null_org;
}

void World::addMessage(const std::string& msg) {
    messageLog.push(msg);
    if (messageLog.size() > MAX_MESSAGES) {
        messageLog.pop();
    }
}

void World::drawMessageLog() {
    attron(COLOR_PAIR(1));
    mvprintw(1, 25, "Event Log:");
    attroff(COLOR_PAIR(1));

    std::queue<std::string> tempLog = messageLog;

    int y = 2;
    while (!tempLog.empty() && y < 20) {
        mvprintw(y++, 25, "%s", tempLog.front().c_str());
        tempLog.pop();
    }
}

void World::clearWorld() {

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            board[i][j] = nullptr;
        }
    }

    for (auto organism : organisms) {
        delete organism;
    }
    organisms.clear();

    while (!messageLog.empty()) {
        messageLog.pop();
    }

    for (int i = 0; i < 6; i++) {
        originalCounts[i] = 0;
    }
}

bool World::saveGame(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        addMessage("Failed to save game!");
        return false;
    }

    // Save basic world state
    file.write(reinterpret_cast<const char*>(&startTime), sizeof(startTime));
    file.write(reinterpret_cast<const char*>(originalCounts), sizeof(originalCounts));

    // Save organisms
    size_t organismCount = organisms.size();
    file.write(reinterpret_cast<const char*>(&organismCount), sizeof(organismCount));

    for (auto organism : organisms) {
        char symbol = organism->getSymbol();
        file.write(&symbol, sizeof(symbol));

        int x = organism->getX();
        int y = organism->getY();
        int strength = organism->getStrength();
        int initiative = organism->getInitiative();
        int age = organism->getAge();

        file.write(reinterpret_cast<const char*>(&x), sizeof(x));
        file.write(reinterpret_cast<const char*>(&y), sizeof(y));
        file.write(reinterpret_cast<const char*>(&strength), sizeof(strength));
        file.write(reinterpret_cast<const char*>(&initiative), sizeof(initiative));
        file.write(reinterpret_cast<const char*>(&age), sizeof(age));

        // Save special data for Human
        if (symbol == 'H') {
            Human* human = dynamic_cast<Human*>(organism);
            if (human) {
                bool specialAbilityActive = human->isSpecialAbilityActive();
                int specialAbilityCooldown = human->getSpecialAbilityCooldown();
                int specialAbilityDuration = human->getSpecialAbilityDuration();
                int baseStrength = human->getBaseStrength();

                file.write(reinterpret_cast<const char*>(&specialAbilityActive), sizeof(specialAbilityActive));
                file.write(reinterpret_cast<const char*>(&specialAbilityCooldown), sizeof(specialAbilityCooldown));
                file.write(reinterpret_cast<const char*>(&specialAbilityDuration), sizeof(specialAbilityDuration));
                file.write(reinterpret_cast<const char*>(&baseStrength), sizeof(baseStrength));
            }
        }
    }

    file.close();
    addMessage("Game saved successfully!");
    return true;
}

bool World::loadGame(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        addMessage("Failed to load game!");
        return false;
    }

    clearWorld();

    file.read(reinterpret_cast<char*>(&startTime), sizeof(startTime));
    file.read(reinterpret_cast<char*>(originalCounts), sizeof(originalCounts));

    // Load organisms
    size_t organismCount;
    file.read(reinterpret_cast<char*>(&organismCount), sizeof(organismCount));

    for (size_t i = 0; i < organismCount; i++) {
        char symbol;
        file.read(&symbol, sizeof(symbol));

        int x, y, strength, initiative, age;
        file.read(reinterpret_cast<char*>(&x), sizeof(x));
        file.read(reinterpret_cast<char*>(&y), sizeof(y));
        file.read(reinterpret_cast<char*>(&strength), sizeof(strength));
        file.read(reinterpret_cast<char*>(&initiative), sizeof(initiative));
        file.read(reinterpret_cast<char*>(&age), sizeof(age));

        Organism* organism = nullptr;
        switch (symbol) {
            case 'H': {
                bool specialAbilityActive;
                int specialAbilityCooldown, specialAbilityDuration, baseStrength;
                file.read(reinterpret_cast<char*>(&specialAbilityActive), sizeof(specialAbilityActive));
                file.read(reinterpret_cast<char*>(&specialAbilityCooldown), sizeof(specialAbilityCooldown));
                file.read(reinterpret_cast<char*>(&specialAbilityDuration), sizeof(specialAbilityDuration));
                file.read(reinterpret_cast<char*>(&baseStrength), sizeof(baseStrength));

                Human* human = new Human(x, y, this);
                human->setSpecialAbilityActive(specialAbilityActive);
                human->setSpecialAbilityCooldown(specialAbilityCooldown);
                human->setSpecialAbilityDuration(specialAbilityDuration);
                human->setBaseStrength(baseStrength);
                organism = human;
                break;
            }
            case 'W': organism = new Wolf(x, y, this); break;
            case 'S': organism = new Sheep(x, y, this); break;
            case 'F': organism = new Fox(x, y, this); break;
            case 'T': organism = new Turtle(x, y, this); break;
            case 'A': organism = new Antelope(x, y, this); break;
            case 't': organism = new Grass(x, y, this); break;
            case 'd': organism = new Dandelion(x, y, this); break;
            case 'g': organism = new Guarana(x, y, this); break;
            case 'n': organism = new Nightshade(x, y, this); break;
            case 'h': organism = new Hogweed(x, y, this); break;
        }

        if (organism) {
            organism->setStrength(strength);
            organism->setInitiative(initiative);
            for (int j = 0; j < age; j++) {
                organism->incrementAge();
            }
            addOrganism(organism);
        }
    }

    file.close();
    addMessage("Game loaded successfully!");
    return true;
}
