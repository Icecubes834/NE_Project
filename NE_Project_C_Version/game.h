#ifndef GAME_H_   /* Include guard */
#define GAME_H_

#include <SDL.h>
#include <stdbool.h>
#include <SDL_ttf.h>


#define HEIGHT 480
#define WIDTH 640
#define MAX_ENEMIES 5
#define MISSILE_SPEED 5

struct binaryPad{
    SDL_Rect rect;
    bool state;
    int padNumber;
};

typedef struct{
    SDL_Rect rect;
    bool defeated;
    char hexText[3];

    SDL_Surface* text_surface;
    SDL_Texture* text_texture;
    SDL_Rect dest;
}enemy;


extern struct binaryPad binaryPadArray[8];
extern int binaryNumber[8][2];
extern TTF_Font* font;
extern SDL_Color BLACK;
extern SDL_Renderer* renderer;
extern enemy enemyArray[MAX_ENEMIES];
extern size_t ptrToFirstEnemy;
extern size_t ptrToLastEnemy;
extern SDL_Rect missile;

void changeState(int padNumber);
char assignHexText();
void createEnemy(enemy *enemyBuffer);
bool checkInput();
void removeEnemy();


#endif // GAME_H_
