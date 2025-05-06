#ifndef GAMEOVER_H
#define GAMEOVER_H
char** loadLeaderboard(const char* filename);
int checkLeaderboard(const char* filename, int score);
void saveLeaderboard(const char* filename, const char* name, int score, int position);
void getPlayerName(SDL_Renderer* renderer, SDL_Surface* screen, SDL_Texture* scrtex, SDL_Surface*charset,char* playerName);
void drawGameOverScreen(SDL_Surface *screen, SDL_Surface *charset, SDL_Texture *scrtex, SDL_Renderer *renderer,
                        const char *message, const char *instructions, SDL_Color textColor, SDL_Color bgColor, const char *filename);
void handleGameOverScreenKeyboard(int& quit, SDL_Event event, int&inGameOver);
#endif