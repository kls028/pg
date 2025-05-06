#ifndef DEFS_H
#define DEFS_H

#define SCREEN_WIDTH  1040
#define SCREEN_HEIGHT 1042

// Rozmiary box z tekstem
#define INFO_HEIGHT 42
#define INFO_WIDTH SCREEN_WIDTH
#define INFO_X 0
#define INFO_Y 0
// Wymiary pola gry
#define GAME_WIDTH SCREEN_WIDTH - 40
#define GAME_HEIGHT (SCREEN_HEIGHT - INFO_HEIGHT)
//Rozmiary progressbar
#define BAR_WIDTH 40
#define BAR_HEIGHT GAME_HEIGHT
#define BAR_X (GAME_WIDTH)
#define BAR_Y (INFO_HEIGHT+INFO_Y)

#define BONUS_ODDS 600 // 0-1000 im wyzej tym wieksze szanse na losowanie bonusu

#define MAX_SNAKE_SIZE 100 // Maksymalny rozmiar węża (np. 100 segmentów)
#define THRESHOLD 100

#define HALF_SNAKE_OFFSET 50

#define SNAKE_START_X THRESHOLD * 1.5
#define SNAKE_START_Y THRESHOLD * 1.5 + INFO_HEIGHT

#define BONUS_DURATION 5.0 // Czas trwania bonusu w sekundach
#define SLOWDOWN_FACTOR 0.5 // Współczynnik spowolnienia
#define SPEEDUP_FACTOR 0.8
#define BONUS_POINTS 5.0
#define FOOD_POINTS 1.0

#define MAX_TELEPORT_SIZE 4
struct GameState {
    int points;
    double worldTime;
    double timeSpeeder;
    double lastUpdateTime;
    int snakeLength;
    int snakeX[MAX_SNAKE_SIZE];
    int snakeY[MAX_SNAKE_SIZE];
    int foodX, foodY;
    int bonusX, bonusY;
    int bonusVisible;
    double bonusStartTime;
    int bonusEffect;
    int teleportX[MAX_TELEPORT_SIZE][2];
    int teleportY[MAX_TELEPORT_SIZE][2];
    int numberOfPairs;
};


#endif