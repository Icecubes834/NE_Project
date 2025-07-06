#include <iostream>
#include <SDL.h>
#include <ctime>
#include <unistd.h>
#include <vector>
#include <time.h>

using namespace std;

bool init();
void kill();
bool loop();
void changeState(int padNumber);

// Pointers to our window and renderer
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Rect r;

//Defining the width and height of the screen
const int WIDTH = 640;
const int HEIGHT = 480;
const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;



class LTimer
{
    public:
		//  Initializes variables
		LTimer();

		//  The various clock actions
		void start();
		void stop();
		void pause();
		void unpause();

		//  Gets the timer's time
		Uint32 getTicks();

		//  Checks the status of the timer
		bool isStarted();
		bool isPaused();

    private:
		//  The clock time when the timer started
		Uint32 mStartTicks;

		//  The ticks stored when the timer was paused
		Uint32 mPausedTicks;

		//  The timer status
		bool mPaused;
		bool mStarted;
};

class enemy{
	public:
		//Constructor
		enemy();
		//A function to draw the enemy
		void draw();
		
	private:
		//Defines the shape of the enemy
		SDL_Rect enemyRect;
		//Shows the hex value it represents in a string format
		string hexData;
		//The pad number it represents
		
};


class BinaryPad{
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
	
	private:
		SDL_Rect padRect;
		int state = 0;
		//The pad number it represents
		int padNumber;
	
};


BinaryPad p1(0,0);
BinaryPad p2(0,1);
BinaryPad p3(0,2);
BinaryPad p4(0,3);

BinaryPad p5(0,4);
BinaryPad p6(0,5);
BinaryPad p7(0,6);
BinaryPad p8(0,7);


vector<enemy> enemyVector;
vector<BinaryPad> binaryPadVector;



enemy e1;


int main(int argc, char** args) {

	if ( !init() ) return 1;
	LTimer fpsTimer;
	LTimer capTimer;
	
	
	
	int countedFrames = 0;
	fpsTimer.start();

	while ( loop() ) {
		// wait before processing the next frame
		capTimer.start();
		
		float avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );
		if ( avgFPS > 2000000 ){
			avgFPS = 0;
		}
		
		//cout <<"FPS: " <<avgFPS<<"\r";
		
		
		int frameTicks = capTimer.getTicks();
        if  ( frameTicks < SCREEN_TICK_PER_FRAME ){
            // Wait remaining time
			SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
        }
		
		++countedFrames;
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
						enemy enemyBuffer;
						enemyVector.push_back(enemyBuffer);
						break;
					
				
				}
				break;
		}
	}

	// Set drawing color to Red
	SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );

	// Test key states - this could also be done with events
	if ( keys[SDL_SCANCODE_2] ) r.x+=2;
	
	if ( keys[SDL_SCANCODE_D] ) r.x += 2;
	
	if ( keys[SDL_SCANCODE_W] ) r.y -= 2;
	
	if ( keys[SDL_SCANCODE_S] ) r.y += 2;
	
	
	
	for (uint8_t i = 0; i < enemyVector.size(); i++) {
		enemyVector.at(i).draw();
	}
	
	
	//SDL_SetRenderDrawColor( renderer, 0,0, 255, 255 );
	
	for(int i = 0; i < 8; i++){
		binaryPadVector.at(i).draw();
	}
	
	// Update window
	SDL_RenderPresent( renderer );
	
	return true;
}





bool init() {
	r.w = 50;
	r.h = 50;
	r.x = 0;
	r.y = 0;
	
	enemyVector.push_back(e1);
	
	
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

	

	SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
	SDL_RenderClear( renderer );
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
	enemyRect.w = 50;
	enemyRect.h = 50;
	
	//Setting a seed based on time (for the random number generation)
	srand(time(0));
	
	//Generate a random number
	int randomStartXPosition = 140 + (rand() % WIDTH -140);
	
	cout <<"Random Number generated: "<< randomStartXPosition << endl;
	
	enemyRect.x = randomStartXPosition;
	enemyRect.y = 0;
}

void enemy::draw(){
	enemyRect.y += 1;
	
	SDL_RenderFillRect(renderer,&enemyRect);
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
}

void BinaryPad::changeX(int x){
	padRect.x = x;
}




void changeState(int padNumber){
	for (int i = 0;i < 8;i++){
		if (padNumber==binaryPadVector.at(i).getPadNumber()){
			if (binaryPadVector.at(i).getState() == 0){
				binaryPadVector.at(i).setState(1);
			}else{
				binaryPadVector.at(i).setState(0);
			}
			cout << "Successfully changed state: " << binaryPadVector.at(i).getState() <<endl;
		}
	}
}









LTimer::LTimer()
{
    //  Initialize the variables
    mStartTicks = 0;
    mPausedTicks= 0;

    mPaused = false;
    mStarted= false;
}

void LTimer::start()
{
    //  Start the timer
    mStarted= true;

    //  Unpause the timer
    mPaused = false;

    //  Get the current clock time
    mStartTicks = SDL_GetTicks();
	mPausedTicks= 0;
}

void LTimer::stop()
{
    //  Stop the timer
    mStarted= false;

    //Unpause the timer
    mPaused = false;

	//  Clear tick variables
	mStartTicks = 0;
	mPausedTicks= 0;
}

void LTimer::pause()
{
    //  If the timer is running and isn't already paused
    if  ( mStarted && !mPaused )
    {
        //  Pause the timer
        mPaused = true;

        //  Calculate the paused ticks
        mPausedTicks= SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
    }
}

void LTimer::unpause()
{
    //  If the timer is running and paused
    if  ( mStarted && mPaused )
    {
        //  Unpause the timer
        mPaused = false;

        //  Reset the starting ticks
        mStartTicks = SDL_GetTicks() - mPausedTicks;

        //  Reset the paused ticks
        mPausedTicks = 0;
    }
}

Uint32 LTimer::getTicks()
{
	//  The actual timer time
	Uint32 time = 0;

    //  If the timer is running
    if  ( mStarted )
    {
        //  If the timer is paused
        if  ( mPaused )
        {
            //  Return the number of ticks when the timer was paused
            time = mPausedTicks;
        }
        else
        {
            //  Return the current time minus the start time
            time = SDL_GetTicks() - mStartTicks;
        }
    }

    return time;
}

bool LTimer::isStarted()
{
	//  Timer is running and paused or unpaused
    return mStarted;
}

bool LTimer::isPaused()
{
	//  Timer is running and paused
    return mPaused && mStarted;
}
