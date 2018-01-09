#include "colortest.h"
#include "random.h"
#include "stdio.h"
#define BOX_SIZE 70
#define BOX_NO 6

struct Box{
	uint16_t x;
	uint16_t y;
	uint8_t color;
};

struct Box bxs[BOX_NO];

int selected = -1;
uint8_t red = 0;
uint8_t green = 0;
uint8_t blue = 0;
int no = 0;
int moves = 0;
int c_on = 0;

struct ColorTestResults res;

void shuf() {
	int ctr = BOX_NO, index;
	uint8_t temp;
	
	while (ctr > 0) {
			index = random(ctr);
			ctr--;
			temp = bxs[ctr].color;
			bxs[ctr].color = bxs[index].color;
			bxs[index].color = temp;
	}
}

void createBoxes(){
	int i;
	int wind = random(70) + 5;
	//red = random(15);
	//green = random(15);
	
	for(i=0; i<BOX_NO; i++){
		bxs[i].color =	i * 25 + wind;
		bxs[i].x = 110;
		bxs[i].y = i*(BOX_SIZE+5) + 10;
	}
	
	shuf();
	
	//for(i=0; i<BOX_NO; i++)
	//	bxs[i].y = i*(BOX_SIZE+5) + 10;
}

void drawBoxes(){
	int i;
	for(i=0; i<BOX_NO; i++){
		rectFillTFT(bxs[i].x, bxs[i].y, BOX_SIZE, BOX_SIZE, RGBto565(red*bxs[i].color,green*bxs[i].color,blue*bxs[i].color));
	}
}

void colorTestEnd(){
	clearTFT(0xFFFF);
	string2TFT(10,30,2,0,"Results");
	char buffer [50];
	sprintf (buffer, "RED = %d", res.r);
	string2TFT(50,200,1,0,buffer);
	sprintf (buffer, "GREEN = %d", res.g);
	string2TFT(50,250,1,0,buffer);
	sprintf (buffer, "BLUE = %d", res.b);
	string2TFT(50,300,1,0,buffer);
	c_on = 0;
}


void colorTestNew(){
	moves = 0;
	red = 0;
	green = 0;
	blue = 0;
	switch (no){
		case 0:
		case 4:
			green = 1;
			break;
		case 1:
		case 3:
			red = 1;
			break;
		case 2:
		case 5:
			blue = 1;
			break;
		case 6:
			colorTestEnd();
			return;
	}
	no++;
	clearTFT(0xFFFF);
	createBoxes();
	drawBoxes();
}

void colorTestInit() {
	randomInit();
	res.r = 0;
	res.g = 0;
	res.b = 0;
	no = 0;
	moves = 0;
	c_on = 1;
	colorTestNew();
}

int checkOrder(){
	int i;
	uint8_t temp_color = 0;
	for(i=0; i<BOX_NO; i++){
		
		if(bxs[i].color < temp_color)
			return 0;
		temp_color = bxs[i].color;
	}
	
	res.r += red * moves;
	res.g += green * moves;
	res.b += blue * moves;
	clearTFT(0x2C0);
	clearTFT(0x2C0);
	colorTestNew();
	return 1;
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
					moves++;
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

int colorTestOn(){
	
	return c_on;
}
