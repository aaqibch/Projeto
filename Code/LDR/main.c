#include "stm32f4xx.h"
#include "ldr.h"
uint16_t val;
int main(){
	initLDR();
	
	val = getLDR();
	while(1){
		val = getLDR();
	}
}
