#ifndef __TFT_H__
#define __TFT_H__
#include "stm32f4xx.h"

#define LCD_PORT GPIOC
#define LCD_PERIPH RCC_AHB1Periph_GPIOC
#define LCD_WR  GPIO_Pin_7 // ? data write
#define LCD_RS  GPIO_Pin_8 // ? data-or-command ? low if command, DC/X
#define LCD_CS  GPIO_Pin_9 // chip select, low enable
#define LCD_RST GPIO_Pin_5 // reset


void initTFT(void);

#endif
