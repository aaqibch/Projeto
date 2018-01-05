#include "stm32f4xx.h"
#include "tft.h"
#include "touch.h"
#include "colortest.h"


int main(){
	int val=0;
	struct Touch t;
	
	initTFT();
	clearTFT(0xFFFF);
	adcTouch();
	buttonTFT(50,50,"hello");
	stringTFT(50,150,3,0,"hello");
	
	colorTestInit();
	
	while(1){

		
		
		
		val = getTouchZ();
		if(val<0x265 && val>0x245 ){
			getTouchXY(&t);
			//rectFillTFT(t.x,t.y,15,15,0x1F);
			colorTestClicked(t.x, t.y);
			int i, j;
			for(i=0; i<65000; i++)
				for(j=0; j<65; j++);
		}
	}
}
