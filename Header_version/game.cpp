#include "game.h"  /* Include the header (not strictly necessary here) */
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>


using namespace std;

const int WIDTH = 640;
const int HEIGHT = 480;

int add(int a,int b)    /* Function definition */
{
    return a + b;
}


enemy::enemy(){
	defeated = false;
	
	//Generate a random number between 0 and WIDTH - its width
	int randomStartXPosition =  rand() % (WIDTH-enemyRect.w);
	
	enemyRect = {randomStartXPosition,-enemyRect.h - 25,50,50};
	
	int firstRandomHexDigit = rand()%16;
	int secondRandomHexDigit = rand()%16;
	
	hexData[0] = assignIntToHex(firstRandomHexDigit);
	hexData[1] = assignIntToHex(secondRandomHexDigit);
	
	for (int i = 0; i < 2; i++){
		cout << hexData[i];
	} 
	
	cout << endl;
}

void enemy::draw(SDL_Renderer* renderer,TTF_Font* font){
	enemyRect.y += 1;
	SDL_SetRenderDrawColor(renderer,0, 255, 0, 100 );
	SDL_RenderFillRect(renderer,&enemyRect);
	
	drawText(hexData,enemyRect.x,enemyRect.y,font,renderer);
}






BinaryPad::BinaryPad(int startingX,int inputPadNumber){
	//Constructor
	padRect.w = 50;
	padRect.h = 50;
	
	padRect.y = HEIGHT - (padRect.h + 20);
	padRect.x = startingX;
	
	padNumber = inputPadNumber;
	
	state = 0;
	
}

void BinaryPad::changeX(int x){
	padRect.x = x;
}


void BinaryPad::draw(SDL_Renderer* renderer,TTF_Font* font){
	if (state == 1){
		SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );
	}else{
		SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
	}
	SDL_RenderFillRect(renderer,&padRect);
	
	
	if (state == 0){
		drawText("0",padRect.x,padRect.y, font,renderer);
	}else{
		drawText("1",padRect.x,padRect.y, font,renderer);
	}
}

Missile::Missile(){
	missileRect = {0,350,20,50};
}

void Missile::draw(SDL_Renderer* renderer){
	SDL_SetRenderDrawColor( renderer, 255, 255, 0, 255 );
	missileRect.y = missileRect.y - 5;
	SDL_RenderFillRect(renderer,&missileRect);
}



/*
struct Missile{
	SDL_Rect missileRect;
	void draw(SDL_Renderer* renderer){
		SDL_SetRenderDrawColor( renderer, 255, 255, 0, 255 );
		missileRect.y = missileRect.y - 5;
		SDL_RenderFillRect(renderer,&missileRect);
	}
	
	SDL_Rect* getRect(){
		return &missileRect;
	}
};
*/





void changeState(int padNumber, vector<BinaryPad> &binaryPadVector,int binaryNumber[8],vector<enemy> &enemyVector){
	for (int i = 0;i < 8;i++){
		if (padNumber==binaryPadVector.at(i).getPadNumber()){
			if (binaryPadVector.at(i).getState() == 0){
				binaryPadVector.at(i).setState(1);
				binaryNumber[i] = 1;
			}else{
				binaryPadVector.at(i).setState(0);
				binaryNumber[i] = 0;
			}
			//cout << "Successfully changed state: " << binaryPadVector.at(i).getState() <<endl;
			for (int i = 0;i < 8;i++){
				//cout << binaryNumber[i];
			}
		}
	}
	
	if(checkIfBinaryNumberEqualsHex(binaryNumber,enemyVector[0].getHexData()) && enemyVector[0].defeated == false){
		cout << "Got it correct" << endl;
		createMissile();
		enemyVector[0].defeated = true;
	}
}

bool check4BitEqualsHex(int fourBitBuffer[4],char inputHexData){
	switch(inputHexData){
		//Checking numbers
		case '0':
			if(fourBitBuffer[0] == 0 && fourBitBuffer[1] == 0 && fourBitBuffer[2] == 0 && fourBitBuffer[3] == 0 ){
				return true;
				
			}else{
				return false;
			}
			break;
		case '1':
			if(fourBitBuffer[3] == 0 && fourBitBuffer[1] == 0 && fourBitBuffer[2] == 0 && fourBitBuffer[3] == 1 ){
				return true;
				
			}else{
				return false;
			}
			break;
		case '2':
			if(fourBitBuffer[0] == 0 && fourBitBuffer[1] == 0 && fourBitBuffer[2] == 1 && fourBitBuffer[3] == 0 ){
				return true;
				
			}else{
				return false;
			}
			break;
		case '3':
			if(fourBitBuffer[0] == 0 && fourBitBuffer[1] == 0 && fourBitBuffer[2] == 1 && fourBitBuffer[3] == 1 ){
				return true;
				
			}else{
				return false;
			}
			break;
		case '4':
			if(fourBitBuffer[0] == 0 && fourBitBuffer[1] == 1 && fourBitBuffer[2] == 0 && fourBitBuffer[3] == 0 ){
				return true;
				
			}else{
				return false;
			}
			break;
		case '5':
			if(fourBitBuffer[0] == 0 && fourBitBuffer[1] == 1 && fourBitBuffer[2] == 0 && fourBitBuffer[3] == 1 ){
				return true;
				
			}else{
				return false;
			}
			break;
		case '6':
			if(fourBitBuffer[0] == 0 && fourBitBuffer[1] == 1 && fourBitBuffer[2] == 1 && fourBitBuffer[3] == 0 ){
				return true;
				
			}else{
				return false;
			}
			break;
		case '7':
			if(fourBitBuffer[0] == 0 && fourBitBuffer[1] == 1 && fourBitBuffer[2] == 1 && fourBitBuffer[3] == 1 ){
				return true;
				
			}else{
				return false;
			}
			break;
		case '8':
			if(fourBitBuffer[0] == 1 && fourBitBuffer[1] == 0 && fourBitBuffer[2] == 0 && fourBitBuffer[3] == 0 ){
				return true;
				
			}else{
				return false;
			}
			break;
		case '9':
			if(fourBitBuffer[0] == 1 && fourBitBuffer[1] == 0 && fourBitBuffer[2] == 0 && fourBitBuffer[3] == 1 ){
				return true;
				
			}else{
				return false;
			}
			break;
		//Checking letters
		case 'A':
			if(fourBitBuffer[0] == 1 && fourBitBuffer[1] == 0 && fourBitBuffer[2] == 1 && fourBitBuffer[3] == 0 ){
				return true;
				
			}else{
				return false;
			}
			break;
		case 'B':
			if(fourBitBuffer[0] == 1 && fourBitBuffer[1] == 0 && fourBitBuffer[2] == 1 && fourBitBuffer[3] == 1 ){
				return true;
				
			}else{
				return false;
			}
			break;
		case 'C':
			if(fourBitBuffer[0] == 1 && fourBitBuffer[1] == 1 && fourBitBuffer[2] == 0 && fourBitBuffer[3] == 0 ){
				return true;
				
			}else{
				return false;
			}
			break;
		case 'D':
			if(fourBitBuffer[0] == 1 && fourBitBuffer[1] == 1 && fourBitBuffer[2] == 0 && fourBitBuffer[3] == 1 ){
				return true;
				
			}else{
				return false;
			}
			break;
		case 'E':
			if(fourBitBuffer[0] == 1 && fourBitBuffer[1] == 1 && fourBitBuffer[2] == 1 && fourBitBuffer[3] == 0 ){
				return true;
				
			}else{
				return false;
			}
			break;
		case 'F':
			if(fourBitBuffer[0] == 1 && fourBitBuffer[1] == 1 && fourBitBuffer[2] == 1 && fourBitBuffer[3] == 1 ){
				return true;
				
			}else{
				return false;
			}
			break;
	}
	
	return false;
}

bool checkIfBinaryNumberEqualsHex(int inputBinaryNumber[8],char inputHexData[2]){
	bool isFirstHalfOfBinaryNumberCorrect;
	bool isSecondHalfOfBinaryNumberCorrect;
	
	int fourBitBuffer[4];
	int secondFourBitBuffer[4];
	//Getting the first half of the binary number and storing it into an array
	for (int i = 4, j = 0;i < 8,j < 4; i++,j++){
		fourBitBuffer[j] = inputBinaryNumber[i];
		cout << fourBitBuffer[j];
	}
	cout << " ";
	//Getting the second half of the binary number and storing it into another array
	
	for (int i = 0;i <4; i++){
		secondFourBitBuffer[i] = inputBinaryNumber[i];
		cout << secondFourBitBuffer[i];
	}
	cout << endl;
	//Since we are checking the first half of binary number, we need then second digit of the hex number, hence the second element
	isFirstHalfOfBinaryNumberCorrect = check4BitEqualsHex(fourBitBuffer,inputHexData[1]);
	isSecondHalfOfBinaryNumberCorrect = check4BitEqualsHex(secondFourBitBuffer,inputHexData[0]);
	
	if(isFirstHalfOfBinaryNumberCorrect==true && isSecondHalfOfBinaryNumberCorrect==true){
		return true;
	}else{
		return false;
	}
	//If somehow we get here, but do this to avoid compiler warnings
	cout << "Function reached a point it shouldn't have " << endl;
	return false;
}
char assignIntToHex(int inputNumber){
	char returnChar;
	if (inputNumber > 9){
		switch(inputNumber){
			case 10:
				returnChar = 'A';
				break;
			
			case 11:
				returnChar = 'B';
				break;
			
			case 12:
				returnChar = 'C';
				break;
			
			case 13:
				returnChar = 'D';
				break;
			
			case 14:
				returnChar = 'E';
				break;
			
			case 15:
				returnChar = 'F';
				break;
		}
	}else{
		switch(inputNumber){
			case 0:
				returnChar = '0';
				break;
			case 1:
				returnChar = '1';
				break;
			case 2:
				returnChar = '2';
				break;
			case 3:
				returnChar = '3';
				break;
			case 4:
				returnChar = '4';
				break;
			case 5:
				returnChar = '5';
				break;
			case 6:
				returnChar = '6';
				break;
			case 7:
				returnChar = '7';
				break;
			case 8:
				returnChar = '8';
				break;
			case 9:
				returnChar = '9';
				break;
		}
	}
	
	return returnChar;
}


void drawText(const char* inputText, int x, int y,TTF_Font* font,SDL_Renderer* renderer){
	SDL_Surface* text;
	// Set color to black
	SDL_Color color = { 0, 0, 0 };

	text = TTF_RenderText_Solid( font, inputText, color );
	if ( !text ) {
		cout << "Failed to render text: " << TTF_GetError() << endl;
	}
	
	SDL_Texture* text_texture;

	text_texture = SDL_CreateTextureFromSurface( renderer, text );

	SDL_Rect dest = {x,y, text->w, text->h };

	SDL_RenderCopy( renderer, text_texture, NULL,&dest );
}


void createMissile(){
	Missile m1;
	
	m1.changeX(enemyVector[0].getPosX();)
	
	missileSpriteVector.push_back(m1);
}

Uint32 addEnemy(Uint32, void*){
	enemy enemyBuffer;
	enemyVector.push_back(enemyBuffer);
	return 10000;
}

void createEnemy(){
	enemy enemyBuffer;
	enemyVector.push_back(enemyBuffer);
}



