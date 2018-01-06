#include "stm32f4xx.h"
#include "tft.h"
#include "touch.h"
#include "imgletters.h"


int main(){
	//int val=0;
	//struct Touch t;
	
	initTFT();
	clearTFT(0xFFFF);
	
	
	loadMonoImg(0,0,0,0xFFFF,letters[3]);
	letter2TFT(0,0,4,0x001F,'X');
	letter2TFT(0,50,3,0x1F00,'X');
	letter2TFT(0,50,2,0xF000,'X');
	letter2TFT(0,50,1,0x0F0F,'X');
	
	/*
	adcTouch();
	while(1){
		if(isTouch()){
			getTouchXY(&t);
			//rectFillTFT(t.x,t.y,15,15,0x1F);
			colorTestClicked(t.x, t.y);
			int i, j;
			for(i=0; i<65000; i++)
				for(j=0; j<65; j++);
		}
	}*/
}
