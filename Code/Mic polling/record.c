#include "record.h"
#include "pdm_filter.h"
#include "stm32f4_discovery.h"

#define INTERNAL_BUFF_SIZE      64

#define VOLUME 50

static uint8_t InternalBuffer[INTERNAL_BUFF_SIZE];
static uint32_t InternalBufferSize = 0;

uint16_t* pAudioRecBuf;

PDMFilter_InitStruct Filter;


#define   APB1_FREQ        100000000                           // Clock driving TIM3
#define   CNT_FREQ         200000000                           // TIM3 counter clock (prescaled APB1)
#define   TIM_PRESCALER    50		//(((APB1_FREQ) / (CNT_FREQ))-1)     // APB1 prescaler

void recordInit(void){

	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);

	//MP45DT02 CLK-PB10
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//MP45DT02 DOUT-PC3
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	NVIC_InitTypeDef NVIC_TIM3;                                 // NVIC structure
  TIM_TimeBaseInitTypeDef TIM3_TimeBase;                      // Time base structure
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);        // Clocking TIM3  (APB1 = 42MHz)

  NVIC_TIM3.NVIC_IRQChannel                   = TIM3_IRQn;    // Specifying the channel (stm32f4xx.h)
  NVIC_TIM3.NVIC_IRQChannelPreemptionPriority = 0;            // Only matters for multiple interrupts
  NVIC_TIM3.NVIC_IRQChannelSubPriority        = 0;            // Only matters for multiple interrupts
  NVIC_TIM3.NVIC_IRQChannelCmd                = ENABLE;       // Enabling global interrupt
  NVIC_Init(&NVIC_TIM3);                                      // Initializing NVIC structure
	
	TIM3_TimeBase.TIM_ClockDivision = 0;                        // Not dividing
  TIM3_TimeBase.TIM_CounterMode   = TIM_CounterMode_Up;       // Upcounting configuration
  TIM3_TimeBase.TIM_Period        = 2*14;                    // Autoreload value (ARR) 
  TIM3_TimeBase.TIM_Prescaler     = 50;            // Dividing APB1 by 2
  TIM_TimeBaseInit(TIM3, &TIM3_TimeBase);                     // Initializing Time Base structure
//.000 03125  31u
  TIM_Cmd(TIM3, ENABLE);                                      // Ready, Set, Go!
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);                     // Enabling TIM3 Ch.1 interrupts
}

int a;
void TIM3_IRQHandler(void){
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){

		GPIO_ToggleBits(GPIOB,GPIO_Pin_10);
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3))
			a = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3);
		
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);                  // Clear TIM3 Ch.1 flag
		
  }
}
