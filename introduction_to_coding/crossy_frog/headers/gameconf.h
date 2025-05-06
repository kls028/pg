#ifndef GAMECONF_H
#define GAMECONF_H
#include <cmath>

// Rozmiary planszy i ustawienia domyślne

const int DEFAULT_GAME_WIDTH = 20;
const int DEFAULT_GAME_HEIGHT = 10;
const int DEFAULT_MOVE_TIME_LIMIT = 3;
const int DEFAULT_MAX_SPEED = 2; 
const int DEFAULT_MAX_OBSTACLES = 5;
const int DEFAULT_MIN_OBSTACLES = 3;
const int DEFAULT_MAX_CARS = 5;
const int DEFAULT_STORK_REFRESH_TIME = 2 ;

//Obsługa spawnowania samochodów
const int MIN_SPAWN_INTERVAL = 7;
const int MAX_SPAWN_INTERVAL = 14;

// Wagi dla poszczególnych typów samochodów (losowanie typu)
const int DEFAULT_TYPE_1_WEIGHT = 4; // Typ 1 (zwykly)
const int DEFAULT_TYPE_2_WEIGHT = 2; // Typ 2 (ostrozny)
const int DEFAULT_TYPE_3_WEIGHT = 1; // Typ 3 (przyjazny)

//Tablica rekordów
const int LEADERBOARD_SIZE = 3;

const double e = exp(1.0);
#endif