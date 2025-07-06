#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <time.h>
#include "game.h"

using namespace std;

bool init();
void kill();
bool loop();


// Pointers to our window and renderer
SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* font;

//Defining the width and height of the screen
const int WIDTH = 640;
const int HEIGHT = 480;

int binaryNumber[8] = {0,0,0,0,0,0,0,0};

vector<enemy> enemyVector;
vector<BinaryPad> binaryPadVector;
vector<Missile> missileSpriteVector;


int main(int argc, char** args) {
	
	srand(time(NULL));
	
	if ( !init() ) return 1;

	while ( loop() ) {
		SDL_Delay(100);
	}

	kill();
	return 0;
}

bool loop() {

	static const unsigned char* keys = SDL_GetKeyboardState( NULL );

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
			// test keycode
				switch ( e.key.keysym.sym ) {
					case SDLK_1: 
						changeState(0,binaryPadVector,binaryNumber,enemyVector);
						break;
					case SDLK_2:
						changeState(1,binaryPadVector,binaryNumber,enemyVector);
						break;
					case SDLK_3: 
						changeState(2,binaryPadVector,binaryNumber,enemyVector);
						break;
					case SDLK_4:
						changeState(3,binaryPadVector,binaryNumber,enemyVector);
						break;
					case SDLK_5: 
						changeState(4,binaryPadVector,binaryNumber,enemyVector);
						break;
					case SDLK_6:
						changeState(5,binaryPadVector,binaryNumber,enemyVector);
						break;
					case SDLK_7: 
						changeState(6,binaryPadVector,binaryNumber,enemyVector);
						break;
					case SDLK_8:
						changeState(7,binaryPadVector,binaryNumber,enemyVector);
						break;
						
					case SDLK_SPACE:
						
						break;
				}
				break;
		}
	}

	

	// Test key states - this could also be done with events - Used for continuios input
	if ( keys[SDL_SCANCODE_W] ) cout << "W was pressed"<<endl;
	
	if ( keys[SDL_SCANCODE_A] ) cout << "A was pressed"<<endl;
	
	if ( keys[SDL_SCANCODE_S] ) cout << "S was pressed"<<endl;
	
	if ( keys[SDL_SCANCODE_D] ) cout << "D was pressed"<<endl;
	
	if (SDL_HasIntersection(missileSpriteVector[0].getRect(),enemyVector[0].getRect())){
		enemyVector.erase(enemyVector.begin());
		missileSpriteVector.erase(missileSpriteVector.begin());
		createEnemy();
	}
	
	for (uint8_t i = 0; i < enemyVector.size(); i++) {
		enemyVector.at(i).draw(renderer,font);
	}
	
	for (uint8_t i = 0; i < missileSpriteVector.size(); i++) {
		missileSpriteVector.at(i).draw(renderer);
	}
	
	//SDL_SetRenderDrawColor( renderer, 0,0, 255, 255 );
	
	for (int i = 0; i < 8; i++){
		binaryPadVector.at(i).draw(renderer,font);
	}
	
	// Update window
	SDL_RenderPresent( renderer );
	
	return true;
}





bool init() {
	
	enemy e1;
	
	enemyVector.push_back(e1);
	
	BinaryPad p1(0,0);
	BinaryPad p2(0,1);
	BinaryPad p3(0,2);
	BinaryPad p4(0,3);

	BinaryPad p5(0,4);
	BinaryPad p6(0,5);
	BinaryPad p7(0,6);
	BinaryPad p8(0,7);
	
	binaryPadVector.push_back(p1);
	binaryPadVector.push_back(p2);
	binaryPadVector.push_back(p3);
	binaryPadVector.push_back(p4);

	binaryPadVector.push_back(p5);
	binaryPadVector.push_back(p6);
	binaryPadVector.push_back(p7);
	binaryPadVector.push_back(p8);
	
	for(int i = 0; i < 8; i++){
		//Setting the position of each binary pad to be equal spaces
		binaryPadVector.at(i).changeX((i*80)+ 15);
	}
	
	if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
		cout << "Error initializing SDL: " << SDL_GetError() << endl;
		system("pause");
		return false;
	} 

	window = SDL_CreateWindow( "Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN );
	if ( !window ) {
		cout << "Error creating window: " << SDL_GetError()  << endl;
		system("pause");
		return false;
	}

	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
	if ( !renderer ) {
		cout << "Error creating renderer: " << SDL_GetError() << endl;
		return false;
	}
	
	if ( TTF_Init() < 0 ) {
		cout << "Error intializing SDL_ttf: " << TTF_GetError() << endl;
		return false;
	}
	
	font = TTF_OpenFont("Fonts/Minecraft.ttf", 24);
	if ( !font ) {
		cout << "Failed to load font: " << TTF_GetError() << endl;
	}

	SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
	SDL_RenderClear( renderer );
	
	//Add an enemy every 10 seconds
	SDL_AddTimer(3000, addEnemy, nullptr);  
	return true;
}

void kill() {
	// Quit
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	SDL_Quit();
}


