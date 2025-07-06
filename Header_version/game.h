#ifndef GAME_H_   /* Include guard */
#define GAME_H_
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>

using namespace std;

int add(int a,int b);  /* An example function declaration */


class enemy{
	public:
		//Constructor
		enemy();
		//A function to draw the enemy
		void draw(SDL_Renderer* renderer,TTF_Font* font);

		char* getHexData(){
			return hexData;
		}
		
		int getPosX(){
			return enemyRect.x;
		}
		
		SDL_Rect* getRect(){
			return &enemyRect;
		}
		
		bool defeated;
		
	private:
		//Defines the shape of the enemy
		SDL_Rect enemyRect;
		//Shows the hex value it represents in a string format
		char hexData[2];
};


class BinaryPad{
	public:
		//Constructor with starting pos x and pad number as its parameters
		BinaryPad(int startingX,int inputPadNumber);
		//Function to draw the pad
		void draw(SDL_Renderer* renderer,TTF_Font* font);
		//Function to change the x pos
		void changeX(int x);
		
		//Returns pad number
		int getPadNumber(){
			return padNumber;
		}
		
		int getState(){
			return state;
		}

		void setState(int inputState){
			state  = inputState;
		}
		
	private:
		SDL_Rect padRect;
		
		int state;
	
		//The pad number it represents
		int padNumber;
};


class Missile{
	public:
		
		Missile();
		
		void draw(SDL_Renderer* renderer);
		
		SDL_Rect* getRect(){
			return &missileRect;
		}
		
		void changeX(int inputX){
			missileRect.x = inputX;
		}
	
	private:
		SDL_Rect missileRect;
};


/*struct Missile{
	SDL_Rect missileRect;
	void draw(SDL_Renderer* renderer);
	
	SDL_Rect* getRect();
};
*/

void changeState(int padNumber,vector<BinaryPad> &binaryPadVector,int binaryNumber[8],vector<enemy> &enemyVector);
char assignIntToHex(int inputNumber);
void drawText(const char* inputText,int x, int y,TTF_Font* font,SDL_Renderer* renderer);
bool checkIfBinaryNumberEqualsHex(int inputBinaryNumber[8],char inputHexData[2]);
bool check4BitEqualsHex(int inputBinaryNumber[4],char inputHexData);
void createMissile();
Uint32 addEnemy(Uint32,void*);
void createEnemy();

#endif // GAME_h_
