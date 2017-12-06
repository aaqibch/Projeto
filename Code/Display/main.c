#include "stm32f4xx.h"
#include "tft.h"
#include "touch.h"


int main(){
	int val=0;
	struct Touch t;
	
	initTFT();
	clearTFT(0xFFFF);
	buttonTFT(40,50,"Teste 1",7);
	buttonTFT(40,150,"Teste 2",7);
	buttonTFT(40,250,"Teste 3",7);

	
	
	adcTouch();
	getTouchZ();
	while(1){
		val = getTouchZ();
		if(val<0x320){
			
			getTouchXY(&t);
			if(inBox(40,50,240,60,t.x,t.y))
				loadImg(0,50);
			else if(inBox(40,150,240,60,t.x,t.y))
				buttonTFT(40,150,"  OK   ",7);
			else if(inBox(40,250,240,60,t.x,t.y))
				buttonTFT(40,250,"  OK   ",7);
			else{
				clearTFT(0xFFFF);
				buttonTFT(40,50,"Teste 1",7);
				buttonTFT(40,150,"Teste 2",7);
				buttonTFT(40,250,"Teste 3",7);
				}
			
		}
	}
}
