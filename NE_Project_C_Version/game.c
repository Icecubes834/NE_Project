#include "game.h"
#include <stdio.h>

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

	enemyBuffer->rect.w = 50;
    enemyBuffer->rect.h = 50;

    int randXPos = (rand() % WIDTH) - enemyBuffer->rect.w;
    if(randXPos < 0) randXPos = 0;
    enemyBuffer->rect.x = randXPos;
    enemyBuffer->rect.y = -100;

	enemyBuffer->defeated = false;
	enemyBuffer->hexText[0]= assignHexText();
	enemyBuffer->hexText[1]= assignHexText();
	enemyBuffer->hexText[2]= '\0';

	SDL_Surface* textSurface;

    textSurface = TTF_RenderText_Solid( font,enemyBuffer->hexText , BLACK );

    if ( !textSurface ) {
        printf("Failed to render text: %s\n",TTF_GetError());
    }

    enemyBuffer->text_texture = SDL_CreateTextureFromSurface( renderer,textSurface );

    enemyBuffer->dest.w = textSurface->w;
    enemyBuffer->dest.h = textSurface->h;

	SDL_FreeSurface(textSurface);
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

