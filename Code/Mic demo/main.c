#include "stm32f4xx.h"                  // Device header
#include "stm32f4_discovery.h"
#include "waverecorder.h"

#define PCM_OUT_SIZE            16
uint16_t RecBuf[PCM_OUT_SIZE], RecBuf1[PCM_OUT_SIZE];

int main(void){

	STM_EVAL_LEDInit(LED4);
	
	WaveRecorderInit(32000,16, 1);
	WaveRecorderStart(RecBuf, PCM_OUT_SIZE);
	
	while(1){
		
	}
	
}

