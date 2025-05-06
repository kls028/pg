#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "defs.h"
extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

int load_bmp_and_check(const char* file_path, SDL_Surface** surface, SDL_Surface* charset, SDL_Surface* screen, SDL_Texture* scrtex, SDL_Window* window, SDL_Renderer* renderer);
void DrawString(SDL_Surface *screen, int x, int y, const char *text, SDL_Surface *charset);
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y);
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color);
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color);
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor);
void DrawPulsatingObject(SDL_Surface *screen, SDL_Surface *object, int x, int y, double worldTime, double frequency);
int loadBmps(SDL_Surface **screen, SDL_Surface **charset,
             SDL_Surface **game, SDL_Surface **eti,
             SDL_Surface **head, SDL_Surface **body, SDL_Surface **tail,
             SDL_Surface **food, SDL_Surface **bonus,
             SDL_Texture *scrtex, SDL_Window *window, SDL_Renderer *renderer);
void drawGame(double &fpsTimer,double &delta,double &fps,
              SDL_Surface *screen,
              SDL_Surface *charset,
              SDL_Surface *game,
              SDL_Surface *eti,
              SDL_Surface *head,
              SDL_Surface *body,
              SDL_Surface *tail,
              SDL_Surface *food,
              SDL_Surface *bonus,
              SDL_Texture *scrtex,
              SDL_Window *window,
              SDL_Renderer *renderer,
              char *text,
              const int czarny,
              const int zielony,
              const int czerwony,
              const int niebieski,
              const int fioletowy,
              int &frames,
              int &foodX,
              int &foodY,
              int &bonusX,
              int &bonusY,
              int &bonusVisible,
              double &lastUpdateTime,
              double &worldTime,
              int &points,
              int &progressHeight,
              double &bonusStartTime,
              int& snakeLength,
              int *snakeX,
              int *snakeY,
              int teleportX[MAX_TELEPORT_SIZE][2],
              int teleportY[MAX_TELEPORT_SIZE][2],
              int numberOfPairs,int&dx,int&dy
              );
#endif
