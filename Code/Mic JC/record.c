#include "record.h"

		
float word_data[Buffer_Mic_size];

volatile static unsigned short int local_buffer0[64];
volatile static unsigned short int local_buffer1[64];
unsigned short int buffer0_m[16];
unsigned short int buffer1_m[16];


void Start_Mic(unsigned int rate)
{
	//Delay(10000);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);

	Filter.LP_HZ = rate;
	Filter.HP_HZ = 10;
	Filter.Fs = rate*2;
	Filter.Out_MicChannels = 1;
	Filter.In_MicChannels = 1;
	
	PDM_Filter_Init(&Filter);
	
	Mic_GPIO_Init();
	
	Init_SPI(8000);
	Init_DMA();
	Init_NVIC();
	
	Mic_Record();
	
	//Delay(10000);

}

void Mic_Record()
{
  /* Enable the I2S DMA Stream*/
  DMA_Cmd(DMA1_Stream3, ENABLE);
	
	/* If the I2S peripheral is still not enabled, enable it */
  if ((SPI2->I2SCFGR & 0x0400) == 0)
	{
    I2S_Cmd(SPI2, ENABLE);
	}
}

void Mic_Stop()
{
	/* Stop the Transfer on the I2S side: Stop and disable the DMA stream */
  DMA_Cmd(DMA1_Stream3, DISABLE);

  I2S_Cmd(SPI2, DISABLE);
}


void Mic_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(SPI_SCK_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(SPI_MOSI_CLK, ENABLE);
	
	/* SPI SCK pin config   */
	GPIO_InitStructure.GPIO_Pin = SPI_SCK_PIN;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed =  GPIO_High_Speed;
	
	GPIO_Init(SPI_SCK_GPIO, &GPIO_InitStructure);
	GPIO_PinAFConfig(SPI_SCK_GPIO, SPI_SCK_SOURCE, SPI_SCK_AF);

	/* SPI MOSI pin config  */
	GPIO_InitStructure.GPIO_Pin =  SPI_MOSI_PIN;
	
  GPIO_Init(SPI_MOSI_GPIO, &GPIO_InitStructure);
  GPIO_PinAFConfig(SPI_MOSI_GPIO, SPI_MOSI_SOURCE, SPI_MOSI_AF);	
}

void Init_DMA()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    
  /* Configure the DMA Stream */
  DMA_Cmd(DMA1_Stream3, DISABLE);
  DMA_DeInit(DMA1_Stream3);
  /* Set the parameters to be configured */
  DMA_InitStructure.DMA_Channel = DMA_Channel_0;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = 0x4000380C;
  DMA_InitStructure.DMA_Memory0BaseAddr = (unsigned int)local_buffer0;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = 64;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  //DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream3, &DMA_InitStructure);
	
	DMA_DoubleBufferModeConfig(DMA1_Stream3, (unsigned int)local_buffer1, DMA_Memory_0);
	DMA_DoubleBufferModeCmd(DMA1_Stream3, ENABLE);
	
	DMA_ITConfig(DMA1_Stream3, DMA_IT_TC, ENABLE);

  /*Enable the selected DMA interrupts */
}

void DMA1_Stream3_IRQHandler(void)
{
	unsigned int i;
	/* Transfer complete interrupt */
  if(DMA_GetFlagStatus(DMA1_Stream3, DMA_FLAG_TCIF3) != RESET)
	{
		DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3);
		
		if(DMA_GetCurrentMemoryTarget(DMA1_Stream3))
		{
			for(i = 0; i < 64; i++)
				local_buffer0[i] = HTONS(local_buffer0[i]);
			
			PDM_Filter_64_LSB((uint8_t *)local_buffer0, (uint16_t *)buffer0_m, 32, &Filter);

			check_buffer((short int*)buffer0_m);
		}
		else
		{
			for(i = 0; i < 64; i++)
				local_buffer1[i] = HTONS(local_buffer1[i]);
			
			PDM_Filter_64_LSB((uint8_t *)local_buffer1, (uint16_t *)buffer1_m, 32, &Filter);

			check_buffer((short int*)buffer1_m);
		}
	}
	
}

void check_buffer(short int* buffer)
{
	static int j=0;
	unsigned int i;
	
	for(i = 0; i < 16; i=i+2)
	{
		word_data[j++] = buffer[i] / 32768.0;
	}
		
	if(j == Buffer_Mic_size)
	{
		Mic_Stop();
		j = 0;
	}
}

void Init_SPI(unsigned int rate)
{
	I2S_InitTypeDef I2S_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	
	SPI_I2S_DeInit(SPI2);
	
	I2S_InitStructure.I2S_AudioFreq = rate*4;
	I2S_InitStructure.I2S_Standard = I2S_Standard_LSB;
  I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
  I2S_InitStructure.I2S_CPOL = I2S_CPOL_High;
  I2S_InitStructure.I2S_Mode = I2S_Mode_MasterRx;
  I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
	
  /* Initialize the I2S peripheral with the structure above */
  I2S_Init(SPI2, &I2S_InitStructure);
	
	RCC_I2SCLKConfig(RCC_I2S2CLKSource_PLLI2S);
	RCC_PLLI2SCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLI2SRDY)==RESET);
	
}

void Init_NVIC()
{
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
  /* Enable the I2S DMA request */
  SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Rx, ENABLE);
}
