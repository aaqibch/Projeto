#include "record.h"
#include "pdm_filter.h"
#include "stm32f4_discovery.h"

#define INTERNAL_BUFF_SIZE      64

#define VOLUME 50

#define Buffer_Mic_size 8000

volatile static unsigned short int local_buffer0[64];
volatile static unsigned short int local_buffer1[64];
unsigned short int buffer0_m[16];
unsigned short int buffer1_m[16];

float word_data[Buffer_Mic_size];

uint16_t* pAudioRecBuf;

PDMFilter_InitStruct Filter;

void recordInit(void){

	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);

	RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
	
	Filter.LP_HZ=8000; 
	Filter.HP_HZ=10;
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



	//RCC_PLLI2SCmd(ENABLE);
	//I2S2 config
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	I2S_InitTypeDef I2S_InitStructure;
	
	SPI_I2S_DeInit(SPI2);
	I2S_InitStructure.I2S_AudioFreq=32000;
	I2S_InitStructure.I2S_Standard=I2S_Standard_LSB;
	I2S_InitStructure.I2S_DataFormat=I2S_DataFormat_16b;
	I2S_InitStructure.I2S_CPOL=I2S_CPOL_High;
	I2S_InitStructure.I2S_Mode=I2S_Mode_MasterRx;
	I2S_InitStructure.I2S_MCLKOutput=I2S_MCLKOutput_Disable;
	I2S_Init(SPI2, &I2S_InitStructure);
	//I2S_Cmd(SPI2, ENABLE);
	
	// Profit ????
	RCC_I2SCLKConfig(RCC_I2S2CLKSource_PLLI2S);
	RCC_PLLI2SCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLI2SRDY)==RESET);
	
	//Start
	//SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, ENABLE);
	
	
	/*** DMA ***/
	NVIC_InitTypeDef nvic_init_structure;
	DMA_InitTypeDef dma_init_structure;
  /* Enable the DMA clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    
  /* Configure the DMA Stream */
  DMA_Cmd(DMA1_Stream3, DISABLE);
  DMA_DeInit(DMA1_Stream3);
  /* Set the parameters to be configured */
  dma_init_structure.DMA_Channel = DMA_Channel_0;  
  dma_init_structure.DMA_PeripheralBaseAddr = 0x4000380C;
  dma_init_structure.DMA_Memory0BaseAddr = (unsigned int)local_buffer0;
  dma_init_structure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  dma_init_structure.DMA_BufferSize = 64;
  dma_init_structure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  dma_init_structure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  dma_init_structure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  dma_init_structure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  //dma_init_structure.DMA_Mode = DMA_Mode_Normal;
	dma_init_structure.DMA_Mode = DMA_Mode_Circular;
  dma_init_structure.DMA_Priority = DMA_Priority_VeryHigh;
  dma_init_structure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	dma_init_structure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
  dma_init_structure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  dma_init_structure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream3, &dma_init_structure);
	DMA_DoubleBufferModeConfig(DMA1_Stream3, (unsigned int)local_buffer1, DMA_Memory_0);
	DMA_DoubleBufferModeCmd(DMA1_Stream3, ENABLE);
  /*Enable the selected DMA interrupts */
  DMA_ITConfig(DMA1_Stream3, DMA_IT_TC, ENABLE);
  /* I2S DMA IRQ Channel configuration */
  nvic_init_structure.NVIC_IRQChannel = DMA1_Stream3_IRQn;
  nvic_init_structure.NVIC_IRQChannelPreemptionPriority = 14;
  //nvic_init_structure.NVIC_IRQChannelSubPriority = 0;
  nvic_init_structure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic_init_structure);

  /* Enable the I2S DMA request */
  SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Rx, ENABLE);
	
}

void recordStart(void){

	DMA_Cmd(DMA1_Stream3, ENABLE);
	I2S_Cmd(SPI2, ENABLE); 
	//SPI_I2S_ReceiveData(SPI2);
}

void recordStop(void){

    /* Stop conversion */
    I2S_Cmd(SPI2, DISABLE); 

}

void MicStop(void){
	/* Stop the Transfer on the I2S side: Stop and disable the DMA stream */
  DMA_Cmd(DMA1_Stream3, DISABLE);
  /* Clear all the DMA flags for the next transfer */
  DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3 | DMA_FLAG_HTIF3 | DMA_FLAG_FEIF3 | DMA_FLAG_TEIF3 | DMA_FLAG_DMEIF3);
  
  /*  
           The I2S DMA requests are not disabled here.
                                                            */
  
  /* In all modes, disable the I2S peripheral */
  I2S_Cmd(SPI2, DISABLE);
}

void MicBufferSwap(short int* buffer){
	static int j = 0;
	unsigned int i;
	
	for(i = 0; i < 16; i=i+2)
		word_data[j++] = buffer[i] / 32768.0;
	
	if(j == Buffer_Mic_size){
		MicStop();
		j = 0;
	}
}


void DMA1_Stream3_IRQHandler(void){
	unsigned int i;
	/* Transfer complete interrupt */
  if(DMA_GetFlagStatus(DMA1_Stream3, DMA_FLAG_TCIF3) != RESET){
		DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3);
		if(DMA_GetCurrentMemoryTarget(DMA1_Stream3)){

			for(i = 0; i < 64; i++)
				local_buffer0[i] = HTONS(local_buffer0[i]);
			PDM_Filter_64_LSB((uint8_t *)local_buffer0, (uint16_t *)buffer0_m, 32, &Filter);

			MicBufferSwap((short int*)buffer0_m);
		}
		else{
			for(i = 0; i < 64; i++)
				local_buffer1[i] = HTONS(local_buffer1[i]);
			PDM_Filter_64_LSB((uint8_t *)local_buffer1, (uint16_t *)buffer1_m, 32, &Filter);

				MicBufferSwap((short int*)buffer1_m);
		}
	}
}
