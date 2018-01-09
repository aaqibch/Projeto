#include "Ultrasonic.h"




int distance ;


void delay_nus(int n) 
{ while (n-- != 0) { }}

 void delay_1ms(void)      
 {  int sysTickCounter = 1000;
    while (sysTickCounter != 0) {}
}
void delay_nms(int n) {
    {  delay_1ms();}
}

void Timer2_configuration(int prescaler, int reload)
{

		NVIC_InitTypeDef nvicStructure;
	
		nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
    nvicStructure.NVIC_IRQChannelSubPriority = 1;
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;
		
    NVIC_Init(&nvicStructure);
	
		TIM_TimeBaseInitTypeDef timerInitStructure;
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
		timerInitStructure.TIM_Prescaler = prescaler;
		timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
		timerInitStructure.TIM_Period = reload;
		timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	
		TIM_TimeBaseInit(TIM2, &timerInitStructure);
		TIM_SetCounter(TIM2,0);
		TIM_Cmd(TIM2, ENABLE);
		
		TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}
void Config_UltrasoundTimer()
{
	TIM_TimeBaseInitTypeDef TIM_BaseStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	  
	TIM_BaseStruct.TIM_Prescaler   = 83; // 1 us 
	TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_BaseStruct.TIM_Period = 0xFFFF; // This is max count
	TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_BaseStruct.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM5, &TIM_BaseStruct);

}
void SAMPLE_time(int time, int unit)
{		
		int prescaler;
	
		float multiplier = 1.2;
	
		if(unit == 1)    		//uSEC
				prescaler = 9;
		
		else if (unit == 2)	//mSEC
		{
				prescaler = 10700;
				multiplier = 2.5;
		}
		
		else										//SEC
		{
				prescaler = 10499;;
				multiplier = 2200;
		}
		
		Timer2_configuration(prescaler,time*multiplier);		
}

void InitUS()
{		
		GPIO_InitTypeDef gpioStructure;
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
    gpioStructure.GPIO_Pin = GPIO_Pin_4;
    gpioStructure.GPIO_Mode = GPIO_Mode_OUT;
		gpioStructure.GPIO_OType = GPIO_OType_PP;
    gpioStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
		gpioStructure.GPIO_Speed = GPIO_High_Speed;
    GPIO_Init(GPIOE, &gpioStructure);
	
    gpioStructure.GPIO_Pin = GPIO_Pin_5;
    gpioStructure.GPIO_Mode = GPIO_Mode_IN;
    gpioStructure.GPIO_OType = GPIO_OType_PP;
    gpioStructure.GPIO_PuPd = GPIO_PuPd_UP;
    gpioStructure.GPIO_Speed = GPIO_High_Speed;
		GPIO_Init(GPIOE, &gpioStructure);
	
		/*gpioStructure.GPIO_Pin = GPIO_Pin_5;
    gpioStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_Init(GPIOE, &gpioStructure);*/
		
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource5);

		EXTI_InitStructure.EXTI_Line = EXTI_Line5;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
		//EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
		EXTI_Init(&EXTI_InitStructure);
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
			//TImer 
		Config_UltrasoundTimer();
		SAMPLE_time(100, 2);
		
}

void EXTI9_5_IRQHandler(void) {
		int countT;
		char	buffer[10];
    if (EXTI_GetITStatus(EXTI_Line5) != RESET) {
        	
					if((TIM5->CR1 & TIM_CR1_CEN) == 0){
						TIM_Cmd(TIM5,ENABLE);
						}
					else {
						TIM_Cmd(TIM5,DISABLE);
						countT = TIM_GetCounter(TIM5);
						TIM_SetCounter(TIM5, 0);
						distance = countT/(58);		
						clearTFT(WHITE);
						sprintf(buffer, "%d", distance);
						stringTFT(200, 100, 4, buffer, BLACK,3 );
						
						}
					EXTI_ClearITPendingBit(EXTI_Line5);
				}

			}
void TIM2_IRQHandler()
{	
	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != 0){
				GPIO_SetBits(GPIOE,GPIO_Pin_4);
				delay_nus(20);
				GPIO_ResetBits(GPIOE,GPIO_Pin_4);
				delay_nus(20);
				TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

		}
}
