#include "stm32f4xx.h"
#include "tft.h"
#include "touch.h"

#include "menu.h"
//#include "accuitytest.h"
//#include "colortest.h"
//#include "dalttest.h"

int main(){
//	int val=0;
	struct Touch t;
	
	initTFT();
	clearTFT(0xFFFF);
	clearTFT(0xFFFF);
	clearTFT(0xFFFF);
	
	/*loadMonoImg(0,0,0,0xFFFF,letters[3]);
	letter2TFT(0,0,4,0x001F,'X');
	letter2TFT(0,50,3,0x1F00,'X');
	letter2TFT(0,50,2,0xF000,'X');
	letter2TFT(0,50,1,0x0F0F,'X');*/
	
	mainMenu();
	//accuityTestInit();
	//daltTestInit();
	//colorTestInit();
	adcTouch();
	while(1){
		if(isTouch()){
			getTouchXY(&t);
			menuClicked(t.x,t.y);
			//rectFillTFT(t.x,t.y,15,15,0x1F);
			int i, j;
			for(i=0; i<65000; i++)
				for(j=0; j<65; j++);
		}
	}
}
