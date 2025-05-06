#include "graphics.h"
#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include "defs.h"

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

int load_bmp_and_check(const char* file_path, SDL_Surface** surface, SDL_Surface* charset, SDL_Surface* screen, SDL_Texture* scrtex, SDL_Window* window, SDL_Renderer* renderer) {
    *surface = SDL_LoadBMP(file_path);
    if (*surface == NULL) {
        printf("SDL_LoadBMP(%s) error: %s\n", file_path, SDL_GetError());
        SDL_FreeSurface(charset);
        SDL_FreeSurface(screen);
        SDL_DestroyTexture(scrtex);
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return 1;  // Błąd podczas ładowania pliku BMP
    }
    return 0;  // Sukces
}

// narysowanie napisu txt na powierzchni screen, zaczynaj¹c od punktu (x, y)
// charset to bitmapa 128x128 zawieraj¹ca znaki
// draw a text txt on surface screen, starting from the point (x, y)
// charset is a 128x128 bitmap containing character images
void DrawString(SDL_Surface *screen, int x, int y, const char *text,
                SDL_Surface *charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while(*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
		};
	};


// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt rodka obrazka sprite na ekranie
// draw a surface sprite on a surface screen in point (x, y)
// (x, y) is the center of sprite on screen
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
	};


// rysowanie pojedynczego pixela
// draw a single pixel
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32 *)p = color;
	};


// rysowanie linii o d³ugoci l w pionie (gdy dx = 0, dy = 1) 
// b¹d poziomie (gdy dx = 1, dy = 0)
// draw a vertical (when dx = 0, dy = 1) or horizontal (when dx = 1, dy = 0) line
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for(int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
		};
	};


// rysowanie prostok¹ta o d³ugoci boków l i k
// draw a rectangle of size l by k
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,
                   Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for(i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
	};

// Funkcja rysująca obiekt z efektem pulsowania
void DrawPulsatingObject(SDL_Surface *screen, SDL_Surface *object, int x, int y, double worldTime, double frequency) {
    double scale = 1.0 + 0.2 * sin(worldTime * frequency);

    int newWidth = (int)(object->w * scale);
    int newHeight = (int)(object->h * scale);

    int offsetX = (newWidth - object->w) / 2;
    int offsetY = (newHeight - object->h) / 2;

    SDL_Rect destRect = {x - offsetX, y - offsetY, newWidth, newHeight};

    SDL_BlitScaled(object, NULL, screen, &destRect);
}
int loadBmps(SDL_Surface **screen, SDL_Surface **charset,
             SDL_Surface **game, SDL_Surface **eti,
             SDL_Surface **head, SDL_Surface **body, SDL_Surface **tail,
             SDL_Surface **food, SDL_Surface **bonus,
             SDL_Texture *scrtex, SDL_Window *window, SDL_Renderer *renderer) {
    if (load_bmp_and_check("./cs8x8.bmp", charset, *charset, *screen, scrtex, window, renderer)) {
        return 1;
    }
    SDL_SetColorKey(*charset, true, 0x000000);

    if (load_bmp_and_check("./eti.bmp", eti, *charset, *screen, scrtex, window, renderer)) {
        return 1;
    }
    if (load_bmp_and_check("./snakehead.bmp", head, *charset, *screen, scrtex, window, renderer)) {
        return 1;
    }
    if (load_bmp_and_check("./snakebody.bmp", body, *charset, *screen, scrtex, window, renderer)) {
        return 1;
    }
    if (load_bmp_and_check("./snaketail.bmp", tail, *charset, *screen, scrtex, window, renderer)) {
        return 1;
    }
    if (load_bmp_and_check("./blue_dot.bmp", food, *charset, *screen, scrtex, window, renderer)) {
        return 1;
    }
    if (load_bmp_and_check("./red_dot.bmp", bonus, *charset, *screen, scrtex, window, renderer)) {
        return 1;
    }
    return 0; // Sukces
}
void updateSnakeHeadDirection(SDL_Surface *&head, int dx, int dy) {
    if (dx == THRESHOLD && dy == 0) {
        // Poruszanie w prawo
        head = SDL_LoadBMP("snakehead4.bmp");
    } else if (dx == -THRESHOLD && dy == 0) {
        // Poruszanie w lewo
        head = SDL_LoadBMP("snakehead2.bmp");
    } else if (dx == 0 && dy == THRESHOLD) {
        // Poruszanie w dół
        head = SDL_LoadBMP("snakehead3.bmp");
    } else if (dx == 0 && dy == -THRESHOLD) {
        // Poruszanie w górę
        head = SDL_LoadBMP("snakehead1.bmp");
    }
}
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
              int numberOfPairs,
              int&dx,int&dy
              )
{
    // Liczenie FPS
    fpsTimer += delta;
    if (fpsTimer > 0.5)
    {
        fps = frames * 2;
        frames = 0;
        fpsTimer -= 0.5;
    }
    // Czyszczenie ekranu
    SDL_FillRect(screen, NULL, czarny);
    // rysowanie obszaru pola gry
    DrawRectangle(screen, 0, INFO_HEIGHT, GAME_WIDTH, GAME_HEIGHT, czarny, czarny);
    //rysowanie jedzenia
    DrawPulsatingObject(screen, food, foodX, foodY, worldTime, 2.0);
    // Obsługa czasu bonusu
    if (bonusVisible && (worldTime - bonusStartTime > BONUS_DURATION))
    {
        bonusVisible = 0; // Bonus znika po czasie
    }
    // Rysowanie bonusu na planszy
    if (bonusVisible)
    {
        DrawPulsatingObject(screen, bonus, bonusX, bonusY, worldTime, 3.0);
        double progress = (BONUS_DURATION - (worldTime - bonusStartTime)) / BONUS_DURATION;
    }
    // rysowanie weza
    updateSnakeHeadDirection(head, dx, dy);
    DrawSurface(screen, head, snakeX[0], snakeY[0]);
    for (int i = 1; i < snakeLength; i++)
    {
        if (i == snakeLength - 1)
        {
            DrawSurface(screen, tail, snakeX[i], snakeY[i]); // Rysowanie ogona
        }
        else
        {
            if (i % 2 == 0)
            {
                // Mniejszy segment
                SDL_Rect smallBodyRect = {
                    snakeX[i] - static_cast<int>(body->w * 0.8) / 2, // Odsunięcie o połowę przeskalowanej szerokości
                    snakeY[i] - static_cast<int>(body->h * 0.8) / 2, // Odsunięcie o połowę przeskalowanej wysokości
                    static_cast<int>(body->w * 0.8), // Przeskalowana szerokość
                    static_cast<int>(body->h * 0.8)  // Przeskalowana wysokość
                };

                //SDL_Rect smallBodyRect = {snakeX[i] - body->w / 4, snakeY[i] - body->h / 4, body->w / 2, body->h / 2};
                SDL_BlitScaled(body, NULL, screen, &smallBodyRect);
            }
            else
            {
                // Większy segment
                DrawSurface(screen, body, snakeX[i], snakeY[i]);
            }
        }
    }
    // Rysowanie portali
    for (int pairIndex = 0; pairIndex < numberOfPairs; ++pairIndex)
    {
        for (int portalIndex = 0; portalIndex < 2; ++portalIndex)
        {
            // Numer pary jako tekst
            sprintf(text, "%d", pairIndex+1);

            // Współrzędne portalu
            int portalX = teleportX[pairIndex][portalIndex];
            int portalY = teleportY[pairIndex][portalIndex];

            // Rysowanie numeru portalu

            DrawRectangle(screen,portalX-THRESHOLD/2,portalY-THRESHOLD/2,THRESHOLD,THRESHOLD,fioletowy,fioletowy);
            DrawString(screen, portalX, portalY, text, charset);
        }
    }
    // Informacje tekstowe
    DrawRectangle(screen, INFO_X, INFO_Y, INFO_WIDTH, INFO_HEIGHT, czerwony, niebieski);
    sprintf(text, "Czas trwania = %.1lf s  %.0lf klatek / s Punkty = %d Autor: Piotr Skierka 203246 COMPLETED:1-4 ABCDEFGH", worldTime, fps, (int)points);
    DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);
    //Progressbar
    DrawRectangle(screen, BAR_X, BAR_Y, BAR_WIDTH, BAR_HEIGHT, czerwony, czarny);
    SDL_Rect progressBarRect = {BAR_X + 2, BAR_Y + 2 + (BAR_HEIGHT - progressHeight), BAR_WIDTH - 4, progressHeight}; // Wewnętrzny pasek
    SDL_FillRect(screen, &progressBarRect, czerwony);
    // Aktualizacja ekranu
    SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
    SDL_RenderCopy(renderer, scrtex, NULL, NULL);
    SDL_RenderPresent(renderer);
    frames++;
}