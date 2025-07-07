#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>

#define MAX_ENEMIES 20


struct binaryPad{
    SDL_Rect rect;
    bool state;
    int padNumber;
};

typedef struct{
    SDL_Rect rect;
    bool defeated;
    char hexText[3];

    SDL_Surface* text;
    SDL_Texture* text_texture;
    SDL_Rect dest;
}enemy;


//Function decleration
bool init();
void Vkill();
bool loop();
void changeState(int padNumber);
char assignHexText();
void createEnemy(enemy *enemyBuffer);
bool checkInput();
void removeEnemy();

//Constants
const int WIDTH = 640;
const int HEIGHT = 480;
const int ENEMYSPAWNRATE = 300;
SDL_Color BLACK = {0,0,0};


//Variables
int DEADLINE = 480 - 100;
int missileDefaultPos = 0xffff;
size_t ptrToFirstEnemy = 0;
size_t ptrToLastEnemy = 4;


int binaryNumber[8][2] = {
	{128,0},{64,0},{32,0},{16,0},
	{8,0},{4,0},{2,0},{1,0}
};
SDL_Rect missile;
struct binaryPad binaryPadArray[8];
enemy enemyArray[5];
size_t framesCounted;





// Pointers to our window and renderer
SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* font;

int main(int argc, char** args) {

    srand(time(NULL));

	if ( !init() ) return 1;

	while ( loop() ){}

	Vkill();
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
                        //addEnemy();
                        break;
                    }
            }
	}
	
	if (enemyArray[ptrToFirstEnemy].rect.y + enemyArray[ptrToFirstEnemy].rect.h > DEADLINE){
		return false;	
	}

	if (missile.y < enemyArray[ptrToFirstEnemy].rect.y+enemyArray[ptrToFirstEnemy].rect.h){
		missile.y = missileDefaultPos;
		enemyArray[ptrToFirstEnemy].rect.y = enemyArray[ptrToLastEnemy].rect.y - 100;
		enemyArray[ptrToFirstEnemy].defeated = false;
		
		(ptrToFirstEnemy == 4) ? ptrToFirstEnemy = 0 : ptrToFirstEnemy++;
		(ptrToLastEnemy == 4) ? ptrToLastEnemy = 0 : ptrToLastEnemy++;
		
	}


	
	
    for (int i =0; i < 8; i++){
        if (binaryPadArray[i].state == false){
            SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
        }else{
            SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );
        }
        SDL_RenderFillRect( renderer, &binaryPadArray[i].rect );
    }
	

	
	for (int i = 0;i < 5; i++){
		SDL_SetRenderDrawColor( renderer, 0, 255, 0, 255 );
		
		enemyArray[i].rect.y++;	
		SDL_RenderFillRect( renderer, &enemyArray[i].rect );
		enemyArray[i].dest.y = enemyArray[i].rect.y;
		enemyArray[i].dest.x = enemyArray[i].rect.x;
		SDL_RenderCopy(renderer,enemyArray[i].text_texture,NULL,&enemyArray[i].dest);		
	}

    SDL_SetRenderDrawColor( renderer, 255, 255, 0, 255 );
    missile.y-= 5;

    SDL_RenderFillRect( renderer, &missile );

	// Update window
	SDL_RenderPresent( renderer );








	Uint64 end = SDL_GetPerformanceCounter();

	float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

	// Cap to 60 FPS
	if (16.666*2 > elapsedMS) SDL_Delay(floor(16.666*2 - elapsedMS));

	framesCounted++;

	return true;
}

bool init() {
    framesCounted = 0;
 
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

	for(int i =0;i <5; i++){
		enemy enemyBuff;
		createEnemy(&enemyBuff);
		enemyBuff.rect.y *= i;
		enemyArray[i]=enemyBuff;
	}


	SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
	SDL_RenderClear( renderer );
	return true;
}

void Vkill() {
	// Quit

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

void createEnemy(enemy *enemyBuffer){

    int randXPos = rand() % WIDTH;

	enemyBuffer->rect.x = randXPos;
	enemyBuffer->rect.y = -100;
	enemyBuffer->rect.w = 50;
	enemyBuffer->rect.h = 50;
	enemyBuffer->defeated = false;
	enemyBuffer->hexText[0]= assignHexText();
	enemyBuffer->hexText[1]= assignHexText();
	enemyBuffer->hexText[2]= '\0';

    enemyBuffer->text = TTF_RenderText_Solid( font,enemyBuffer->hexText , BLACK );

    if ( !enemyBuffer->text ) {
        printf("Failed to render text: %s\n",TTF_GetError());
    }

    enemyBuffer->text_texture = SDL_CreateTextureFromSurface( renderer,enemyBuffer->text );

    enemyBuffer->dest.w = enemyBuffer->text->w;
    enemyBuffer->dest.h = enemyBuffer->text->h;

    printf("%s\n",enemyBuffer->hexText);
}


bool checkInput(){
   
	int currentNum = 0;

	int realHex;

    for (int i =0; i < 8;i++){
    	if(binaryNumber[i][1]==1) currentNum += binaryNumber[i][0];
    }

    sscanf(enemyArray[ptrToFirstEnemy].hexText,"%x",&realHex);
	
    if(currentNum == realHex && enemyArray[ptrToFirstEnemy].defeated == false){
    	missile.y = HEIGHT - 100;
		missile.x = enemyArray[ptrToFirstEnemy].rect.x;
		enemyArray[ptrToFirstEnemy].defeated = true;
    	printf("Pointer to first enemy is %d\n",ptrToFirstEnemy);
        return true;
    }else{
        
        return false;
    }
	
    return false;
}

