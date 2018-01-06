#ifndef __MIC_H_
#define __MIC_H_

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


#define Buffer_Mic_size 8000	//1s
#define VOLUME 64

static PDMFilter_InitStruct Filter;
static DMA_InitTypeDef DMA_InitStructure;


void micInit(void);
void micStart(void);
void micStop(void);
void check_buffer(short int*);

#endif
