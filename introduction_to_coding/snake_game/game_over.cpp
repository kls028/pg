#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "graphics.h"
#include "gameover.h"
#include "defs.h"
extern "C"
{
#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
}
char **loadLeaderboard(const char *filename)
{
    FILE *file = fopen(filename, "r");

    char **leaderboard = (char **)malloc(3 * sizeof(char *));
    for (int i = 0; i < 3; i++)
    {
        leaderboard[i] = (char *)malloc(50 * sizeof(char));
    }

    char name[20];
    int score;
    int count = 0;
    while (fscanf(file, "%s %d", name, &score) == 2 && count < 3)
    {
        snprintf(leaderboard[count], 50, "%s %d", name, score);
        count++;
    }
    fclose(file);
    return leaderboard;
}
// Funkcja do sprawdzania pozycji wyniku gracza w tabeli
int checkLeaderboard(const char *filename, int score)
{
    char **leaderboard = loadLeaderboard(filename);
    if (!leaderboard)
    {
        printf("Error: Could not load leaderboard.\n");
        return 0;
    }

    int position = 0;
    for (int i = 0; i < 3; i++)
    {
        if (leaderboard[i][0] == '\0')
            break;

        int recordScore;
        sscanf(strrchr(leaderboard[i], ' ') + 1, "%d", &recordScore);

        if (score > recordScore)
        {
            position = i + 1;
            break;
        }
    }

    for (int i = 0; i < 3; i++)
    {
        free(leaderboard[i]);
    }
    free(leaderboard);

    return position;
}
void saveLeaderboard(const char *filename, const char *name, int score, int position)
{
    char **leaderboard = loadLeaderboard(filename);

    // Rozdzielenie istniejących rekordów na imiona i wyniki
    char names[3][20];
    int scores[3] = {0, 0, 0};
    for (int i = 0; i < 3; i++)
    {
        if (sscanf(leaderboard[i], "%s %d", names[i], &scores[i]) != 2)
        {
            strcpy(names[i], "");
            scores[i] = 0;
        }
    }

    // Przesuwanie rekordów w dół, aby zrobić miejsce na nowy wynik
    for (int i = 2; i >= position - 1; i--)
    {
        if (i < 2)
        {
            scores[i + 1] = scores[i];
            strcpy(names[i + 1], names[i]);
        }
    }

    // Wstawienie nowego wyniku na odpowiednią pozycję
    scores[position - 1] = score;
    strncpy(names[position - 1], name, 19);
    names[position - 1][19] = '\0';

    // Zapis zaktualizowanej tabeli do pliku
    FILE *file = fopen(filename, "w");
    if (file)
    {
        for (int i = 0; i < 3; i++)
        {
            if (scores[i] > 0)
            {
                fprintf(file, "%s %d\n", names[i], scores[i]);
            }
        }
        fclose(file);
    }

    // Zwolnienie pamięci z funkcji loadLeaderboard
    for (int i = 0; i < 3; i++)
    {
        free(leaderboard[i]);
    }
    free(leaderboard);
}
// Funkcja do wprowadzania imienia użytkownika
void getPlayerName(SDL_Renderer *renderer, SDL_Surface *screen, SDL_Texture *scrtex, SDL_Surface *charset, char *playerName)
{
    SDL_StartTextInput();
    int quit = 0;
    SDL_Event event;
    int charIndex = 0;

    // Czyszczenie bufora imienia
    for (int i = 0; i < 20; i++)
    {
        playerName[i] = '\0';
    }

    while (!quit)
    {
        // Obsługa zdarzeń SDL
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = 1;
            }
            else if (event.type == SDL_TEXTINPUT)
            {
                // Dodawanie znaków (ograniczenie do 19 znaków)
                if (charIndex < 19)
                {
                    playerName[charIndex] = event.text.text[0];
                    charIndex++;
                }
            }
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_BACKSPACE && charIndex > 0)
                {
                    // Usuwanie ostatniego znaku
                    charIndex--;
                    playerName[charIndex] = '\0';
                }
                else if (event.key.keysym.sym == SDLK_RETURN)
                {
                    quit = 1; // Użytkownik zakończył wprowadzanie
                }
            }
        }

        // Rysowanie popup okienka
        SDL_FillRect(screen, NULL, 0x000000);                                                  // Czyszczenie tła na czarno
        DrawRectangle(screen, 100, 100, screen->w - 200, screen->h - 200, 0x000000, 0x000000); // Białe obramowanie

        DrawString(screen, screen->w / 2 - strlen("Enter your name:") * 8 / 2, 150, "Enter your name:", charset);
        DrawString(screen, screen->w / 2 - charIndex * 8 / 2, 200, playerName, charset); // Wyświetlanie imienia

        SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
        SDL_RenderCopy(renderer, scrtex, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_StopTextInput();
}
void drawGameOverScreen(SDL_Surface *screen, SDL_Surface *charset, SDL_Texture *scrtex, SDL_Renderer *renderer,
                        const char *message, const char *instructions, SDL_Color textColor, SDL_Color bgColor, const char *filename)
{
    // Wypełnij cały obszar gry kolorem tła
    SDL_Rect gameOverRect = {0, INFO_HEIGHT, GAME_WIDTH, GAME_HEIGHT};
    Uint32 bg = SDL_MapRGB(screen->format, bgColor.r, bgColor.g, bgColor.b);
    SDL_FillRect(screen, &gameOverRect, bg);

    //komunikat "GAME OVER"
    int textWidth = strlen(message) * 8;
    int textX = SCREEN_WIDTH / 2 - textWidth / 2;
    int textY = INFO_HEIGHT + GAME_HEIGHT / 6;
    DrawString(screen, textX, textY, message, charset);

    // Załaduj leaderboard
    char **leaderboard = loadLeaderboard(filename);

    // Wyświetl leaderboard poniżej komunikatu
    textY += 40; // Odstęp między komunikatem a leaderboardem
    DrawString(screen, SCREEN_WIDTH / 2 - strlen("LEADERBOARD") * 8 / 2, textY, "LEADERBOARD", charset);
    textY += 20;

    for (int i = 0; i < 3; i++)
    {
        if (leaderboard[i][0] != '\0')
        {
            DrawString(screen, SCREEN_WIDTH / 2 - strlen(leaderboard[i]) * 8 / 2, textY, leaderboard[i], charset);
            textY += 20; // Odstęp między wpisami
        }
    }

    textWidth = strlen(instructions) * 8;
    textX = SCREEN_WIDTH / 2 - textWidth / 2;
    textY += 40;
    DrawString(screen, textX, textY, instructions, charset);

    // Aktualizacja ekranu
    SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
    SDL_RenderCopy(renderer, scrtex, NULL, NULL);
    SDL_RenderPresent(renderer);

    for (int i = 0; i < 3; i++)
    {
        free(leaderboard[i]);
    }
    free(leaderboard);
}
void handleGameOverScreenKeyboard(int& quit, SDL_Event event, int&inGameOver)
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            quit = 1;
            inGameOver = 0;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                quit = 1;
                inGameOver = 0;
            }
            else if (event.key.keysym.sym == SDLK_n)
            {
                inGameOver = 0;
            }
        }
    }
}