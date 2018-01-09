#include "accuitytest.h"
#include "imgletters.h"
#include "tft.h"
#include "stdio.h"

int a_no;
int a_on;

char a_letters[] = "AXCUCAXUAX";
struct AccuityTestResults a_res;

void accuityTestEnd(){
	clearTFT(0xFFFF);
	string2TFT(10,30,2,0,"Results");
	char buffer [50];
	sprintf (buffer, "Direito %d/10", a_res.dir);
	string2TFT(10,200,1,0,buffer);
	sprintf (buffer, "Esquerdo %d/10", a_res.esq);
	string2TFT(10,280,1,0,buffer);
	a_on = 0;
}

void accuityTestNew(){
	if(a_no == 20)
		accuityTestEnd();
	else{
		switch(a_no%10){
			case 0:
			case 1:
				clearTFT(0xFFFF);
				loadMonoImg(80,120,0,0xFFFF,letters[a_no%10]);
				break;
			case 2:
			case 3:
				clearTFT(0xFFFF);
				letter2TFT(110,140,3,0,a_letters[a_no%10]);
				break;
			case 4:
			case 5:
				clearTFT(0xFFFF);
				letter2TFT(130,175,2,0,a_letters[a_no%10]);
				break;
			case 6:
			case 7:
			case 8:
			case 9:
				clearTFT(0xFFFF);
				letter2TFT(145,210,1,0,a_letters[a_no%10]);
				break;
			default:
				break;
		}
	
		if(a_no<10)
			a_res.dir ++;
		else
			a_res.esq ++;
		
		a_no++;
	}
}

void accuityTestInit(){
	a_on=1;
	a_no=0;
	a_res.dir = 0;
	a_res.esq = 0;
	accuityTestNew();
}

int accuityTestOn(){
	return a_on;
}
