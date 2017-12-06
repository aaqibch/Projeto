#include "touch.h"

#define LEFT 0X62
#define RIGHT 0x38F
#define TOP 0X4C
#define BOT 0X395
#define W 320
#define H 480

#define SET_YP(); 	GPIO_WriteBit(YP_PORT, YP_PIN, Bit_SET);//TOUCH_PORT->BSRRL = TOUCH_YP;
#define SET_YM(); 	GPIO_WriteBit(YM_PORT, YM_PIN, Bit_SET);//TOUCH_PORT->BSRRL = TOUCH_YM;
#define RESET_YP(); GPIO_WriteBit(YP_PORT, YP_PIN, Bit_RESET);//TOUCH_PORT->BSRRH = TOUCH_YP;
#define RESET_YM(); GPIO_WriteBit(YM_PORT, YM_PIN, Bit_RESET);//TOUCH_PORT->BSRRH = TOUCH_YM;
#define SET_XP(); 	GPIO_WriteBit(XP_PORT, XP_PIN, Bit_SET);//TOUCH_PORT->BSRRL = TOUCH_XP;
#define SET_XM(); 	GPIO_WriteBit(XM_PORT, XM_PIN, Bit_SET);//TOUCH_PORT->BSRRL = TOUCH_XM;
#define RESET_XP(); GPIO_WriteBit(XP_PORT, XP_PIN, Bit_RESET);//TOUCH_PORT->BSRRH = TOUCH_XP;
#define RESET_XM(); GPIO_WriteBit(XM_PORT, XM_PIN, Bit_RESET);//TOUCH_PORT->BSRRH = TOUCH_XM;


#define SET_CS(); GPIO_WriteBit(CS_PORT, CS_PIN, Bit_SET);//TOUCH_PORT->BSRRH = TOUCH_XP;
#define RESET_CS(); GPIO_WriteBit(CS_PORT, CS_PIN, Bit_RESET);//TOUCH_PORT->BSRRH = TOUCH_XM;

#define GET_YP();
#define GET_XM();

void adcTouch(){	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);
	
	
	RCC_APB2PeriphClockCmd(TOUCH_ADC_PERIPH, ENABLE);
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_10b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //??
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(TOUCH_ADC, &ADC_InitStructure);
	
	ADC_RegularChannelConfig(TOUCH_ADC, TOUCH_YP_CH, 1, ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(TOUCH_ADC, TOUCH_XM_CH, 1, ADC_SampleTime_480Cycles);
	
	ADC_Cmd(TOUCH_ADC, ENABLE);
	
}

void pinMode(GPIO_TypeDef* port, uint32_t pin, GPIOMode_TypeDef mode){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = pin;
	GPIO_InitStructure.GPIO_Mode = mode;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(port, &GPIO_InitStructure);
}

void defaultPins(){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(YP_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_Init(YM_PORT, &GPIO_InitStructure);
}

uint16_t anRead(uint8_t ch){
	ADC_RegularChannelConfig(TOUCH_ADC, ch, 1, ADC_SampleTime_480Cycles);
	ADC_SoftwareStartConv(TOUCH_ADC);
	while( ADC_GetFlagStatus(TOUCH_ADC,ADC_FLAG_EOC) == RESET);
	return ADC_GetConversionValue(TOUCH_ADC);
}

void getTouchXY(struct Touch *t){
	SET_CS();
	
	//YP ANALOG
	// ym input
	//xp xm output
	pinMode(YP_PORT,YP_PIN,GPIO_Mode_AN);
	pinMode(YM_PORT,YM_PIN,GPIO_Mode_IN);
	pinMode(XP_PORT,XP_PIN,GPIO_Mode_OUT);
	pinMode(XM_PORT,XM_PIN,GPIO_Mode_OUT);

  SET_XP();
	RESET_XM();

  int x = anRead(TOUCH_YP_CH) + anRead(TOUCH_YP_CH);
  x /= 2;
  x = 1023 - x;
	
	//XM ANALOG
	//xp input
	//yp ym output
	pinMode(XM_PORT,XM_PIN,GPIO_Mode_AN);
	pinMode(XP_PORT,XP_PIN,GPIO_Mode_IN);
	pinMode(YP_PORT,YP_PIN,GPIO_Mode_OUT);
	pinMode(YM_PORT,YM_PIN,GPIO_Mode_OUT);
	
	RESET_YM();
	SET_YP();

  int y = anRead(TOUCH_XM_CH) + anRead(TOUCH_XM_CH);
  y /= 2;
  y = 1023 - y;
	
	//0-left
	//320-right
	
	t->x = (x-LEFT)*W/RIGHT;
	t->y = (y-TOP)*H/BOT;
	
	RESET_CS();
	defaultPins();
}



int getTouchZ(){
	SET_CS();
  // Set X+ to ground
  //pinMode(XP,OUTPUT)S
	pinMode(XP_PORT,XP_PIN,GPIO_Mode_OUT);
  RESET_XP();
  
  // Set Y- to VCC
  //pinMode(YM,OUTPUT);
	pinMode(YM_PORT,YM_PIN,GPIO_Mode_OUT);
  SET_YM();
	
  // Hi-Z X- and Y+
  RESET_XM();
	//xm analog
	pinMode(XM_PORT,XM_PIN,GPIO_Mode_AN);
	RESET_YP();
  //yp analog
	pinMode(YP_PORT,YP_PIN,GPIO_Mode_AN);
  
  int z1 = anRead(TOUCH_XM_CH); 
  int z2 = anRead(TOUCH_YP_CH);

	RESET_CS();
	defaultPins();
	return z2-z1;
}

uint8_t isTouch(){
  return getTouchZ()>200;
}

uint8_t inBox(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t tx, uint16_t ty){
	if(tx>x && tx<x+w && ty>y && ty<y+h)
		return 1;
	return 0;
}

