#ifndef __MICRO_H_
#define __MICRO_H_
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include "pdm_filter.h"
#include "stm32f4xx.h"
#include "misc.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_dma.h"


#define SPI_SCK_PIN						GPIO_Pin_10
#define SPI_SCK_GPIO					GPIOB
#define SPI_SCK_CLK						RCC_AHB1Periph_GPIOB
#define SPI_SCK_SOURCE				GPIO_PinSource10
#define SPI_SCK_AF						GPIO_AF_SPI2

#define SPI_MOSI_PIN					GPIO_Pin_3
#define SPI_MOSI_GPIO					GPIOC
#define SPI_MOSI_CLK					RCC_AHB1Periph_GPIOC
#define SPI_MOSI_SOURCE				GPIO_PinSource3
#define SPI_MOSI_AF						GPIO_AF_SPI2


#define Buffer_Mic_size 8000

static PDMFilter_InitStruct Filter;
static DMA_InitTypeDef DMA_InitStructure;


//void Delay(volatile unsigned int);
void Start_Mic(unsigned int);
void Mic_Record(void);
void Mic_Stop(void);
void Mic_GPIO_Init(void);
void Init_DMA(void);
void Init_SPI(unsigned int);
void Init_NVIC(void);
void check_buffer(short int*);

#endif
