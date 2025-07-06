#include <iostream>
#include <SDL.h>
#include <unistd.h>
#include <vector>
#include <time.h>
#include <SDL_ttf.h>

using namespace std;

bool init();
void kill();
bool loop();
void changeState(int padNumber);
char assignIntToHex(int inputNumber);
void drawText(const char* inputText,int x, int y);
bool checkIfBinaryNumberEqualsHex(int inputBinaryNumber[8],char inputHexData[2]);
bool check4BitEqualsHex(int inputBinaryNumber[4],char inputHexData);
void createMissile();
Uint32 addEnemy(Uint32,void*);
void createEnemy();


// Pointers to our window and renderer
SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* font;

//Defining the width and height of the screen
const int WIDTH = 640;
const int HEIGHT = 480;

int binaryNumber[8] = {0,0,0,0,0,0,0,0};

class enemy{
	public:
		//Constructor
		enemy();
		//A function to draw the enemy
		void draw();
		
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
	private:
		SDL_Rect padRect;
		
		int state = 0;
	
		//The pad number it represents
		int padNumber;
	
	
	public:
		//Constructor with starting pos x and pad number as its parameters
		BinaryPad(int startingX,int inputPadNumber);
		//Function to draw the pad
		void draw();
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
		
	
	
};

struct Missile{
	SDL_Rect missileRect;
	void draw(){
		SDL_SetRenderDrawColor( renderer, 255, 255, 0, 255 );
		missileRect.y = missileRect.y - 5;
		SDL_RenderFillRect(renderer,&missileRect);
	}
	
	SDL_Rect* getRect(){
		return &missileRect;
	}
};

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
						changeState(0);
						break;
					case SDLK_2:
						changeState(1);
						break;
					case SDLK_3: 
						changeState(2);
						break;
					case SDLK_4:
						changeState(3);
						break;
					case SDLK_5: 
						changeState(4);
						break;
					case SDLK_6:
						changeState(5);
						break;
					case SDLK_7: 
						changeState(6);
						break;
					case SDLK_8:
						changeState(7);
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
		enemyVector.at(i).draw();
	}
	
	for (uint8_t i = 0; i < missileSpriteVector.size(); i++) {
		missileSpriteVector.at(i).draw();
	}
	
	//SDL_SetRenderDrawColor( renderer, 0,0, 255, 255 );
	
	for (int i = 0; i < 8; i++){
		binaryPadVector.at(i).draw();
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


enemy::enemy(){
	//Defining the width and height of the enemy and starting position
	
	defeated = false;
	
	enemyRect.w = 50;
	enemyRect.h = 50;
	
	//Generate a random number between 0 and WIDTH - its width
	int randomStartXPosition =  rand() % (WIDTH-enemyRect.w);
	
	enemyRect.x = randomStartXPosition;
	enemyRect.y = -enemyRect.h - 25;
	
	int firstRandomHexDigit = rand()%16;
	int secondRandomHexDigit = rand()%16;
	
	hexData[0] = assignIntToHex(firstRandomHexDigit);
	hexData[1] = assignIntToHex(secondRandomHexDigit);
	
	for (int i = 0; i < 2; i++){
		cout << hexData[i];
	} 
	
	cout << endl;
}

void enemy::draw(){
	enemyRect.y += 1;
	SDL_SetRenderDrawColor( renderer,0, 255, 0, 100 );
	SDL_RenderFillRect(renderer,&enemyRect);
	
	drawText(hexData,enemyRect.x,enemyRect.y);
}






BinaryPad::BinaryPad(int startingX,int inputPadNumber){
	//Constructor
	padRect.w = 50;
	padRect.h = 50;
	
	padRect.y = HEIGHT - (padRect.h + 20);
	padRect.x = startingX;
	
	padNumber = inputPadNumber;
	
}

void BinaryPad::draw(){
	if (state == 1){
		SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );
	}else{
		SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
	}
	SDL_RenderFillRect(renderer,&padRect);
	
	
	if (state == 0){
		drawText("0",padRect.x,padRect.y);
	}else{
		drawText("1",padRect.x,padRect.y);
	}
}

void BinaryPad::changeX(int x){
	padRect.x = x;
}




void changeState(int padNumber){
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

void drawText(const char* inputText, int x, int y){
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
	
	m1.missileRect.w = 20;
	m1.missileRect.h = 50;
	m1.missileRect.x = enemyVector[0].getPosX();
	m1.missileRect.y = 350;
	
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


