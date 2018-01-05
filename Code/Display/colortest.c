#include "colortest.h"

#define BOX_SIZE 70
#define BOX_NO 6

struct Box{
	uint16_t x;
	uint16_t y;
	uint8_t color;
};

struct Box bxs[BOX_NO];

int selected = -1;
uint8_t r = 0;
uint8_t g = 0;

int random(int no){
	while(RNG_GetFlagStatus(RNG_FLAG_DRDY) == RESET);
	uint32_t rnd = RNG_GetRandomNumber();
	return rnd * no / 0xFFFFFFFF;
}

void shuf() {
	int ctr = BOX_NO, index;
	uint8_t temp;
	
// While there are elements in the array
	while (ctr > 0) {
// Pick a random index
			index = random(ctr);
// Decrease ctr by 1
			ctr--;
// And swap the last element with it
			temp = bxs[ctr].color;
			bxs[ctr].color = bxs[index].color;
			bxs[index].color = temp;
	}
}

void createBoxes(){
	int i;
	r = random(255);
	g = 0;//random(255);
	int wind = 100;//random(100);
	
	for(i=0; i<BOX_NO; i++){
		bxs[i].color =	i * 25 + wind;
		bxs[i].x = 110;
		bxs[i].y = i*(BOX_SIZE+5) + 10;
	}
	
	//shuf();
	
	//for(i=0; i<BOX_NO; i++)
	//	bxs[i].y = i*(BOX_SIZE+5) + 10;
}

void drawBoxes(){
	int i;
	for(i=0; i<BOX_NO; i++){
		rectFillTFT(bxs[i].x, bxs[i].y, BOX_SIZE, BOX_SIZE, RGBto565(r,g,bxs[i].color));
	}
}

int checkOrder(){
	int i;
	uint8_t temp_color = 0;
	for(i=0; i<BOX_NO; i++){
		
		if(bxs[i].color < temp_color)
			return 0;
		temp_color = bxs[i].color;
	}
	clearTFT(0xF);
	return 1;
}


void colorTestInit() {
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);
	RNG_Cmd(ENABLE);
	while(RNG_GetFlagStatus(RNG_FLAG_DRDY) == RESET);
	RNG_GetRandomNumber();
	
	clearTFT(0xFFFF);
	
	createBoxes();
	drawBoxes();
}

int colorTestClicked(uint16_t x, uint16_t y){
	int i;
	uint8_t temp_color;
	
	for(i =0; i<BOX_NO; i++){
		if(x>110 && x<100+BOX_SIZE){
			if(y>bxs[i].y && y<bxs[i].y+BOX_SIZE){
				
				if(selected != -1){
					temp_color = bxs[i].color;
					bxs[i].color = bxs[selected].color;
					bxs[selected].color = temp_color;
					drawBoxes();

					rectFillTFT(bxs[selected].x - 20,bxs[selected].y + 20, 10, 10, 0xFFFF);
					rectFillTFT(bxs[selected].x + BOX_SIZE + 20,bxs[selected].y + 20, 10, 10, 0xFFFF);
					
					selected = -1;
				}
				
				else{
					selected = i;
					rectFillTFT(bxs[i].x - 20,bxs[i].y + 20, 10, 10, 0xF000);
					rectFillTFT(bxs[i].x + BOX_SIZE + 20,bxs[i].y + 20, 10, 10, 0xF000);
				}
				
				if(checkOrder())
					return 1;
			}
		}
	}
	return 0;
}

