#ifndef __TFT_H__
#define __TFT_H__
#include "stm32f4xx.h"

#define LCD_PORT GPIOC
#define LCD_PERIPH RCC_AHB1Periph_GPIOC
#define LCD_WR  GPIO_Pin_7 // ? data write
#define LCD_RS  GPIO_Pin_8 // ? data-or-command ? low if command, DC/X
#define LCD_CS  GPIO_Pin_9 // chip select, low enable
#define LCD_RST GPIO_Pin_6 // reset

#define DBI_PORT GPIOD
#define DBI_PERIPH RCC_AHB1Periph_GPIOD
#define DBI_PINS GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7


void initTFT(void);
void rectFillTFT(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t c);
void clearTFT(uint16_t c);

void minimal(void);

#endif
