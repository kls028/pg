#include <iostream>
#include "World.h"
#include <ncurses.h>
#include <chrono>
#include <thread>
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
#include "load_config.h"
#include <cstdlib>
#include <ctime>

int main() {
    srand(time(nullptr));
    int*conf =load_config();
    // Initialize ncurses
    initscr();
    start_color();
    curs_set(0);
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    // Organisms colors
    init_pair(1, COLOR_WHITE, COLOR_BLACK);    // Human
    init_pair(2, COLOR_RED, COLOR_BLACK);      // Wolf
    init_pair(3, COLOR_WHITE, COLOR_BLACK);    // Sheep
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);   // Fox
    init_pair(5, COLOR_GREEN, COLOR_BLACK);    // Turtle
    init_pair(6, COLOR_CYAN, COLOR_BLACK);     // Antelope
    // Plant colors
    init_pair(7, COLOR_GREEN, COLOR_GREEN);    // Grass
    init_pair(8, COLOR_YELLOW, COLOR_GREEN);   // Dandelion
    init_pair(9, COLOR_RED, COLOR_GREEN);      // Guarana
    init_pair(10, COLOR_MAGENTA, COLOR_GREEN); // Nightshade
    init_pair(11, COLOR_WHITE, COLOR_GREEN);   // Hogweed

    World world;
    world.setStartTime(time(nullptr));
    
    // Add human at center
    Human* human = new Human(10, 10, &world);
    world.addOrganism(human);

    for (int i = 0; i < 2; i++) {
        int x = rand() % 20;
        int y = rand() % 20;
        world.addOrganism(new Wolf(x, y, &world));
    }
    
    for (int i = 0; i < 2; i++) {
        int x = rand() % 20;
        int y = rand() % 20;
        world.addOrganism(new Sheep(x, y, &world));
    }
    
    for (int i = 0; i < 1; i++) {
        int x = rand() % 20;
        int y = rand() % 20;
        world.addOrganism(new Fox(x, y, &world));
    }

    for (int i = 0; i < 1; i++) {
        int x = rand() % 20;
        int y = rand() % 20;
        world.addOrganism(new Turtle(x, y, &world));
    }

    for (int i = 0; i < 1; i++) {
        int x = rand() % 20;
        int y = rand() % 20;
        world.addOrganism(new Antelope(x, y, &world));
    }

    int x = rand() % 20;
    int y = rand() % 20;
    world.addOrganism(new Grass(x, y, &world));

    do {
        x = rand() % 20;
        y = rand() % 20;
    } while (world.isPositionOccupied(x, y));
    world.addOrganism(new Dandelion(x, y, &world));

    do {
        x = rand() % 20;
        y = rand() % 20;
    } while (world.isPositionOccupied(x, y));
    world.addOrganism(new Guarana(x, y, &world));

    do {
        x = rand() % 20;
        y = rand() % 20;
    } while (world.isPositionOccupied(x, y));
    world.addOrganism(new Nightshade(x, y, &world));

    do {
        x = rand() % 20;
        y = rand() % 20;
    } while (world.isPositionOccupied(x, y));
    world.addOrganism(new Hogweed(x, y, &world));

    bool running = true;

    while (running) {
        clear();

        // world border
        for (int i = 0; i < world.getWidth()+1; i++) {
            mvprintw(0, i, "#");
            mvprintw(21, i, "#");
        }
        for (int i = 0; i < world.getHeight()+1; i++) {
            mvprintw(i, 0, "#");
            mvprintw(i, 21, "#");
        }

        // Draw world content
        world.drawWorld();

        // Draw species tracker
        int trackerY = 22;
        attron(COLOR_PAIR(1) | A_BOLD);
        mvprintw(trackerY++, 1, "Living Species Stats:");
        attroff(COLOR_PAIR(1) | A_BOLD);

        const std::vector<std::pair<char, int>> speciesTypes = {
            {'H', 1}, // Human - white
            {'W', 2}, // Wolf - red
            {'S', 3}, // Sheep - white
            {'F', 4}, // Fox - yellow
            {'T', 5}, // Turtle - green
            {'A', 6}  // Antelope - cyan
        };

        for (const auto& [symbol, colorPair] : speciesTypes) {
            std::vector<Organism*> speciesMembers;
            for (auto* org : world.getOrganisms()) {
                if (org->getSymbol() == symbol) {
                    speciesMembers.push_back(org);
                }
            }
            
            if (!speciesMembers.empty()) {
                attron(COLOR_PAIR(colorPair));
                std::string speciesName;
                switch(symbol) {
                    case 'H': speciesName = "Human   "; break;
                    case 'W': speciesName = "Wolf    "; break;
                    case 'S': speciesName = "Sheep   "; break;
                    case 'F': speciesName = "Fox     "; break;
                    case 'T': speciesName = "Turtle  "; break;
                    case 'A': speciesName = "Antelope"; break;
                }
                
                for (auto* org : speciesMembers) {
                    mvprintw(trackerY++, 1, "%s - Strength: %d, Initiative: %d, Position: (%d,%d)",
                            speciesName.c_str(), org->getStrength(), org->getInitiative(),
                            org->getX(), org->getY());
                }
                attroff(COLOR_PAIR(colorPair));
            }
        }

        attron(COLOR_PAIR(1) | A_BOLD);
        mvprintw(trackerY + 1, 1, "Press 'q' to quit");
        attroff(COLOR_PAIR(1) | A_BOLD);

        refresh();

        int ch = getch();
        if (ch != ERR) {
            switch (ch) {
                case 'q':
                    running = false;
                    break;
                case 'e':
                    if (!world.checkSimulationEnd()) {
                        world.executeRound();
                    }
                    break;
                case 'o':  // Save game
                    world.saveGame("game_save.bin");
                    break;
                case 'l':  // Load game
                    world.loadGame("game_save.bin");
                    break;
                case 's':  // Human special ability
                    for (auto* org : world.getOrganisms()) {
                        if (org->getSymbol() == 'H') {
                            Human* human = dynamic_cast<Human*>(org);
                            if (human) {
                                human->activateSpecialAbility();
                            }
                        }
                    }
                    break;
                case KEY_UP:
                    for (auto* org : world.getOrganisms()) {
                        if (org->getSymbol() == 'H') {
                            Human* human = dynamic_cast<Human*>(org);
                            if (human) {
                                human->setMoveDirection(1);
                            }
                        }
                    }
                    break;
                case KEY_RIGHT:
                    for (auto* org : world.getOrganisms()) {
                        if (org->getSymbol() == 'H') {
                            Human* human = dynamic_cast<Human*>(org);
                            if (human) {
                                human->setMoveDirection(2);
                            }
                        }
                    }
                    break;
                case KEY_DOWN:
                    for (auto* org : world.getOrganisms()) {
                        if (org->getSymbol() == 'H') {
                            Human* human = dynamic_cast<Human*>(org);
                            if (human) {
                                human->setMoveDirection(3);
                            }
                        }
                    }
                    break;
                case KEY_LEFT:
                    for (auto* org : world.getOrganisms()) {
                        if (org->getSymbol() == 'H') {
                            Human* human = dynamic_cast<Human*>(org);
                            if (human) {
                                human->setMoveDirection(4);
                            }
                        }
                    }
                    break;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    endwin();
    return 0;
}