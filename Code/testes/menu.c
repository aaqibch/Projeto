#include "menu.h"
#include "tft.h"
#include "touch.h"
#include "accuitytest.h"
#include "colortest.h"
#include "dalttest.h"

void mainMenu(){
	clearTFT(0xFFFF);
	
	buttonTFT(32,100," Accuitity");
	buttonTFT(32,200,"Daltonism");
	buttonTFT(32,300,"Color Test");
}

void menuClicked(uint16_t x, uint16_t y){
	if(colorTestOn()){
		colorTestClicked(x,y);
	}
	else if(daltTestOn()){
		daltNewTest();
	}
	else if(accuityTestOn()){
		accuityTestNew();
	}
	else{
		mainMenu();
		if(inBox(32,100,240,60,x,y))
			accuityTestInit();
		else if (inBox(32,200,240,60,x,y))
			daltTestInit();
		else if (inBox(32,300,240,60,x,y))
			colorTestInit();
	}
}
