#ifndef __TFT_H__
#define __TFT_H__
#include "stm32f4xx.h"

#define LCD_PORT GPIOC
#define LCD_PERIPH RCC_AHB1Periph_GPIOC
#define LCD_WR  GPIO_Pin_4 // ? data write		c4
#define LCD_RS  GPIO_Pin_5 // ? data-or-command ? low if command, DC/X			c5
#define LCD_CS  GPIO_Pin_6 // chip select, low enable

#define DBI_PORT GPIOE
#define DBI_PERIPH RCC_AHB1Periph_GPIOE
#define DBI_PINS GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7


void initTFT(void);
void rectFillTFT(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t c);
void clearTFT(uint16_t c);
void letterTFT(uint16_t x, uint16_t y, uint16_t s, uint16_t c, char letter);
void stringTFT(uint16_t x, uint16_t y, uint16_t s, uint16_t c, char str[]);
void buttonTFT(uint16_t x, uint16_t y, char str[]);

uint16_t RGBto565(uint8_t r, uint8_t g, uint8_t b);

void loadImg(uint16_t x, uint16_t y);

#endif
