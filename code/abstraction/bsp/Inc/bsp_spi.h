#ifndef __BSP_SPI_H
#define __BSP_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_spi.h"


extern SPI_HandleTypeDef hspi3;
extern SPI_HandleTypeDef hspi4;
extern DMA_HandleTypeDef hdma_spi4_rx;

extern uint16_t SPI3_Rx_Buffer;
extern uint16_t SPI4_Rx_Buffer;

void MX_SPI3_Init(void);
void MX_SPI4_Init(void);


#ifdef __cplusplus
}
#endif

#endif