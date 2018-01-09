#ifndef __DALTTEST_H__
#define __DALTTEST_H__

struct DaltTestResults{
	int protanopia, tritanopia;
};

void daltTestInit(void);
int daltTestOn(void);
void daltNewTest(void);

#endif
