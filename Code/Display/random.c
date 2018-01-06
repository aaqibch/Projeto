#include "random.h"

void randomInit(){
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);
	RNG_Cmd(ENABLE);
	while(RNG_GetFlagStatus(RNG_FLAG_DRDY) == RESET);
	RNG_GetRandomNumber();	
}


int random(int no){
	while(RNG_GetFlagStatus(RNG_FLAG_DRDY) == RESET);
	uint32_t rnd = RNG_GetRandomNumber();
	return rnd * no / 0xFFFFFFFF;
}
