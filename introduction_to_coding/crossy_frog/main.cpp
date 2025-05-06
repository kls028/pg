#include "headers/gameconf.h"
#include "headers/structs.h"
#include "headers/funcs.h"
#include <iostream>
#include <conio.h> 
#include <ctime>
#include <cmath>
#include <string.h>
using namespace std;

// Nazwa pliku z ustawieniami i tablicą rekordów w trybie losowym
const char* SETTINGS_FILENAME = "textfiles/config.txt";
const char* currentSettings = "default";
const char* LEADERBOARD_FILENAME = "textfiles/leaderboard.txt";

int main() {
    // Inicjalizacja generatora liczb losowych
    srand(static_cast<unsigned>(time(0)));
    int mode;
    printf("Wybierz tryb: losowy(1), poziom(2), odtwarzanie(3)");
    scanf("%d", &mode);
    if (mode!= 1 && mode!= 2 && mode!=3) {
        printf("Nieprawidłowy tryb.\n");
    }

    Settings settings;
    Frog frog;
    Car *cars = nullptr;
    Obstacle *obstacles = nullptr;
    Stork stork;
    int numCars = 0, numObstacles = 0;
    int leaderboard[LEADERBOARD_SIZE];
    bool cooldownActive = false;
    if (mode == 3) {
        replayGame("textfiles/replay.txt");
        return 0;
    }
    else if (mode == 2) {
        levelInitialize(settings, frog, cars,obstacles,stork,leaderboard, numCars, numObstacles, cooldownActive);
    }
    else if (mode == 1) {
        randomInitialize(settings, frog, cars,obstacles,stork,leaderboard, numCars, numObstacles);
    }
    bool gameRunning = true;
    time_t startTime = time(0);
    time_t moveStartTime = time(0);
    FILE* recordFile = fopen("textfiles/replay.txt", "w");

    gameRunningLoop(settings,frog, cars, stork,numCars, obstacles, numObstacles, moveStartTime, startTime, gameRunning, leaderboard, cooldownActive, recordFile);
    
    free (cars);
    free (obstacles);
    fclose(recordFile);
    return 0;
}
/*
----------------------------------------------------------------
|                                                              |
|                   TRYB POZIOMU                               |
|                                                              |
----------------------------------------------------------------
*/
//inicjalizcja trybu poziomu
void setLevelFilenames(int levelId, char *levelSettingsFilename, char *levelMapFilename, char *levelLeaderboardFilename) {
    if (levelId < 1 || levelId > 3) {
        printf("Nieprawidłowy numer poziomu.\n");
        return;
    } 
    sprintf(levelSettingsFilename, "levels/%d/config%d.txt", levelId, levelId);
    sprintf(levelMapFilename, "levels/%d/map%d.txt", levelId, levelId);
    sprintf(levelLeaderboardFilename, "levels/%d/leaderboard%d.txt", levelId, levelId);
}
void levelInitialize(Settings &settings, Frog &frog, Car *&cars, Obstacle *&obstacles, Stork &stork, int *leaderboard, int &numCars, int &numObstacles, bool &cooldownActive) {
    char levelSettingsFilename[256], levelMapFilename[256], levelLeaderboardFilename[256];
    int levelId = 0;
    printf("Podaj nr poziomu: 1|2|3\n");
    scanf("%d", &levelId);
    // Ustawienie nazw plików w zależności od poziomu
    setLevelFilenames(levelId,levelSettingsFilename,levelMapFilename, levelLeaderboardFilename );
    // Załadowanie leaderboardu i konfiguracji poziomu
    loadLeaderboard(levelLeaderboardFilename, leaderboard);
    loadConfig(levelSettingsFilename, settings);
    numCars = settings.MAX_CARS;
    numObstacles = settings.MAX_OBSTACLES;
    cars = (Car *)malloc(settings.MAX_CARS * sizeof(Car));
    obstacles = (Obstacle *)malloc(settings.MAX_OBSTACLES * sizeof(Obstacle));
    // Inicjalizacja obiektów
    initializeGameFromConfig(settings, frog, cars, numCars, levelMapFilename);
    initializeObstaclesFromConfig(settings, obstacles, numObstacles, levelMapFilename);
    initializeStorkFromConfig(settings, stork, levelMapFilename);
    cooldownActive = (levelId == 3);
}
//inicjalizacje obiektow w trybie poziomu
void initializeGameFromConfig(Settings settings, Frog &frog, Car *cars, int &numCars, const char *mapFilename) {
    FILE *file = fopen(mapFilename, "r");
    if (!file) {
        printf("Błąd: Nie udało się otworzyć pliku konfiguracji poziomu.\n");
        exit(1);
    }

    // Wczytanie pozycji żaby
    fscanf(file, "%d %d", &frog.x, &frog.y);
    frog.attachedCar = nullptr;
    // Wczytanie samochodów
    for (int i = 0; i < numCars; ++i) {
        fscanf(file, "%d %d %d %d %d", &cars[i].x, &cars[i].y, &cars[i].speed, &cars[i].direction, &cars[i].type);
    }

    fclose(file);
}
void initializeObstaclesFromConfig(Settings settings, Obstacle *obstacles, int &numObstacles, const char *mapFilename) {
    FILE *file = fopen(mapFilename, "r");
    if (!file) {
        printf("Błąd: Nie udało się otworzyć pliku konfiguracji poziomu.\n");
        exit(1);
    }

    // Przeskoczenie linii z pozcyjami żaby i aut
    char buffer[256];
    fgets(buffer, sizeof(buffer), file);
    for (int i = 0; i < settings.MAX_CARS; ++i) {
        fgets(buffer, sizeof(buffer), file);
    }

    // Wczytanie przeszkód
    for (int i = 0; i < numObstacles; ++i) {
        fscanf(file, "%d %d %d %d", &obstacles[i].x, &obstacles[i].y, &obstacles[i].width, &obstacles[i].height);
    }

    fclose(file);
}
void initializeStorkFromConfig(Settings settings, Stork &stork, const char *mapFilename) {
    FILE *file = fopen(mapFilename, "r");
    if (!file) {
        printf("Błąd: Nie udało się otworzyć pliku konfiguracji poziomu.\n");
        exit(1);
    }

    // Przeskoczenie linii żaby, samochodów i przeszkód
    char buffer[256];
    fgets(buffer, sizeof(buffer), file);

    for (int i = 0; i < settings.MAX_CARS; ++i) {
        fgets(buffer, sizeof(buffer), file);
    }
    for (int i = 0; i < settings.MAX_OBSTACLES; ++i) {
        fgets(buffer, sizeof(buffer), file);
    }

    // Wczytanie bociana
    fscanf(file, "%d %d %d", &stork.x, &stork.y, &stork.speed);

    fclose(file);
}
/*
----------------------------------------------------------------
|                                                              |
|                    TRYB LOSOWY                               |
|                                                              |
----------------------------------------------------------------
*/
//inicjalizacja trybu losowego
void randomInitialize(Settings &settings, Frog &frog, Car *&cars, Obstacle *&obstacles, Stork &stork, int *leaderboard, int &numCars, int &numObstacles){
// Załadowanie ustawień z pliku
    loadConfig(SETTINGS_FILENAME,settings);
    numCars = settings.MAX_CARS;
    numObstacles = settings.MAX_OBSTACLES;
    loadLeaderboard(LEADERBOARD_FILENAME, leaderboard);
    // Alokacja pamięci dla obiektów
    cars = (Car *)malloc(settings.MAX_CARS * sizeof(Car));
    obstacles = (Obstacle *)malloc(settings.MAX_OBSTACLES * sizeof(Obstacle));

    // Inicjalizacja obiektów
    initializeGame(settings, frog, cars, numCars);
    initializeObstacles(settings, obstacles, numObstacles);
    initializeStork(settings, stork);
}
// Funkcja inicjalizująca pozycje żaby i aut
void initializeGame(Settings settings,Frog &frog, Car *cars, int &numCars) {
    //ustalenie pozycji startowej żaby 
    frog.x = settings.GAME_WIDTH / 2;
    frog.y = settings.GAME_HEIGHT - 1;
    //Żaba nie jest przyczepiona domyslnie do zadnego auta typu przyjaznego
    frog.attachedCar = nullptr;
    for (int i = 0; i < numCars; ++i) {
        cars[i].x = rand() % settings.GAME_WIDTH;
        cars[i].y = rand() % (settings.GAME_HEIGHT - 1); // Samochody nie mogą być na linii startowej żaby
        cars[i].speed = 1 + rand() % settings.MAX_SPEED; // minimum 1 aby auta nie stały w miejscu
        cars[i].direction = rand() % 2;
        cars[i].type = getRandomCarType(settings);
    }
}
// Funkcja tworząca przeszkody
void initializeObstacles(Settings settings,Obstacle *obstacles, int &numObstacles) {
    numObstacles = max(rand() % (settings.MAX_OBSTACLES), settings.MIN_OBSTACLES);
    for (int i = 0; i < numObstacles; ++i) {
        obstacles[i].x = rand() % settings.GAME_WIDTH;
        obstacles[i].y = rand() % settings.GAME_HEIGHT;
        obstacles[i].width = 1;
        obstacles[i].height = 1; 
    }
}
//Funckja tworząca bociana
void initializeStork(Settings settings,Stork &stork) {
    stork.x = rand() % settings.GAME_WIDTH;
    stork.y = rand() % (settings.GAME_HEIGHT / 2);
    stork.speed = 1;
}
/*
----------------------------------------------------------------
|                                                              |
|                    MECHANIZMY ROZGRYWKI                      |
|                                                              |
----------------------------------------------------------------
*/
//glowna petla gry
void gameRunningLoop(Settings settings,Frog &frog, Car *cars, Stork &stork ,int numCars, const Obstacle *obstacles, int numObstacles, time_t &moveStartTime, time_t &startTime, bool &gameRunning, int *leaderboard, bool cooldownActive, FILE *recordFile) {
    time_t lastMoveTime = time(0);
    while (gameRunning) {
        int elapsedTime = static_cast<int>(time(0) - startTime); //glowny pomiar czasu (od początku gry)
        int moveRemainingTime = settings.MOVE_TIME_LIMIT - static_cast<int>(time(0) - moveStartTime);// pomiar pozostałego czasu na ruch do przodu
        if (moveRemainingTime < 0) {
            printf("Czas na ruch do przodu minal! Koniec gry.\n");
            break;
        }
        //bieżące obliczanie punktów w zależności od czasu rozgrywki
        double current_points = (int)points(elapsedTime);
        //rysowanie planszy
        drawGame(settings,frog, cars,stork ,numCars,obstacles,numObstacles ,elapsedTime, moveRemainingTime, recordFile);
        checkStorkCollision(settings,frog, stork);
        if (checkCollision(frog, cars, numCars)) {
            printf("Kolizja! Koniec gry\n");
            gameRunning = false;
        }
        // Sprawdzenie warunku wygranej
        if (frog.y == 0) {
            displayLeaderboard(leaderboard, current_points, moveRemainingTime, elapsedTime);
            gameRunning = false;
        }
        keyboardInput(settings, frog, cars, numCars, obstacles, numObstacles, gameRunning, moveStartTime, lastMoveTime, cooldownActive);
        updateGame(settings,frog, cars, numCars, obstacles, numObstacles);
        updateStork(settings,stork, frog);
        _sleep(100);
    }
}
//wyświetlanie tablicy rekordów
void displayLeaderboard(int *leaderboard, int current_points, int moveRemainingTime, int elapsedTime) {
    system("cls");
    printf("Jumping Frog by: Piotr Skierka s203246\n");
    printf("Czas: %ds | Czas na przejscie wyzej: %ds | Ustawienia: %s\n", elapsedTime, moveRemainingTime, currentSettings);
    printf("Wygrana! Zaba dotarla do celu | Punkty: %d\n", current_points);
    for (int i = 0; i < LEADERBOARD_SIZE; ++i) {
        if (current_points > leaderboard[i]) {
            saveLeaderboard(LEADERBOARD_FILENAME, leaderboard,current_points , i);
                break;
        }
    }
    printf("-------Najlepsze wyniki-------\n");
    for (int i = 0; i < 3; ++i) {
        printf("%d: %d\n", i + 1, leaderboard[i]);
    }
}
//Obsługa doklejania żaby do auta typu przyjaznego
bool attachFrog(Frog &frog, Car *cars, int numCars) {
    for (int i = 0; i < numCars; ++i) {// Przyczepianie do auta typu przyjaznego
        if ((frog.x >= cars[i].x -2 || frog.x <=cars[i].x +2) && frog.y == cars[i].y && cars[i].type == 3) {
            frog.attachedCar = &cars[i];
            frog.carriedByCar = true;
            return false;
            break;
        }
    }
    return true;
}
// Obsługa klawiatury
void keyboardInput(Settings settings,Frog &frog, Car *cars, int numCars, const Obstacle *obstacles, int numObstacles, bool gameRunning, time_t &moveStartTime, time_t &lastMoveTime, bool cooldownActive) {        
        if (_kbhit()) {
            time_t currentTime = time(0);

            // Sprawdzenie cooldownu
            if (cooldownActive && difftime(currentTime, lastMoveTime) < 0.05) {
                return;
            }
            int oldX = frog.x, oldY = frog.y;
            int newX = frog.x, newY = frog.y;
            // Zwolnienie żaby od przypisanego auta typu przyjaznego
            bool detach = true;
            switch (_getch()) {
                case 'w': newY = max(frog.y - 1, 0);  moveStartTime = time(0);frog.attachedCar = nullptr; break;  // Góra
                case 's': newY = min(frog.y + 1, settings.GAME_HEIGHT - 1);frog.attachedCar = nullptr; break;  // Dół
                case 'a': newX = max(frog.x - 1, 0); break; frog.attachedCar = nullptr;  // Lewo
                case 'd': newX = min(frog.x + 1, settings.GAME_WIDTH - 1);frog.attachedCar = nullptr; break;  // Prawo
                case 'r': detach = attachFrog(frog,cars,numCars); break;
                case 'q': gameRunning = false; break;          // Wyjście
            }
            if (detach) {
                frog.attachedCar = nullptr; // Odłączanie od auta przy ruchu (poza "R" key)
            }
            //sprawdzenie, czy żaba nie wejdzie po ruchu gracza na przeszkodę
            if (!isObstacleAtPosition(newX, newY, obstacles, numObstacles)) { 
                frog.x = newX;
                frog.y = newY;
            }
            lastMoveTime = currentTime;
        }
}
//losowanie typu samochodu
int getRandomCarType(Settings settings) { 
    int totalWeight = settings.TYPE_1_WEIGHT + settings.TYPE_2_WEIGHT + settings.TYPE_3_WEIGHT;
    int randomValue = rand() % totalWeight;

    if (randomValue < settings.TYPE_1_WEIGHT) {
        return 1; // Typ 1
    } else if (randomValue < settings.TYPE_1_WEIGHT + settings.TYPE_2_WEIGHT) {
        return 2; // Typ 2
    } else {
        return 3; // Typ 3
    }
}
//Funkcja sprawdzająca kolizję z przeszkodą
bool isObstacleAtPosition(int x, int y, const Obstacle *obstacles, int numObstacles) {
    for (int i = 0; i < numObstacles; ++i) {
        if (obstacles[i].x == x && obstacles[i].y == y) {
            return true;
        }
    }
    return false;
}
//funkcja rysujaca auta
bool drawCars(int x, int y, const Car *cars, int numCars, FILE* recordFile) {
    for (int i = 0; i < numCars; ++i) {
        if (x == cars[i].x && y == cars[i].y) {
            if (cars[i].type == 1) { // Samochód zwykły
                printf("\033[31mC\033[0m");
                fprintf(recordFile, "\033[31mC\033[0m");
            } else if (cars[i].type == 2) { // Samochód ostrożny
                printf("\033[33mC\033[0m");
                fprintf(recordFile, "\033[33mC\033[0m");
            } else if (cars[i].type == 3) { // Samochód przyjazny
                printf("\033[36mC\033[0m");
                fprintf(recordFile, "\033[36mC\033[0m");
            }
            return true; // Auto zostało narysowane
        }
    }
    return false; // Auto nie zostało znalezione
}
//funkcja rysujaca przeszkody
bool drawObstacles(int x, int y, const Obstacle *obstacles, int numObstacles, FILE* recordFile) {
    for (int i = 0; i < numObstacles; ++i) {
        if (x == obstacles[i].x && y == obstacles[i].y) {
            printf("#"); // Rysujemy przeszkodę
            fprintf(recordFile, "#");
            return true; // Przeszkoda została narysowana
        }
    }
    return false; // Brak przeszkody
}
// Funkcja rysująca planszę
void drawGame(Settings settings, const Frog &frog, const Car *cars, const Stork &stork, int numCars, const Obstacle *obstacles, int numObstacles, int elapsedTime, int moveRemainingTime, FILE* recordFile) {
    system("cls"); // Czyszczenie ekranu
    printf("Jumping Frog by: Piotr Skierka s203246\n");
    printf("Czas: %ds | Czas na przejscie wyzej: %ds | Ustawienia: %s\n", elapsedTime, moveRemainingTime, currentSettings);
    
    fprintf(recordFile, "Jumping Frog by: Piotr Skierka s203246\n");
    fprintf(recordFile, "Czas: %ds | Czas na przejscie wyzej: %ds\n", elapsedTime, moveRemainingTime);
    for (int y = 0; y < settings.GAME_HEIGHT; ++y) {
        for (int x = 0; x < settings.GAME_WIDTH; ++x) {
            if (x == frog.x && y == frog.y) {
                printf("\033[32mF\033[0m");  // Żaba
                fprintf(recordFile, "\033[32mF\033[0m");
            } else if (x == stork.x && y == stork.y) {
                printf("\033[95mS\033[0m");  // Bocian
                fprintf(recordFile, "\033[95mS\033[0m");
            } else {
                // Rysowanie samochodów i przeszkód
                if (!drawCars(x, y, cars, numCars, recordFile) && !drawObstacles(x, y, obstacles, numObstacles, recordFile)) {
                    printf("."); // Puste pole
                    fprintf(recordFile, ".");
                }
            }
        }
        printf("\n");
        fprintf(recordFile, "\n");
    }
    fprintf(recordFile, "END_FRAME\n");
}
//przypadkowe tworzenie nowych aut w trakcie rozgrywki
void createCars(Settings settings,Frog &frog, Car *cars, int &numCars, int numObstacles, const Obstacle *obstacles) {
    // Stworzenie nowego auta
    Car newCar;
    bool positionIsValid;
    do {
        newCar.x = rand() % settings.GAME_WIDTH;
        newCar.y = rand() % (settings.GAME_HEIGHT-1);
        newCar.speed = (rand() % settings.MAX_SPEED) + 1;
        newCar.direction = rand() % 2;
        newCar.type = rand() % 3 + 1;

        //Sprawdzenie czy auto ma poprawną lokalizację
        positionIsValid = true;
        if (frog.x == newCar.x && frog.y == newCar.y) {
                positionIsValid = false;

        }
        for (int k = 0; k < numCars; ++k) {
            if (cars[k].x == newCar.x && cars[k].y == newCar.y) {
                positionIsValid = false;
                break;
            }
        }
        if (positionIsValid) {
            for (int k = 0; k < numObstacles; ++k) {
                if (obstacles[k].x == newCar.x && obstacles[k].y == newCar.y) {
                    positionIsValid = false;
                    break;
                }
            }
        }
    } while (!positionIsValid);

    cars[numCars] = newCar;
    ++numCars;
}
//obsluga zderzen aut z granica planszy
void handleBorderCollision(Settings settings, Frog &frog, Car *cars, int &numCars, int &currentIndex, int nextX, int numObstacles, const Obstacle *obstacles) {
    Car &car = cars[currentIndex]; // Referencja do aktualnego samochodu
    bool atBorder = (car.direction && nextX == 0) || (!car.direction && nextX == settings.GAME_WIDTH - 1);

    if (atBorder) {
        int randomCheck = rand() % 10;
        if (randomCheck % 3 == 0) {
            // Usunięcie auta
            for (int j = currentIndex; j < numCars - 1; ++j) {
                cars[j] = cars[j + 1];
            }
            --numCars;
            --currentIndex;
            createCars(settings, frog, cars, numCars, numObstacles, obstacles);
        } else {
            car.x = nextX; // Wrapping
        }
    } else {
        car.x = nextX; // Normalna nowa pozycja
    }
}
//obsluga zachowania aut
void carBehaviour(Settings settings,Frog &frog, Car *cars, int numCars, int numObstacles , const Obstacle *obstacles) {
    for (int i = 0; i < numCars; ++i) {
        if (cars[i].type == 2 && frog.y == cars[i].y) {
            continue;
        }
        //nowa pozycja auta
        int nextX = cars[i].x;
        if (cars[i].direction) {
            nextX = (cars[i].x + cars[i].speed) % settings.GAME_WIDTH;
        } else {
            nextX = (cars[i].x - cars[i].speed + settings.GAME_WIDTH) % settings.GAME_WIDTH;
        }
        // obsluga kolizji z przeszkodami
        bool isChangedDirection = false;
        for (int j=0; j<cars[i].speed+1;j++){
            if (isObstacleAtPosition(nextX+j, cars[i].y, obstacles, numObstacles)) {
                cars[i].direction = !cars[i].direction; //odwracamy kierunek jazdy auta
                isChangedDirection = true;
                break;
            }
        }
        if (!isChangedDirection) {
            // Obsługa kolizji z granicą planszy
            handleBorderCollision(settings, frog, cars, numCars, i, nextX, numObstacles, obstacles);
        }
        
    }
}
//obsluga ruchu bociana
void updateStork(Settings settings,Stork &stork, const Frog &frog) {
    static int counter = 0;
    if (++counter % settings.STORK_REFRESH_TIME == 0) {  //Bocian rusza się co wskazaną liczbe klatek
        if (frog.x > stork.x) stork.x += stork.speed;
        else if (frog.x < stork.x) stork.x -= stork.speed;
        
        if (frog.y > stork.y) stork.y += stork.speed;
        else if (frog.y < stork.y) stork.y -= stork.speed;
    }
}
//obsluga kolizji żaby i bociana
void checkStorkCollision(Settings settings,Frog &frog, Stork &stork) {
    if (frog.x == stork.x && frog.y == stork.y) {
        frog.x = settings.GAME_WIDTH / 2;  // Żaba wraca na pozycję startową
        frog.y = settings.GAME_HEIGHT - 1;
        stork.x = 0;
        stork.y = 0;
    }
}
//spawnowanie aut
void spawnCars(Settings settings, Car *cars, int &numCars, time_t &nextSpawnTime, time_t currentTime){
    if (numCars < settings.MAX_CARS) { // nie mozemy wyjsc poza gorny limit ilosci aut narzucony w ustawieniach
            cars[numCars].x = rand() % settings.GAME_WIDTH;
            cars[numCars].y = rand() % (settings.GAME_HEIGHT - 1);
            cars[numCars].speed = 1 + rand() % settings.MAX_SPEED;
            cars[numCars].direction = rand() % 2;
            cars[numCars].type = getRandomCarType(settings);
            numCars++;
        }
        //Zaplanowanie kolejnego spawnu
        nextSpawnTime = currentTime + (MIN_SPAWN_INTERVAL + rand() % (MAX_SPAWN_INTERVAL - MIN_SPAWN_INTERVAL + 1));
}
// Aktualizacja pozycji samochodów
void updateGame(Settings settings,Frog &frog, Car *cars, int numCars, const Obstacle *obstacles,int numObstacles) {
    time_t currentTime = time(0);
    static time_t nextSpawnTime = currentTime + MIN_SPAWN_INTERVAL;
    //tworzenie przypadkowych nowych aut co jakiś czas
    if (currentTime >= nextSpawnTime) {
        spawnCars(settings, cars, numCars, nextSpawnTime, currentTime);
    }
    // Jeśli żaba jest przyklejona do auta, porusza się razem z nim
    if (frog.attachedCar != nullptr) {
        if (frog.attachedCar->direction) {
            frog.x = (frog.attachedCar->x + frog.attachedCar->speed) % settings.GAME_WIDTH;
        } else {
            frog.x = (frog.attachedCar->x - frog.attachedCar->speed + settings.GAME_WIDTH) % settings.GAME_WIDTH;
        }
    }

    carBehaviour(settings,frog, cars, numCars, numObstacles, obstacles);
}
// Sprawdzenie kolizji między żabą a samochodami
bool checkCollision(Frog &frog, Car *cars, int numCars) {
    for (int i = 0; i < numCars; ++i) {
        for (int j = 0; j <cars[i].speed+1; ++j) {
            if (frog.x == cars[i].x + j && frog.y == cars[i].y) {
                if (cars[i].type == 3) { //auto przyjazne nie sprawia kolizji
                    return false;
                }
                return true;
            }
        }    
    }
    return false;
}
//funkcja licząca punkty ze wzoru
double points(int elapsedTime){
    double subwynik = pow(e, -0.069 * ((elapsedTime-1)*0.1));
    return 1000 * subwynik;
}
/*
----------------------------------------------------------------
|                                                              |
|                    OBLSUGA PLIKOW                            |
|                                                              |
----------------------------------------------------------------
*/
//funkcja przypisujaca wartosci ustawień z pliku
bool assignValue(int value, int counter, Settings &settings) {
    switch (counter) {
        case 0: settings.GAME_WIDTH = value; break;
        case 1: settings.GAME_HEIGHT = value; break;
        case 2: settings.MOVE_TIME_LIMIT = value; break;
        case 3: settings.MAX_CARS = value; break;
        case 4: settings.MAX_SPEED = value; break;
        case 5: settings.TYPE_1_WEIGHT = value; break;
        case 6: settings.TYPE_2_WEIGHT = value; break;
        case 7: settings.TYPE_3_WEIGHT = value; break;
        case 8: settings.MAX_OBSTACLES = value; break;
        case 9: settings.MIN_OBSTACLES = value; break;
        case 10: settings.STORK_REFRESH_TIME = value; break;
        default: return false; // Nieprawidłowy indeks
    }
    return true;
}
// Funkcja do sprawdzania, czy używane wartości ustawień są domyślne
bool isDefaultConfig(Settings &settings) {
    return settings.GAME_WIDTH == DEFAULT_GAME_WIDTH &&
           settings.GAME_HEIGHT == DEFAULT_GAME_HEIGHT &&
           settings.MOVE_TIME_LIMIT == DEFAULT_MOVE_TIME_LIMIT &&
           settings.MAX_CARS == DEFAULT_MAX_CARS &&
           settings.MAX_SPEED == DEFAULT_MAX_SPEED &&
           settings.TYPE_1_WEIGHT == DEFAULT_TYPE_1_WEIGHT &&
           settings.TYPE_2_WEIGHT == DEFAULT_TYPE_2_WEIGHT &&
           settings.TYPE_3_WEIGHT == DEFAULT_TYPE_3_WEIGHT &&
           settings.MAX_OBSTACLES == DEFAULT_MAX_OBSTACLES &&
           settings.MIN_OBSTACLES == DEFAULT_MIN_OBSTACLES &&
           settings.STORK_REFRESH_TIME == DEFAULT_STORK_REFRESH_TIME;
}
// Funkcja do wczytywania konfiguracji z pliku
void loadConfig(const char *filename,Settings &settings) {
    FILE *file = fopen(filename, "r");
    if (file == nullptr) {
        cerr << "Nie można otworzyc pliku config.txt, uzywane wartosci domyslne." << endl;
        return;
    }
    char line[256];
    int valueCounter = 0;

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n') continue;

        int value;
        if (sscanf(line, "%d", &value) == 1) {
            if (assignValue(value, valueCounter,settings)) {
                valueCounter++;
            }
        }
    }
    fclose(file);

    currentSettings = isDefaultConfig(settings) ? "default" : filename;
}
//funkcja do sortowania tablicy m.in. wyników w porzadku malejacym
void bubbleSortDescending(int arr[], int size) {
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            if (arr[j] < arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}
//Funkcja do wczytywania listy najlepszych wyników
void loadLeaderboard(const char *filename, int scores[3]) {
    FILE *file = fopen(filename, "r");
    if (file == nullptr) {
        cerr << "Nie można otworzyć pliku leaderboard.txt." << endl;
    }
    char line[256];
    int valueCounter = 0;
    while (fgets(line, sizeof(line), file) && valueCounter < 3) {
        if (line[0] == '#' || line[0] == '\n') continue;
        int value;
        if (sscanf(line, "%d", &value) == 1) {
            scores[valueCounter++] = value;
        }
    }
    bubbleSortDescending(scores, 3);
    fclose(file);
}
//funkcja nadpisująca leaderboard
void saveLeaderboard(const char* filename, int leaderboard[], const int current_points, int position) {
    for (int j = LEADERBOARD_SIZE - 1; j > position; --j) {
        leaderboard[j] = leaderboard[j - 1];
    }
    leaderboard[position] = current_points;
    // Zapis wyników do pliku
    FILE* file = fopen(filename, "w");
    if (file == nullptr) {
        return;
    }

    for (int i = 0; i < LEADERBOARD_SIZE; ++i) {
        fprintf(file, "%d\n", leaderboard[i]);
    }
    fclose(file);
}
/*
 ----------------------------------------------------------------
 |                                                              |
 |                   ROZGRYWKA Z/DO PLIKU                       |
 |                                                              |
 ----------------------------------------------------------------
 */

void replayGame(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Nie można otworzyć pliku do odtwarzania.\n");
        return;
    }

    char line[256];
    system("cls");

    while (fgets(line, sizeof(line), file)) {
        if (strcmp(line, "END_FRAME\n") == 0) {
            _sleep(100);
            system("cls");
        } else {
            printf("%s", line); // Wyświetlanie linii planszy
        }
    }

    fclose(file);
}
