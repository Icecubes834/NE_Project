#include <stdio.h>
#include <SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <SDL_ttf.h>
#include <math.h>

#define MAX_ENEMIES 20

bool init();
void kill();
bool loop();

const int WIDTH = 640;
const int HEIGHT = 480;
const int ENEMYSPAWNRATE = 300;
int DEADLINE = 480 - 100;
int missileDefaultPos = 0xffff;

SDL_Color BLACK = {0,0,0};


int binaryNumber[8][2] = {
	{128,0},{64,0},{32,0},{16,0},
	{8,0},{4,0},{2,0},{1,0}
};

struct binaryPad{
    SDL_Rect rect;
    bool state;
    int padNumber;
};

struct enemy{
    SDL_Rect rect;
    bool defeated;
    char hexText[3];

    SDL_Surface* text;
    SDL_Texture* text_texture;
    SDL_Rect dest;
};


SDL_Rect missile;

struct binaryPad binaryPadArray[8];
struct enemy *enemyArrayPtr;

int enemyArrayHead;
int enemyArrayTail;
size_t numOfEnemies;
size_t enemyArrayCapacity;
size_t framesCounted;

void changeState(int padNumber);
char assignHexText();
bool addEnemy();
bool checkInput();
void removeEnemy();

// Pointers to our window and renderer
SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* font;

int main(int argc, char** args) {

    srand(time(NULL));

	if ( !init() ) return 1;

	while ( loop() ){}

	kill();
	return 0;
}

bool loop() {

    Uint64 start = SDL_GetPerformanceCounter();

	SDL_Event e;

	// Clear the window to white
	SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
	SDL_RenderClear( renderer );

	// Event loop
	while ( SDL_PollEvent( &e ) != 0 ) {
		switch ( e.type ) {
			case SDL_QUIT:
				return false;

            case SDL_KEYDOWN:
                for (int keyCode = 49, i = 0; keyCode < 59; keyCode++,i++){
                    if (e.key.keysym.sym == keyCode){
                        changeState(i);
                        checkInput();
                        break;
                    }
                }
                switch ( e.key.keysym.sym ) {
                    case SDLK_e:
                        addEnemy();
                        break;
                    }
            }
	}

	if(enemyArrayPtr[enemyArrayHead].rect.y + enemyArrayPtr[enemyArrayHead].rect.h  > DEADLINE && numOfEnemies>0) return false;

    if(framesCounted % ENEMYSPAWNRATE == 0) addEnemy();

    if(missile.y < (enemyArrayPtr[enemyArrayHead].rect.y + enemyArrayPtr[enemyArrayHead].rect.h) && numOfEnemies>0){
        removeEnemy();
        missile.y = missileDefaultPos;
    }


    for (int i =0; i < 8; i++){
        if (binaryPadArray[i].state == false){
            SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
        }else{
            SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );
        }
        SDL_RenderFillRect( renderer, &binaryPadArray[i].rect );
    }

    for (int i = enemyArrayHead; i < enemyArrayTail+1; i++){
        enemyArrayPtr[i].rect.y++;

        enemyArrayPtr[i].dest.x = enemyArrayPtr[i].rect.x;
        enemyArrayPtr[i].dest.y = enemyArrayPtr[i].rect.y;

        SDL_SetRenderDrawColor( renderer, 0, 255, 0, 255 );
        SDL_RenderFillRect( renderer, &enemyArrayPtr[i].rect );
        SDL_RenderCopy( renderer, enemyArrayPtr[i].text_texture,NULL, &enemyArrayPtr[i].dest );
    }


    SDL_SetRenderDrawColor( renderer, 255, 255, 0, 255 );
    missile.y-= 5;

    SDL_RenderFillRect( renderer, &missile );

	// Update window
	SDL_RenderPresent( renderer );

	Uint64 end = SDL_GetPerformanceCounter();

	float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

	// Cap to 60 FPS
	if (16.666f > elapsedMS) SDL_Delay(floor(16.666f - elapsedMS));

	framesCounted++;

	return true;
}

bool init() {
    framesCounted = 0;
    enemyArrayCapacity = 5;
    enemyArrayPtr = calloc(enemyArrayCapacity,sizeof(struct enemy));

	enemyArrayHead = 0;
    enemyArrayTail = 0;

    missile.x = 0;
    missile.y = missileDefaultPos;
    missile.w = 30;
    missile.h = 75;

	if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
		printf("Error initializing SDL: %s",SDL_GetError());
		system("pause");
		return false;
	}

	// Initialize SDL_ttf
	if ( TTF_Init() < 0 ) {
		printf("Error intializing SDL_ttf: %s",TTF_GetError());
		return false;
	}

	window = SDL_CreateWindow( "Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN );
	if ( !window ) {
		printf("Error creating window: %s",SDL_GetError());
		system("pause");
		return false;
	}

	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
	if ( !renderer ) {
		printf("Error creating renderer: %s",SDL_GetError());
		return false;
	}

	for (int i =0; i < 8; i++){
        struct binaryPad binaryPadBuffer = {
            {(i*WIDTH/8)+15,HEIGHT - 70,50,50},
            false,
            i
        };
        binaryPadArray[i] = binaryPadBuffer;
	}

	font = TTF_OpenFont("Fonts/Minecraft.ttf", 24);
	if ( !font ) {
		printf("Error loading font: %s\n",TTF_GetError());
		return false;
	}

	SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
	SDL_RenderClear( renderer );
	return true;
}

void kill() {
	// Quit
	free(enemyArrayPtr);

    font = NULL;
	TTF_CloseFont( font );


	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	SDL_Quit();
}
















void changeState(int padNumber){
    for (int i =0; i < 8; i++){
        if(padNumber == binaryPadArray[i].padNumber){
            binaryPadArray[i].state = !binaryPadArray[i].state;
            if(binaryNumber[i][1] == 0) binaryNumber[i][1] = 1;
            else binaryNumber[i][1] = 0;
        }
    }
}

char assignHexText(){
    int num = rand() % 15;

    if(num < 10)
        return num + '0';
    else
        return num + 55;

    return '0';
}

bool addEnemy(){

    int randXPos = rand() % WIDTH;

    if (enemyArrayTail >= enemyArrayCapacity){
        enemyArrayCapacity = enemyArrayCapacity * 2;

        printf("Array is full, resizing array to %d!\n",enemyArrayCapacity);

        enemyArrayPtr = realloc(enemyArrayPtr,enemyArrayCapacity * sizeof(struct enemy));
        if(enemyArrayPtr == NULL){
            printf("Failed. Unable to resize array!\n");
            return false;
        }
    }

    struct enemy enemyBuffer = {
        {randXPos,-100,50,50},
        false,
        {assignHexText(),assignHexText(),'\0'}
    };

    enemyBuffer.text = TTF_RenderText_Solid( font,enemyBuffer.hexText , BLACK );

    if ( !enemyBuffer.text ) {
        printf("Failed to render text: %s\n",TTF_GetError());
        return false;
    }

    enemyBuffer.text_texture = SDL_CreateTextureFromSurface( renderer,enemyBuffer.text );

    enemyBuffer.dest.w = enemyBuffer.text->w;
    enemyBuffer.dest.h = enemyBuffer.text->h;

    enemyArrayPtr[enemyArrayTail] = enemyBuffer;
    enemyArrayTail++;
    printf("%s\n",enemyBuffer.hexText);
    numOfEnemies++;
    return true;
}


void removeEnemy(){
    if (numOfEnemies > 0){
        enemyArrayHead++;
        numOfEnemies--;
    }else{
        printf("Array is empty!\n");
    }
}



bool checkInput(){

    if (numOfEnemies > 0){
        int currentNum = 0;

        int realHex;

        for (int i =0; i < 8;i++){
            if(binaryNumber[i][1]==1) currentNum += binaryNumber[i][0];
        }

        sscanf(enemyArrayPtr[enemyArrayHead].hexText,"%x",&realHex);

        if(currentNum == realHex){
            printf("Correct\n");
            if (enemyArrayPtr[enemyArrayHead].defeated == false){
                missile.y = HEIGHT - 100;
                missile.x = enemyArrayPtr[enemyArrayHead].rect.x;
                enemyArrayPtr[enemyArrayHead].defeated = true;
            }
            return true;
        }else{
            printf("Incorrect\n");
            return false;
        }

    }

    return false;
}

