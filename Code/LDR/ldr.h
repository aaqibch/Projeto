#ifndef __LDR_H__
#define __LDR_H__
#include "stm32f4xx.h"

#define LDR_PORT GPIOC
#define LDR_PIN GPIO_Pin_5
#define LDR_PERIPH RCC_AHB1Periph_GPIOC
#define LDR_ADC ADC1
#define LDR_ADC_PERIPH RCC_APB2Periph_ADC1
#define LDR_ADC_CH ADC_Channel_15

void initLDR(void);
uint16_t getLDR(void);

#endif
