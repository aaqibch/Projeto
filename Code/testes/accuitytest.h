#ifndef __ACCUITY_H__
#define __ACCUITY_H__
#include "stm32f4xx.h"

struct AccuityTestResults{
	int esq, dir;
};

void accuityTestInit(void);
void accuityTestNew(void);
int accuityTestOn(void);

#endif
