#include "ldr.h"

void initLDR(void){
	GPIO_InitTypeDef GPIO_InitStructure;	
	ADC_InitTypeDef ADC_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);
	
	/* Configure NVIC */
  NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB2PeriphClockCmd(LDR_ADC_PERIPH, ENABLE);
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //??
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(LDR_ADC, &ADC_InitStructure);

  ADC_Cmd(LDR_ADC, ENABLE);
	
	
	//PORT
	RCC_AHB1PeriphClockCmd(LDR_PERIPH, ENABLE);
	//PIN
	GPIO_InitStructure.GPIO_Pin = LDR_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LDR_PORT, &GPIO_InitStructure);
	
	ADC_RegularChannelConfig(LDR_ADC, LDR_ADC_CH, 1, ADC_SampleTime_480Cycles);
	
	ADC_Cmd(LDR_ADC, ENABLE);
}

uint16_t getLDR(){
	ADC_SoftwareStartConv(LDR_ADC);
	while( ADC_GetFlagStatus(LDR_ADC,ADC_FLAG_EOC) == RESET);
	return ADC_GetConversionValue(LDR_ADC);
}

