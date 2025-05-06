#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "graphics.h"
#include "defs.h"
#include "gameover.h"
#include "corefuncs.h"
extern "C"
{
#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
}
/*
|----------------------------------------------------------------|
|                              SNAKE                             |
|----------------------------------------------------------------|
*/
void hitBorder(int &etiX, int &etiY, int &dx, int &dy)
{
    const int leftBorder = 0;
    const int rightBorder = GAME_WIDTH;
    const int topBorder = INFO_HEIGHT;
    const int bottomBorder = GAME_HEIGHT + INFO_HEIGHT;

    // Jeśli uderza w lewy górny narożnik
    if (etiX - HALF_SNAKE_OFFSET <= leftBorder && etiY - HALF_SNAKE_OFFSET <= topBorder)
    {
        etiX = leftBorder + HALF_SNAKE_OFFSET;
        etiY = topBorder + HALF_SNAKE_OFFSET;
        dx = THRESHOLD;
        dy = 0;
    }
    // Jeśli uderza w prawy górny narożnik
    else if (etiX + HALF_SNAKE_OFFSET >= rightBorder && etiY - HALF_SNAKE_OFFSET <= topBorder)
    {
        etiX = rightBorder - HALF_SNAKE_OFFSET;
        etiY = topBorder + HALF_SNAKE_OFFSET;
        dx = 0;
        dy = THRESHOLD;
    }
    // Jeśli uderza w prawy dolny narożnik
    else if (etiX + HALF_SNAKE_OFFSET >= rightBorder && etiY + HALF_SNAKE_OFFSET >= bottomBorder)
    {
        etiX = rightBorder - HALF_SNAKE_OFFSET;
        etiY = bottomBorder - HALF_SNAKE_OFFSET;
        dx = -THRESHOLD;
        dy = 0;
    }
    // Jeśli uderza w lewy dolny narożnik
    else if (etiX - HALF_SNAKE_OFFSET <= leftBorder && etiY + HALF_SNAKE_OFFSET >= bottomBorder)
    {
        etiX = leftBorder + HALF_SNAKE_OFFSET;
        etiY = bottomBorder - HALF_SNAKE_OFFSET;
        dx = 0;
        dy = -THRESHOLD;
    }
    // Jeśli uderza w lewą granicę (nie w narożnik)
    else if (etiX - HALF_SNAKE_OFFSET <= leftBorder)
    {
        etiX = leftBorder + HALF_SNAKE_OFFSET;
        dx = 0;
        dy = -THRESHOLD;
    }
    // Jeśli uderza w prawą granicę (nie w narożnik)
    else if (etiX + HALF_SNAKE_OFFSET >= rightBorder)
    {
        etiX = rightBorder - HALF_SNAKE_OFFSET;
        dx = 0;
        dy = THRESHOLD;
    }
    // Jeśli uderza w górną granicę (nie w narożnik)
    else if (etiY - HALF_SNAKE_OFFSET <= topBorder)
    {
        etiY = topBorder + HALF_SNAKE_OFFSET;
        dx = THRESHOLD;
        dy = 0;
    }
    // Jeśli uderza w dolną granicę (nie w narożnik)
    else if (etiY + HALF_SNAKE_OFFSET >= bottomBorder)
    {
        etiY = bottomBorder - HALF_SNAKE_OFFSET;
        dx = -THRESHOLD;
        dy = 0;
    }
}
void moveSnake(int snakeX[], int snakeY[], int &snakeLength, int dx, int dy,
               int teleportX[MAX_TELEPORT_SIZE][2], int teleportY[MAX_TELEPORT_SIZE][2],
               int numberOfPairs)
{
    const int COLLISION_THRESHOLD = THRESHOLD/2;
    // Sprawdzenie kolizji głowy z portalem
    for (int pairIndex = 0; pairIndex < numberOfPairs; ++pairIndex)
    {
        for (int portalIndex = 0; portalIndex < 2; ++portalIndex)
        {
            if (abs(snakeX[0] - teleportX[pairIndex][portalIndex]) < COLLISION_THRESHOLD &&
                abs(snakeY[0] - teleportY[pairIndex][portalIndex]) < COLLISION_THRESHOLD)
            {
                // Głowa węża wchodzi na portal
                int otherPortalIndex = 1 - portalIndex;
                int portalExitX = teleportX[pairIndex][otherPortalIndex];
                int portalExitY = teleportY[pairIndex][otherPortalIndex];

                // Wyliczenie pozycji obok portalu wyjściowego
                int newHeadX = portalExitX + (dx != 0 ? dx : 0); //+ THRESHOLD/2; // Jeśli rusza w poziomie
                int newHeadY = portalExitY + (dy != 0 ? dy : 0);//+ THRESHOLD/2; // Jeśli rusza w pionie

                // Przesunięcie ciała węża
                for (int i = snakeLength - 1; i > 0; --i)
                {
                    snakeX[i] = snakeX[i - 1];
                    snakeY[i] = snakeY[i - 1];
                }

                // Przypisanie nowej pozycji głowy
                snakeX[0] = newHeadX;
                snakeY[0] = newHeadY;

                return; // Kończymy teleportację dla tej klatki
            }
        }
    }

    // Jeśli nie było teleportacji, normalny ruch
    for (int i = snakeLength - 1; i > 0; i--)
    {
        snakeX[i] = snakeX[i - 1];
        snakeY[i] = snakeY[i - 1];
    }
    snakeX[0] += dx;
    snakeY[0] += dy;
}

bool checkSelfCollision(int etiX, int etiY, int snakeX[], int snakeY[], int snakeLength)
{
    for (int i = 1; i < snakeLength; i++)
    {
        if (snakeX[i] == etiX && snakeY[i] == etiY)
        {
            return true; // Kolizja z ciałem węża
        }
    }
    return false; // Brak kolizji
}
/*
|----------------------------------------------------------------|
|                              FOOD                              |
|----------------------------------------------------------------|
*/
void generateFood(int &foodX, int &foodY, int snakeX[], int snakeY[], int snakeLength, int teleportX[MAX_TELEPORT_SIZE][2], int teleportY[MAX_TELEPORT_SIZE][2], int numberOfPairs)
{
    bool validPosition = false;

    while (!validPosition)
    {
        // Losowa pozycja jedzenia
        foodX = abs(rand() % GAME_WIDTH - THRESHOLD/2);
        foodY = (rand() % (GAME_HEIGHT - INFO_HEIGHT)) + INFO_HEIGHT;

        validPosition = true;

        // Sprawdzamy, czy jedzenie nie koliduje z ciałem węża
        for (int i = 0; i < snakeLength; i++)
        {
            if (abs(foodX - snakeX[i]) < THRESHOLD && abs(foodY - snakeY[i]) < THRESHOLD)
            {
                validPosition = false;
                break;
            }
        }
        // Sprawdzamy, czy jedzenie nie koliduje z portalami
        for (int i = 0; i < numberOfPairs; i++)
        {
            if (abs(foodX - teleportX[i][0]) <2*THRESHOLD && abs(foodY - teleportY[i][1]) < 2*THRESHOLD)
            {
                validPosition = false;
                break;
            }
        }
    }
}
void checkEatFood(int &etiX, int &etiY, int &foodX, int &foodY, int snakeX[], int snakeY[], int &snakeLength, int &points, int teleportX[MAX_TELEPORT_SIZE][2], int teleportY[MAX_TELEPORT_SIZE][2], int numberOfPairs)
{
    if (abs(etiX - foodX) < THRESHOLD && abs(etiY - foodY) < THRESHOLD)
    {
        snakeLength++;
        snakeX[snakeLength - 1] = snakeX[snakeLength - 2]; // Dodaj segment węża na końcu (można rozbudować tę logikę)
        snakeY[snakeLength - 1] = snakeY[snakeLength - 2];
        generateFood(foodX, foodY, snakeX, snakeY, snakeLength, teleportX, teleportY, numberOfPairs);
        points += FOOD_POINTS;
    }
}
/*
|----------------------------------------------------------------|
|                              BONUS                             |
|----------------------------------------------------------------|
*/
void generateBonus(int &bonusX, int &bonusY, int snakeX[], int snakeY[], int snakeLength, int &foodX, int &foodY, int teleportX[MAX_TELEPORT_SIZE][2], int teleportY[MAX_TELEPORT_SIZE][2], int numberOfPairs)
{
    bool validPosition = false;

    while (!validPosition)
    {
        // Losowa pozycja jedzenia
        bonusX = abs(rand() % GAME_WIDTH -THRESHOLD/2);
        bonusY = (rand() % (GAME_HEIGHT - INFO_HEIGHT)) + INFO_HEIGHT;

        validPosition = true;
        // Sprawdzamy, czy bonus nie koliduje z jedzeniem
        if (abs(bonusX - foodX) < THRESHOLD && abs(bonusY - foodY) < THRESHOLD)
        {
            validPosition = false;
            break;
        }
        // Sprawdzamy, czy bonus nie koliduje z ciałem węża
        for (int i = 0; i < snakeLength; i++)
        {
            if (abs(bonusX - snakeX[i]) < THRESHOLD && abs(bonusY - snakeY[i]) < THRESHOLD)
            {
                validPosition = false;
                break;
            }
        }
        // Sprawdzamy, czy bonus nie koliduje z portalami
        for (int i = 0; i < numberOfPairs; i++)
        {
            if (abs(bonusX - teleportX[i][0]) < 2*THRESHOLD && abs(bonusY - teleportY[i][1]) < 2*THRESHOLD)
            {
                validPosition = false;
                break;
            }
        }
    }
}
void activateBonus(double worldTime, int &bonusVisible, int &bonusEffect, double &bonusStartTime)
{
    bonusVisible = 1;
    bonusStartTime = worldTime;
    bonusEffect = rand() % 2; // Losowy efekt: 0 - skrócenie węża, 1 - spowolnienie
}
void checkEatBonus(int &etiX, int &etiY, int &bonusX, int &bonusY, int &snakeLength, double &lastUpdateTime, double &moveDelay, int &bonusVisible, int &points, int &bonusEffect)
{
    if (abs(etiX - bonusX) < THRESHOLD && abs(etiY - bonusY) < THRESHOLD && bonusVisible)
    {
        if (bonusEffect == 0)
        {                                               // Skrócenie węża
            snakeLength = std::max(1, snakeLength - 2); // Nie mniej niż 1 segment
        }
        else if (bonusEffect == 1)
        { // Spowolnienie
            moveDelay *= (1 + SLOWDOWN_FACTOR);
        }
        bonusVisible = 0; // Bonus znika po zjedzeniu
        points += BONUS_POINTS;
    }
}
/*
|----------------------------------------------------------------|
|                           PORTALS                              |
|----------------------------------------------------------------|
*/
void generatePortals(int teleportX[MAX_TELEPORT_SIZE][2], int teleportY[MAX_TELEPORT_SIZE][2],
                     int snakeX[], int snakeY[], int snakeLength, int &numberOfPairs)
{
    const int SAFETY_MARGIN = 2 * THRESHOLD; // Minimalny dystans między portalami

    for (int pairIndex = 0; pairIndex < numberOfPairs; ++pairIndex)
    {
        for (int portalIndex = 0; portalIndex < 2; ++portalIndex)
        {
            bool validPosition = false;

            while (!validPosition)
            {
                // Losowanie pozycji portalu
                int newX = (rand() % (GAME_WIDTH / THRESHOLD)) * THRESHOLD + THRESHOLD/2;
                int newY = (rand() % ((GAME_HEIGHT - INFO_HEIGHT) / THRESHOLD)) * THRESHOLD + INFO_HEIGHT +THRESHOLD/2;

                // Sprawdzenie, czy współrzędne mieszczą się w planszy
                if (newX < 0 || newX >= GAME_WIDTH || newY < INFO_HEIGHT || newY >= GAME_HEIGHT)
                {
                    validPosition = false;
                    continue; // Pomijamy tę iterację, bo pozycja jest poza mapą
                }

                validPosition = true;

                // Sprawdzanie kolizji z ciałem węża
                for (int i = 0; i < snakeLength; i++)
                {
                    if (abs(newX - snakeX[i]) < SAFETY_MARGIN && abs(newY - snakeY[i]) < SAFETY_MARGIN)
                    {
                        validPosition = false;
                        break;
                    }
                }

                // Sprawdzanie kolizji z już istniejącymi portalami
                for (int i = 0; i <= pairIndex; i++)
                {
                    for (int j = 0; j < 2; j++)
                    {
                        if (i == pairIndex && j == portalIndex)
                            break; // Pomijamy bieżący portal
                        if (abs(newX - teleportX[i][j]) < SAFETY_MARGIN &&
                            abs(newY - teleportY[i][j]) < SAFETY_MARGIN)
                        {
                            validPosition = false;
                            break;
                        }
                    }
                    if (!validPosition)
                        break;
                }

                if (validPosition)
                {
                    // Zapisanie pozycji portalu
                    teleportX[pairIndex][portalIndex] = newX;
                    teleportY[pairIndex][portalIndex] = newY;
                }
            }
        }
    }
}
/*
|----------------------------------------------------------------|
|                       GAME MECHANISMS                          |
|----------------------------------------------------------------|
*/
void resetGame(int &etiX, int &etiY, double &worldTime, int &points, int &dx, int &dy, double &lastUpdateTime, int snakeX[], int snakeY[], int &snakeLength)
{
    etiX = SNAKE_START_X;
    etiY = SNAKE_START_Y;
    worldTime = 0;
    points = 0;
    dx, dy = 0;
    lastUpdateTime = 0;
    // Resetowanie węża
    snakeLength = 1;  // Długość węża na początku
    snakeX[0] = etiX; // Pierwszy segment to głowa
    snakeY[0] = etiY;
}
void freeObjects(SDL_Surface *screen, SDL_Surface *charset, SDL_Surface *eti, SDL_Surface *head, SDL_Surface *body, SDL_Surface *tail, SDL_Surface *food, SDL_Surface *bonus, SDL_Texture *scrtex, SDL_Window *window, SDL_Renderer *renderer)
{
    SDL_FreeSurface(eti);
    SDL_FreeSurface(head);
    SDL_FreeSurface(body);
    SDL_FreeSurface(tail);
    SDL_FreeSurface(food);
    SDL_FreeSurface(bonus);
    SDL_FreeSurface(charset);
    SDL_FreeSurface(screen);
    SDL_DestroyTexture(scrtex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
int initializeSDL(SDL_Window **window, SDL_Renderer **renderer, SDL_Surface **screen, SDL_Texture **scrtex)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }

    int rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, window, renderer);
    if (rc != 0)
    {
        SDL_Quit();
        printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(*renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 255);
    SDL_SetWindowTitle(*window, "Snake");

    *screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    *scrtex = SDL_CreateTexture(*renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_ShowCursor(SDL_DISABLE);

    return 0;
}
void handleKeyboard(SDL_Event event, int &quit, int &dx, int &dy, double &worldTime, int &points, double &lastUpdateTime, int &foodX, int &foodY, int &bonusX, int &bonusY, int *snakeX, int *snakeY, int &snakeLength, double &timeSpeeder, int &bonusVisible, double &bonusStartTime, int &bonusEffect, const char *saveFilename, int teleportX[MAX_TELEPORT_SIZE][2], int teleportY[MAX_TELEPORT_SIZE][2], int &numberOfPairs)
{
    switch (event.type)
    {
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
            quit = 1;
        if (event.key.keysym.sym == SDLK_UP && dy == 0)
        {
            dx = 0;
            dy = -THRESHOLD;
        }
        if (event.key.keysym.sym == SDLK_DOWN && dy == 0)
        {
            dx = 0;
            dy = THRESHOLD;
        }
        if (event.key.keysym.sym == SDLK_LEFT && dx == 0)
        {
            dx = -THRESHOLD;
            dy = 0;
        }
        if (event.key.keysym.sym == SDLK_RIGHT && dx == 0)
        {
            dx = THRESHOLD;
            dy = 0;
        }
        if (event.key.keysym.sym == SDLK_n)
        {
            resetGame(snakeX[0], snakeY[0], worldTime, points, dx, dy, lastUpdateTime, snakeX, snakeY, snakeLength);
            numberOfPairs = 1 + rand() % MAX_TELEPORT_SIZE;
            generatePortals(teleportX, teleportY, snakeX, snakeY, snakeLength, numberOfPairs);
            generateFood(foodX, foodY, snakeX, snakeY, snakeLength, teleportX, teleportY, numberOfPairs);
            generateBonus(bonusX, bonusY, snakeX, snakeY, snakeLength, foodX, foodY, teleportX, teleportY, numberOfPairs);
        }
        if (event.key.keysym.sym == SDLK_s)
        {
            saveData(worldTime, points, lastUpdateTime, foodX, foodY, bonusX, bonusY, snakeX, snakeY, snakeLength, timeSpeeder, bonusVisible, bonusStartTime, bonusEffect, saveFilename, teleportX, teleportY, numberOfPairs);
        }
        else if (event.key.keysym.sym == SDLK_l)
        {
            loadData(worldTime, points, lastUpdateTime, foodX, foodY, bonusX, bonusY, snakeX, snakeY, snakeLength, timeSpeeder, bonusVisible, bonusStartTime, bonusEffect, saveFilename, teleportX, teleportY, numberOfPairs);
        }
        break;
    case SDL_QUIT:
        quit = 1;
        break;
    }
}
void handleSelfCollision(const char *filename, int &points, int &dx, int &dy, SDL_Surface *screen, SDL_Surface *charset, SDL_Surface *game, SDL_Surface *eti, SDL_Surface *head, SDL_Surface *body, SDL_Surface *tail, SDL_Surface *food, SDL_Surface *bonus, SDL_Texture *scrtex, SDL_Window *window, SDL_Renderer *renderer, int *snakeX, int *snakeY, int &snakeLength, double &lastUpdateTime, double &worldTime, int &foodX, int &foodY, int &bonusX, int &bonusY, int &bonusVisible, SDL_Event event, int &quit, int teleportX[MAX_TELEPORT_SIZE][2], int teleportY[MAX_TELEPORT_SIZE][2], int &numberOfPairs)
{
    dx = 0;
    dy = 0;
    int position = checkLeaderboard(filename, points);
    if (position > 0)
    {
        char playerName[20];                                          // input imienia uzytkownika
        getPlayerName(renderer, screen, scrtex, charset, playerName); // Pobierz imię użytkownika
        saveLeaderboard(filename, playerName, points, position);      // Zapisz rekord
    }
    // Obsługa wejść użytkownika w pętli przegranej
    int inGameOver = 1;
    while (inGameOver)
    {
        drawGameOverScreen(screen, charset, scrtex, renderer, "GAME OVER", "\"n\" to start a new game, \"esc\" to quit", {255, 255, 255}, {0, 0, 0}, filename);
        handleGameOverScreenKeyboard(quit, event, inGameOver);
    }
    resetGame(snakeX[0], snakeY[0], worldTime, points, dx, dy, lastUpdateTime, snakeX, snakeY, snakeLength);
    numberOfPairs = 1 + rand() % MAX_TELEPORT_SIZE;
    generatePortals(teleportX, teleportY, snakeX, snakeY, snakeLength, numberOfPairs);
    generateFood(foodX, foodY, snakeX, snakeY, snakeLength, teleportX, teleportY, numberOfPairs);
    generateBonus(bonusX, bonusY, snakeX, snakeY, snakeLength, foodX, foodY, teleportX, teleportY, numberOfPairs);
}
void updateGame(const char* filename, int& points, int& dx, int& dy, int &bonusEffect, double &bonusStartTime,double &moveDelay,SDL_Surface *screen, SDL_Surface* charset, SDL_Surface *game, SDL_Surface *eti, SDL_Surface *head, SDL_Surface* body, SDL_Surface* tail, SDL_Surface *food, SDL_Surface *bonus, SDL_Texture *scrtex, SDL_Window *window, SDL_Renderer *renderer, int* snakeX, int* snakeY, int& snakeLength, double& lastUpdateTime, double& worldTime, int& foodX, int& foodY, int& bonusX, int& bonusY, int& bonusVisible, SDL_Event event, int& quit,int teleportX[MAX_TELEPORT_SIZE][2], int teleportY[MAX_TELEPORT_SIZE][2], int &numberOfPairs){
    // Aktualizacja pozycji głowy
    moveSnake(snakeX, snakeY, snakeLength, dx, dy, teleportX, teleportY, numberOfPairs);
    // Losowe generowanie bonusu
    if (!bonusVisible && rand() % 1000 < BONUS_ODDS)
        {
            generateBonus(bonusX, bonusY, snakeX, snakeY, snakeLength,foodX,foodY,teleportX, teleportY,numberOfPairs);
            activateBonus(worldTime, bonusVisible, bonusEffect, bonusStartTime);
        }
        // Sprawdzanie kolizji
        if (checkSelfCollision(snakeX[0], snakeY[0], snakeX, snakeY, snakeLength))
        {
            handleSelfCollision(filename, points, dx, dy, screen, charset, game, eti, head, body, tail, food, bonus, scrtex, window, renderer,snakeX, snakeY, snakeLength, lastUpdateTime, worldTime, foodX, foodY, bonusX, bonusY, bonusVisible, event, quit,teleportX, teleportY,numberOfPairs);
        }
        hitBorder(snakeX[0], snakeY[0], dx, dy); // Odbicie od krawędzi ekranu
        // Sprawdzamy, czy wąż zjadł kropkę
        checkEatFood(snakeX[0], snakeY[0], foodX, foodY, snakeX, snakeY, snakeLength, points,teleportX, teleportY,numberOfPairs);
        // Sprawdzanie kolizji z bonusem
        checkEatBonus(snakeX[0], snakeY[0], bonusX, bonusY, snakeLength, lastUpdateTime, moveDelay, bonusVisible, points, bonusEffect);
        lastUpdateTime = worldTime; // aktualizacja czasu ostatniej zmiany pozycji
}
/*
|----------------------------------------------------------------|
|                       GAMESTATE SAVE/CONFIG                    |
|----------------------------------------------------------------|
*/

void saveGameState(const GameState &state, const char *filename)
{
    FILE *file = fopen(filename, "wb");
    if (file)
    {
        fwrite(&state, sizeof(GameState), 1, file);
        fclose(file);
        printf("Game saved successfully.\n");
    }
    else
    {
        printf("Error saving game.\n");
    }
}
bool loadGameState(GameState &state, const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (file)
    {
        fread(&state, sizeof(GameState), 1, file);
        fclose(file);
        printf("Game loaded successfully.\n");
        return true;
    }
    else
    {
        printf("No saved game found.\n");
        return false;
    }
}
void loadData(double &worldTime, int &points, double &lastUpdateTime, int &foodX, int &foodY, int &bonusX, int &bonusY, int *snakeX, int *snakeY, int &snakeLength, double &timeSpeeder, int &bonusVisible, double &bonusStartTime, int &bonusEffect, const char *saveFilename, int teleportX[MAX_TELEPORT_SIZE][2], int teleportY[MAX_TELEPORT_SIZE][2], int &numberOfPairs)
{
    // Wczytanie stanu gry
    GameState loadedState;
    if (loadGameState(loadedState, saveFilename))
    {
        points = loadedState.points;
        worldTime = loadedState.worldTime;
        timeSpeeder = loadedState.timeSpeeder;
        lastUpdateTime = loadedState.lastUpdateTime;
        snakeLength = loadedState.snakeLength;
        memcpy(snakeX, loadedState.snakeX, sizeof(int) * snakeLength);
        memcpy(snakeY, loadedState.snakeY, sizeof(int) * snakeLength);
        foodX = loadedState.foodX;
        foodY = loadedState.foodY;
        bonusX = loadedState.bonusX;
        bonusY = loadedState.bonusY;
        bonusVisible = loadedState.bonusVisible;
        bonusStartTime = loadedState.bonusStartTime;
        bonusEffect = loadedState.bonusEffect;
        memcpy(teleportX, loadedState.teleportX, sizeof(int) * numberOfPairs * 2);
        memcpy(teleportY, loadedState.teleportY, sizeof(int) * numberOfPairs * 2);
        numberOfPairs = loadedState.numberOfPairs;
    }
}
void saveData(double &worldTime, int &points, double &lastUpdateTime, int &foodX, int &foodY, int &bonusX, int &bonusY, int *snakeX, int *snakeY, int &snakeLength, double &timeSpeeder, int &bonusVisible, double &bonusStartTime, int &bonusEffect, const char *saveFilename, int teleportX[MAX_TELEPORT_SIZE][2], int teleportY[MAX_TELEPORT_SIZE][2], int &numberOfPairs)
{
    // Zapis stanu gry
    GameState currentState;
    currentState.points = points;
    currentState.worldTime = worldTime;
    currentState.timeSpeeder = timeSpeeder;
    currentState.lastUpdateTime = lastUpdateTime;
    currentState.snakeLength = snakeLength;
    memcpy(currentState.snakeX, snakeX, sizeof(int) * snakeLength);
    memcpy(currentState.snakeY, snakeY, sizeof(int) * snakeLength);
    currentState.foodX = foodX;
    currentState.foodY = foodY;
    currentState.bonusX = bonusX;
    currentState.bonusY = bonusY;
    currentState.bonusVisible = bonusVisible;
    currentState.bonusStartTime = bonusStartTime;
    currentState.bonusEffect = bonusEffect;
    memcpy(currentState.teleportX, teleportX, sizeof(int) * numberOfPairs * 2);
    memcpy(currentState.teleportY, teleportY, sizeof(int) * numberOfPairs * 2);
    currentState.numberOfPairs = numberOfPairs;

    saveGameState(currentState, saveFilename);
}