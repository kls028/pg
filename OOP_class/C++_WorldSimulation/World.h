#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <string>
#include <queue>
#include <fstream>
#include "Organism.h"

class World {
    private:
        static const int width = 20;
        static const int height = 20;
        static const int MAX_MESSAGES = 10;
        Organism* board[height][width];
        std::vector<Organism*> organisms;
        time_t startTime;
        int originalCounts[6];  // H, W, S, F, T, A
        std::queue<std::string> messageLog;  // Queue for messages
        bool simulationEnded;

    public:
        World();
        ~World();
        void executeRound();
        void drawWorld();
        void addOrganism(Organism* organism);
        void removeOrganism(Organism* organism);
        bool isPositionValid(int x, int y) const;
        bool isPositionOccupied(int x, int y) const;
        Organism* getOrganismAt(int x, int y) const;
        Organism*& getOrganismAt(int x, int y);
        int getWidth() const { return width; }
        int getHeight() const { return height; }

        void drawStatusBar();
        int getSpeciesCount(char symbol) const;
        int getOriginalCount(char symbol) const;
        int getRunningTime() const;
        void setStartTime(time_t time) { startTime = time; }
        void setOriginalCount(char symbol, int count) {
            switch(symbol) {
                case 'H': originalCounts[0] = count; break;
                case 'W': originalCounts[1] = count; break;
                case 'S': originalCounts[2] = count; break;
                case 'F': originalCounts[3] = count; break;
                case 'T': originalCounts[4] = count; break;
                case 'A': originalCounts[5] = count; break;
            }
        }

        // Message log methods
        void addMessage(const std::string& msg);
        void drawMessageLog();

        // Save/Load methods
        bool saveGame(const std::string& filename);
        bool loadGame(const std::string& filename);
        void clearWorld();

        // Simulation end check
        bool checkSimulationEnd();
        bool hasSimulationEnded() const { return simulationEnded; }

        // Getters
        const std::vector<Organism*>& getOrganisms() const { return organisms; }
};


#endif
