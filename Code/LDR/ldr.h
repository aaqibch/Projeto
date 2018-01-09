#ifndef __LDR_H__
#define __LDR_H__
#include "stm32f4xx.h"

#define LDR_PORT GPIOA
#define LDR_PIN GPIO_Pin_3
#define LDR_PERIPH RCC_AHB1Periph_GPIOA
#define LDR_ADC ADC2
#define LDR_ADC_PERIPH RCC_APB2Periph_ADC2
#define LDR_ADC_CH ADC_Channel_3

void initLDR(void);
uint16_t getLDR(void);

#endif
