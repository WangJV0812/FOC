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
extern DMA_HandleTypeDef hdma_spi4_tx;
extern DMA_HandleTypeDef hdma_spi3_rx;
extern DMA_HandleTypeDef hdma_spi3_tx;

extern uint16_t SPI3_Rx_Buffer;
extern uint16_t SPI3_Tx_Buffer;
extern uint16_t SPI4_Rx_Buffer;
extern uint16_t SPI4_Tx_Buffer;

void MX_SPI3_Init(void);
void MX_SPI4_Init(void);

void SPI4_Receive();
uint16_t SPI4_Receive_DMA(uint16_t transmit_data);

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef* hspi);
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef* hspi);

#ifdef __cplusplus
}
#endif

#endif