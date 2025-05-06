#ifndef FUNCS_H
#define FUNCS_H
#include "structs.h"
#include "gameconf.h"
#include <cstdio>

// Deklaracje funkcji
void initializeGame(Settings settings,Frog &frog, Car *cars, int &numCars);
void initializeObstacles(Settings settings,Obstacle *obstacles, int &numObstacles);
bool isObstacleAtPosition(int x, int y, const Obstacle *obstacles, int numObstacles);
void drawGame(Settings settings,const Frog &frog, const Car *cars, const Stork &stork ,int numCars,const Obstacle *obstacles, int numObstacles, int elapsedTime, int moveRemainingTime, FILE *recordFile);
void carBehaviour(Settings settings,Frog &frog, Car *cars, int numCars, int numObstacles , const Obstacle *obstacles);
void updateGame(Settings settings,Frog &frog, Car *cars, int numCars, const Obstacle *obstacles, int numObstacles);
bool checkCollision( Frog &frog,  Car *cars, int numCars);
int getRandomCarType(Settings settings);
void gameRunningLoop(Settings settings,Frog &frog, Car *cars, Stork &stork ,int numCars, const Obstacle *obstacles, int numObstacles, time_t &moveStartTime, time_t &startTime, bool &gameRunning, int *leaderboard, bool cooldownActive, FILE *recordFile);
void keyboardInput(Settings settings,Frog &frog, Car *cars, int numCars, const Obstacle *obstacles, int numObstacles, bool gameRunning, time_t &moveStartTime, time_t &lastMoveTime, bool cooldownActive) ;
void createCars(Settings settings,Frog &frog, Car *cars, int &numCars, int numObstacles, const Obstacle *obstacles);
void carBehaviour(Settings settings,Frog &frog, Car *cars, int numCars, int numObstacles , const Obstacle *obstacles);
double points(int elapsedTime);
void loadConfig(const char *filename,Settings &settings);
void saveLeaderboard(const char* filename, int leaderboard[], const int current_points, int position);
void loadLeaderboard(const char *filename, int scores[3]);
void bubbleSortDescending(int arr[], int size);
void displayLeaderboard(int *leaderboard, int current_points, int moveRemainingTime, int elapsedTime);
bool attachFrog(Frog &frog, Car *cars, int numCars);
void initializeStork(Settings settings,Stork &stork) ;
void updateStork(Settings settings,Stork &stork, const Frog &frog);
void checkStorkCollision(Settings settings,Frog &frog,  Stork &stork);
bool assignValue(int value, int counter,Settings &settings);
bool isDefaultConfig(Settings &settings);
void initializeGameFromConfig(Settings settings, Frog &frog, Car *cars, int &numCars, const char *configFilename);
void initializeObstaclesFromConfig(Settings settings, Obstacle *obstacles, int &numObstacles, const char *configFilename);
void initializeStorkFromConfig(Settings settings, Stork &stork, const char *configFilename) ;
void levelInitialize(Settings &settings, Frog &frog, Car *&cars, Obstacle *&obstacles, Stork &stork, int *leaderboard, int &numCars, int &numObstacles, bool &cooldownActive);
void randomInitialize(Settings &settings, Frog &frog, Car *&cars, Obstacle *&obstacles, Stork &stork, int *leaderboard, int &numCars, int &numObstacles);
void replayGame(const char* filename);
#endif