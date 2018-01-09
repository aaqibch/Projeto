#ifndef __COLORTEST_H__
#define __COLORTEST_H__
#include "stm32f4xx.h"
#include "tft.h"

struct ColorTestResults{
	int r,g,b;
};

void colorTestInit(void);
int colorTestClicked(uint16_t x, uint16_t y);
int colorTestOn(void);

#endif
