#ifndef COREFUNCS_H
#define COREFUNCS_H

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "graphics.h"
#include "defs.h"
#include "gameover.h"
extern "C"
{
#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
}

void hitBorder(int &etiX, int &etiY, int &dx, int &dy);
void moveSnake(int snakeX[], int snakeY[], int &snakeLength, int dx, int dy,int teleportX[MAX_TELEPORT_SIZE][2], int teleportY[MAX_TELEPORT_SIZE][2], int numberOfPairs);
bool checkSelfCollision(int etiX, int etiY, int snakeX[], int snakeY[], int snakeLength);
void generateFood(int &foodX, int &foodY, int snakeX[], int snakeY[], int snakeLength,int teleportX[MAX_TELEPORT_SIZE][2], int teleportY[MAX_TELEPORT_SIZE][2], int numberOfPairs);
void checkEatFood(int &etiX, int &etiY, int &foodX, int &foodY, int snakeX[], int snakeY[], int &snakeLength, int &points,int teleportX[MAX_TELEPORT_SIZE][2], int teleportY[MAX_TELEPORT_SIZE][2], int numberOfPairs);
void generateBonus(int &bonusX, int &bonusY, int snakeX[], int snakeY[], int snakeLength,int&foodX,int&foodY,int teleportX[MAX_TELEPORT_SIZE][2], int teleportY[MAX_TELEPORT_SIZE][2], int numberOfPairs);
void activateBonus(double worldTime, int &bonusVisible, int &bonusEffect, double &bonusStartTime);
void checkEatBonus(int &etiX, int &etiY, int &bonusX, int &bonusY, int &snakeLength, double &lastUpdateTime, double &moveDelay, int &bonusVisible, int &points, int &bonusEffect);
void resetGame(int &etiX, int &etiY, double &worldTime, int &points, int &dx, int &dy, double &lastUpdateTime, int snakeX[], int snakeY[], int &snakeLength);
void freeObjects(SDL_Surface *screen, SDL_Surface *charset, SDL_Surface *eti, SDL_Surface *head, SDL_Surface *body, SDL_Surface *tail, SDL_Surface *food, SDL_Surface *bonus, SDL_Texture *scrtex, SDL_Window *window, SDL_Renderer *renderer);
void handleKeyboard(SDL_Event event, int &quit, int &dx, int &dy, double &worldTime, int &points, double &lastUpdateTime, int &foodX, int &foodY, int &bonusX, int &bonusY, int *snakeX, int *snakeY, int &snakeLength, double &timeSpeeder,int& bonusVisible,double& bonusStartTime,int& bonusEffect, const char* saveFilename,int teleportX[MAX_TELEPORT_SIZE][2], int teleportY[MAX_TELEPORT_SIZE][2], int &numberOfPairs);
void handleSelfCollision(const char* filename, int& points, int& dx, int& dy, SDL_Surface *screen, SDL_Surface* charset, SDL_Surface *game, SDL_Surface *eti, SDL_Surface *head, SDL_Surface* body, SDL_Surface* tail, SDL_Surface *food, SDL_Surface *bonus, SDL_Texture *scrtex, SDL_Window *window, SDL_Renderer *renderer, int* snakeX, int* snakeY, int& snakeLength, double& lastUpdateTime, double& worldTime, int& foodX, int& foodY, int& bonusX, int& bonusY, int& bonusVisible, SDL_Event event, int& quit,int teleportX[MAX_TELEPORT_SIZE][2], int teleportY[MAX_TELEPORT_SIZE][2], int &numberOfPairs);
void saveGameState(const GameState &state, const char *filename);
bool loadGameState(GameState &state, const char *filename);
int initializeSDL(SDL_Window **window, SDL_Renderer **renderer, SDL_Surface **screen, SDL_Texture **scrtex);
void generatePortals(int teleportX[MAX_TELEPORT_SIZE][2], int teleportY[MAX_TELEPORT_SIZE][2],
                     int snakeX[], int snakeY[], int snakeLength, int& numberOfPairs);
void saveData(double &worldTime, int &points, double &lastUpdateTime, int &foodX, int &foodY, int &bonusX, int &bonusY, int *snakeX, int *snakeY, int &snakeLength, double &timeSpeeder, int &bonusVisible, double &bonusStartTime, int &bonusEffect, const char *saveFilename, int teleportX[MAX_TELEPORT_SIZE][2], int teleportY[MAX_TELEPORT_SIZE][2], int &numberOfPairs);
void loadData(double &worldTime, int &points, double &lastUpdateTime, int &foodX, int &foodY, int &bonusX, int &bonusY, int *snakeX, int *snakeY, int &snakeLength, double &timeSpeeder, int &bonusVisible, double &bonusStartTime, int &bonusEffect, const char *saveFilename, int teleportX[MAX_TELEPORT_SIZE][2], int teleportY[MAX_TELEPORT_SIZE][2], int &numberOfPairs);
void updateGame(const char* filename, int& points, int& dx, int& dy, int &bonusEffect, double &bonusStartTime,double &moveDelay,SDL_Surface *screen, SDL_Surface* charset, SDL_Surface *game, SDL_Surface *eti, SDL_Surface *head, SDL_Surface* body, SDL_Surface* tail, SDL_Surface *food, SDL_Surface *bonus, SDL_Texture *scrtex, SDL_Window *window, SDL_Renderer *renderer, int* snakeX, int* snakeY, int& snakeLength, double& lastUpdateTime, double& worldTime, int& foodX, int& foodY, int& bonusX, int& bonusY, int& bonusVisible, SDL_Event event, int& quit,int teleportX[MAX_TELEPORT_SIZE][2], int teleportY[MAX_TELEPORT_SIZE][2], int &numberOfPairs);
#endif