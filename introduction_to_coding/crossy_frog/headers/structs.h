#ifndef STRUCTS_H
#define STRUCTS_H
#include "gameconf.h"
// Struktury
struct Car {
    int x, y;       // Pozycja samochodu
    int speed;      // Prędkość samochodu
    bool direction; // true - w prawo, false - w lewo
    int type; // 1 - zwykly, 2 - ostrozny, 3 - przyjazny
    bool stopped = false; // Zatrzymanie (dla auta typu 3)
};

struct Frog {
    int x, y;       // Pozycja żaby na planszy
    bool carriedByCar = false; //Czy zaba trafila na przyjazny samochod?
    Car *attachedCar;     // Wskaźnik do auta, do którego jest przyklejona
    
};

struct Obstacle {
    int x, y;  // Pozycja startowa przeszkody
    int width; // Szerokość przeszkody
    int height; // Wysokość przeszkody
};

struct Stork{
    int x, y;       // Pozycja bociana na planszy
    int speed; //Prędkość bociana
};

struct Settings{
    int GAME_WIDTH = DEFAULT_GAME_WIDTH; // Szerokość planszy
    int GAME_HEIGHT = DEFAULT_GAME_HEIGHT; // Wysokość planszy
    int MOVE_TIME_LIMIT = DEFAULT_MOVE_TIME_LIMIT; // Czas na ruch do przodu
    
    int MAX_CARS = DEFAULT_MAX_CARS; // Maksymalna liczba samochodów
    int MAX_SPEED = DEFAULT_MAX_SPEED; // Maksymalna prędkość aut
    
    int MAX_OBSTACLES = DEFAULT_MAX_OBSTACLES;// Maksymalna ilość przeszkód
    int MIN_OBSTACLES = DEFAULT_MIN_OBSTACLES;//Minimalna ilość przeszkód
    
    int STORK_REFRESH_TIME = DEFAULT_STORK_REFRESH_TIME;

    int TYPE_1_WEIGHT = DEFAULT_TYPE_1_WEIGHT;
    int TYPE_2_WEIGHT = DEFAULT_TYPE_2_WEIGHT;
    int TYPE_3_WEIGHT = DEFAULT_TYPE_3_WEIGHT;
};


#endif