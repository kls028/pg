#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "graphics.h"
#include "defs.h"
#include "gameover.h"
#include "corefuncs.h"
#include <ctime>
extern "C"
{
#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
}
// Kompilacja: g++ -O2 -I./sdl/include -L. -o main main.cpp graphics.cpp game_over.cpp corefuncs.cpp -lm -lSDL2 -lpthread -ldl -lrt
#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char **argv)
{
    int t1, t2, quit, frames, rc, points;
    double delta, worldTime, fpsTimer, fps;
    int foodX, foodY;   // Pozycje kropki
    int bonusX, bonusY; // Pozycja bonusu
    const char *filename = "leaderboard.txt";
    const char *saveFilename = "savegame.dat";
    SDL_Event event;
    SDL_Surface *screen, *charset;
    SDL_Surface *game;
    SDL_Surface *eti;
    SDL_Surface *head, *body, *tail;
    SDL_Surface *food;
    SDL_Surface *bonus;
    SDL_Texture *scrtex;
    SDL_Window *window;
    SDL_Renderer *renderer;
    // Inicjalizacja SDL
    if (initializeSDL(&window, &renderer, &screen, &scrtex))
    {
        return 1;
    }
    //zaladowanie bitmap spriteow
    if (loadBmps(&screen, &charset, &game, &eti, &head, &body, &tail, &food, &bonus, scrtex, window, renderer))
    {
        return 1;
    }
    srand(static_cast<unsigned int>(time(nullptr)));
    char text[128];
    //kolory
    const int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
    const int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
    const int niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);
    const int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
    const int fioletowy = SDL_MapRGB(screen->format, 0x58, 0x18, 0x45);
    int dx = 0;                 // Ruch w prawo/lewo
    int dy = 0;                 // Ruchu w pionie/poziomie
    int snakeLength = 1;        // Początkowa długość węża
    double moveDelay = 1;       // Opóźnienie ruchu węża
    int snakeX[MAX_SNAKE_SIZE]; // Przechowuje współrzędne X węża
    int snakeY[MAX_SNAKE_SIZE]; // Przechowuje współrzędne Y węża
    int bonusActive = 0;        // Czy bonus jest aktywny
    double bonusStartTime = 0;  // Kiedy bonus został aktywowany
    int bonusEffect = 0;        // 0 - skrócenie węża, 1 - spowolnienie
    int bonusVisible = 0;       // Czy bonus jest widoczny
    snakeX[0] = SNAKE_START_X; // Pozycja początkowa węża X
    snakeY[0] = SNAKE_START_Y; // Pozycja początkowa węża Y
    int teleportX[MAX_TELEPORT_SIZE][2];// Przechowuje współrzędne X par portali
    int teleportY[MAX_TELEPORT_SIZE][2];// Przechowuje współrzędne Y par portali
    double timeSpeeder = 1;  // tempo gry
    double lastUpdateTime = 0; // czas ostatniej aktualizacji pozycji
    double lastSpeederTime = 0; // czas ostatniej aktualizacji tempa

    t1 = SDL_GetTicks();
    frames = 0;
    fpsTimer = 0;
    fps = 0;
    quit = 0;
    worldTime = 0;
    points = 0;
    int numberOfPairs =  1 + rand() % MAX_TELEPORT_SIZE;
    // Progressbar
    double elapsedTime = 0;
    double normalizedProgress = 0;
    int progressHeight = 0;
    generatePortals(teleportX, teleportY,snakeX,snakeY,snakeLength,numberOfPairs);
    generateFood(foodX, foodY, snakeX, snakeY, snakeLength,teleportX, teleportY,numberOfPairs);
    generateBonus(bonusX, bonusY, snakeX, snakeY, snakeLength,foodX,foodY,teleportX, teleportY, numberOfPairs);

    while (!quit)
    {
        t2 = SDL_GetTicks();
        delta = (t2 - t1) * 0.001;
        t1 = t2;
        worldTime += delta;
        //przyspieszanie snake 
        if (worldTime - lastSpeederTime >= 10.0)
        {
            timeSpeeder *= SPEEDUP_FACTOR;
            lastSpeederTime = worldTime;
        }
        if (bonusVisible !=0){
            // Progressbar
            elapsedTime = worldTime - bonusStartTime;
            normalizedProgress = fmod(elapsedTime, BONUS_DURATION) / BONUS_DURATION; // Normalizacja do [0, 1]
            progressHeight = (1.0 - normalizedProgress) * BAR_HEIGHT;    
        }
        // Obsługa klawiszy
        while (SDL_PollEvent(&event))
        {
            handleKeyboard(event, quit, dx, dy, worldTime, points, lastUpdateTime, foodX, foodY, bonusX, bonusY, snakeX, snakeY, snakeLength, timeSpeeder, bonusVisible, bonusStartTime, bonusEffect, saveFilename,teleportX, teleportY,numberOfPairs);
        }
        // Aktualizacja pozycji węża
        if (worldTime - lastUpdateTime >= 1.0 * timeSpeeder * moveDelay)
        {
            updateGame(filename, points, dx, dy, bonusEffect, bonusStartTime, moveDelay, screen, charset, game, eti, head, body, tail, food, bonus, scrtex, window, renderer, snakeX, snakeY, snakeLength, lastUpdateTime, worldTime, foodX, foodY, bonusX, bonusY, bonusVisible, event, quit, teleportX, teleportY, numberOfPairs);
        }
        drawGame(fpsTimer, delta, fps, screen, charset, game, eti, head, body, tail, food, bonus, scrtex, window, renderer, text, czarny, zielony, czerwony, niebieski,fioletowy, frames, foodX, foodY, bonusX, bonusY, bonusVisible, lastUpdateTime, worldTime, points, progressHeight, bonusStartTime, snakeLength, snakeX, snakeY,teleportX, teleportY,numberOfPairs,dx, dy);
    }
    // Zwolnienie zasobów
    freeObjects(screen, charset, eti, head, body, tail, food, bonus, scrtex, window, renderer);
    SDL_Quit();
    return 0;
}
