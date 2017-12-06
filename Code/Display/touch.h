#ifndef __TOUCH_H__
#define __TOUCH_H__
#include "stm32f4xx.h"

#define TOUCH_PORT GPIOC
#define TOUCH_PERIPH RCC_AHB1Periph_GPIOC
#define TOUCH_YP  GPIO_Pin_11		//WR
#define TOUCH_XM  GPIO_Pin_11		//RS
#define TOUCH_YM  GPIO_Pin_11		//6
#define TOUCH_XP  GPIO_Pin_11		//7

#define YP_PIN  GPIO_Pin_4		//WR
#define XM_PIN  GPIO_Pin_5		//RS
#define YM_PIN  GPIO_Pin_13		//6
#define XP_PIN  GPIO_Pin_14
#define CS_PIN  GPIO_Pin_6

#define YP_PORT GPIOC		//WR
#define XM_PORT GPIOC		//RS
#define YM_PORT GPIOE		//6
#define XP_PORT GPIOE	
#define CS_PORT GPIOC

#define TOUCH_ADC ADC2
#define TOUCH_ADC_PERIPH RCC_APB2Periph_ADC2
#define TOUCH_YP_CH ADC_Channel_14
#define TOUCH_XM_CH ADC_Channel_15

struct Touch{
	int x;
	int y;
};

void adcTouch(void);
int getTouchZ(void);
void getTouchXY(struct Touch *t);
	
uint8_t inBox(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t tx, uint16_t ty);

#endif
