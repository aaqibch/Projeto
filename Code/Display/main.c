#include "stm32f4xx.h"
#include "tft.h"

int main(){
	initTFT();
	clearTFT(0xFFF);
	while(1);
}
