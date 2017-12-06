#include "record.h"
#include "pdm_filter.h"
#include "stm32f4_discovery.h"

#define INTERNAL_BUFF_SIZE      64

#define VOLUME 50

static uint8_t InternalBuffer[INTERNAL_BUFF_SIZE];
static uint32_t InternalBufferSize = 0;

uint16_t* pAudioRecBuf;

PDMFilter_InitStruct Filter;

void recordInit(void){

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);

	Filter.LP_HZ=8000; 
	Filter.HP_HZ=20;
	Filter.Fs=16000;
	Filter.Out_MicChannels=1;
	Filter.In_MicChannels=1;
	PDM_Filter_Init(&Filter);

	//MP45DT02 CLK-PB10
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_SPI2);

	//MP45DT02 DOUT-PC3
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_SPI2);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	RCC_PLLI2SCmd(ENABLE);
	//I2S2 config
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	I2S_InitTypeDef I2S_InitStructure;
	I2S_InitStructure.I2S_AudioFreq=32000;
	I2S_InitStructure.I2S_Standard=I2S_Standard_LSB;
	I2S_InitStructure.I2S_DataFormat=I2S_DataFormat_16b;
	I2S_InitStructure.I2S_CPOL=I2S_CPOL_High;
	I2S_InitStructure.I2S_Mode=I2S_Mode_MasterRx;
	I2S_InitStructure.I2S_MCLKOutput=I2S_MCLKOutput_Disable;
	I2S_Init(SPI2, &I2S_InitStructure);
	I2S_Cmd(SPI2, ENABLE);

	//Start
	SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, ENABLE);
}

void recordStart(void){

	/* Store the location and size of the audio buffer 
	pAudioRecBuf = pbuf;
	AudioRecCurrSize = size;*/
	
	/* Enable the Rx buffer not empty interrupt */
	SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, ENABLE);
	/* The Data transfer is performed in the SPI interrupt routine */
	/* Enable the SPI peripheral */
	I2S_Cmd(SPI2, ENABLE); 
	//SPI_I2S_ReceiveData(SPI2);
}

void recordStop(void){

    /* Stop conversion */
    I2S_Cmd(SPI2, DISABLE); 

}

void SPI2_IRQHandler(void){
   u16 app;
  /* Check if data are available in SPI Data register */
  if (SPI_GetITStatus(SPI2, SPI_I2S_IT_RXNE) != RESET){
		
    app = SPI_I2S_ReceiveData(SPI2);
    InternalBuffer[InternalBufferSize++] = HTONS(app);	//swap bytes

		if (app>0)
			STM_EVAL_LEDOn(LED4);
		
    /* Check to prevent overflow condition */
		if (InternalBufferSize >= INTERNAL_BUFF_SIZE){
      InternalBufferSize = 0;
      
      PDM_Filter_64_LSB((uint8_t *)InternalBuffer, (uint16_t *)pAudioRecBuf, VOLUME, (PDMFilter_InitStruct *)&Filter);     
    }
  }
}
