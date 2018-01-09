#include "dalttest.h"
#include "tft.h"
#include "img.h"
#include "stdio.h"

int d_no, d_on;
struct DaltTestResults d_res;
char d_imgLetters[] = "ACCJEIM";


void daltEnd(){
	clearTFT(0xFFFF);
	string2TFT(10,30,2,0,"Results");
	char buffer [50];
	sprintf (buffer, "Protanopia %d/6", d_res.protanopia);
	string2TFT(10,200,1,0,buffer);
	sprintf (buffer, "Tritanopia  %d/1", d_res.tritanopia);
	string2TFT(10,280,1,0,buffer);
	d_on = 0;
}

void daltNewTest(){
	resizeImg(0,80,dalts[d_no],4);
	
	d_no++;
}

void daltTestInit(){
	d_no = 0;
	d_on = 1;
	d_res.protanopia = 0;
	d_res.tritanopia = 0;
	daltNewTest();
}

int daltTestOn(){
	return d_on;
}
