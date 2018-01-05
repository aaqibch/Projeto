#include "stm32f4xx.h"                  // Device header
#include "record.h"
#include "stm32f4_discovery.h"

int main(void){

	STM_EVAL_LEDInit(LED4);
	
	RCC_PLLI2SCmd(ENABLE);
	mp45dt02_init();
	mp45dt02_start();
	
	while(1){
		
	}
	
}

