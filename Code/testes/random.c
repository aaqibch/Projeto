#include "random.h"
#include "stdlib.h"
void randomInit(){
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);
	RNG_Cmd(ENABLE);
	while(RNG_GetFlagStatus(RNG_FLAG_DRDY) == RESET);
	srand(RNG_GetRandomNumber());
}

/*
uint8_t random(uint8_t no){
	while(RNG_GetFlagStatus(RNG_FLAG_DRDY) == RESET);
	uint8_t rnd = (uint8_t) RNG_GetRandomNumber();
	rnd *= no;
	rnd /= 255;
	return rnd;
}*/

int random(int no){
	int rnd = rand() % no;
	return rnd;
}
