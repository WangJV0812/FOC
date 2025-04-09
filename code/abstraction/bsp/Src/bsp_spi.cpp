#include "bsp.h"


SPI_HandleTypeDef hspi3;
SPI_HandleTypeDef hspi4;
DMA_HandleTypeDef hdma_spi3_rx;
DMA_HandleTypeDef hdma_spi3_tx;
DMA_HandleTypeDef hdma_spi4_rx;
DMA_HandleTypeDef hdma_spi4_tx;

uint16_t SPI3_Rx_Buffer;
uint16_t SPI3_Tx_Buffer;
uint16_t SPI4_Rx_Buffer;
uint16_t SPI4_Tx_Buffer;

/**
 * @brief 
 * @date 2024-04-08
 * @author wangjv (wangjv0812@126.com)
 */
void MX_SPI3_Init(void)
{
    hspi3.Instance               = SPI3;
    hspi3.Init.Mode              = SPI_MODE_MASTER;
    hspi3.Init.Direction         = SPI_DIRECTION_2LINES;    /* Full Duplex Mode */
    hspi3.Init.DataSize          = SPI_DATASIZE_16BIT;
    hspi3.Init.CLKPolarity       = SPI_POLARITY_HIGH;
    hspi3.Init.CLKPhase          = SPI_PHASE_2EDGE;
    hspi3.Init.NSS               = SPI_NSS_SOFT;
    hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8; /* Baud Rate: 21.25 MBits/s */
    hspi3.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    hspi3.Init.TIMode            = SPI_TIMODE_DISABLE;
    hspi3.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    hspi3.Init.CRCPolynomial     = 7;
    hspi3.Init.CRCLength         = SPI_CRC_LENGTH_DATASIZE;
    hspi3.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;

    if (HAL_SPI_Init(&hspi3) != HAL_OK) {
        Error_Handler();
    }
}

/**
 * @brief 
 * @date 2024-04-08
 * @author wangjv (wangjv0812@126.com)
 */
void MX_SPI4_Init(void)
{
    hspi4.Instance               = SPI4;
    hspi4.Init.Mode              = SPI_MODE_MASTER;
    hspi4.Init.Direction         = SPI_DIRECTION_2LINES;
    hspi4.Init.DataSize          = SPI_DATASIZE_16BIT;
    hspi4.Init.CLKPolarity       = SPI_POLARITY_HIGH;
    hspi4.Init.CLKPhase          = SPI_PHASE_2EDGE;
    hspi4.Init.NSS               = SPI_NSS_SOFT;
    hspi4.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    hspi4.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    hspi4.Init.TIMode            = SPI_TIMODE_DISABLE;
    hspi4.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    hspi4.Init.CRCPolynomial     = 7;
    hspi4.Init.CRCLength         = SPI_CRC_LENGTH_DATASIZE;
    hspi4.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;

    if (HAL_SPI_Init(&hspi4) != HAL_OK) {
        Error_Handler();
    }
}

/**
 * @brief 
 * @param  spiHandle        My Param doc
 * @date 2024-04-08
 * @author wangjv (wangjv0812@126.com)
 */
void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (spiHandle->Instance == SPI3) {
        /* SPI3 clock enable */
        __HAL_RCC_SPI3_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_DMA1_CLK_ENABLE();
        __HAL_RCC_DMAMUX1_CLK_ENABLE();

        /**SPI3 GPIO Configuration
            PA4     ------> SPI3_NSS
            PC10     ------> SPI3_SCK
            PC11     ------> SPI3_MISO
            PC12     ------> SPI3_MOSI
            */
        GPIO_InitStruct.Pin       = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        /* SPI3 DMA Init */
        /* SPI3_RX Init */
        hdma_spi3_rx.Instance                 = DMA1_Channel3;
        hdma_spi3_rx.Init.Request             = DMA_REQUEST_SPI3_RX;
        hdma_spi3_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
        hdma_spi3_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_spi3_rx.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_spi3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_spi3_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
        hdma_spi3_rx.Init.Mode                = DMA_NORMAL;
        hdma_spi3_rx.Init.Priority            = DMA_PRIORITY_HIGH;
        /* reset dma*/
        if (HAL_DMA_DeInit(&hdma_spi3_rx) != HAL_OK) {
            Error_Handler();
        }
        if (HAL_DMA_Init(&hdma_spi3_rx) != HAL_OK) {
            Error_Handler();
        }

        __HAL_LINKDMA(spiHandle, hdmarx, hdma_spi3_rx);
        /* DMA1_Channel1_IRQn interrupt configuration */
        HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

        /* SPI3_TX Init */
        hdma_spi3_tx.Instance                 = DMA1_Channel4;
        hdma_spi3_tx.Init.Request             = DMA_REQUEST_SPI3_TX;
        hdma_spi3_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
        hdma_spi3_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_spi3_tx.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_spi3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_spi3_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
        hdma_spi3_tx.Init.Mode                = DMA_NORMAL;
        hdma_spi3_tx.Init.Priority            = DMA_PRIORITY_LOW;
        /* reset dma*/
        if (HAL_DMA_DeInit(&hdma_spi3_tx) != HAL_OK) {
            Error_Handler();
        }
        if (HAL_DMA_Init(&hdma_spi3_tx) != HAL_OK) {
            Error_Handler();
        }

        __HAL_LINKDMA(spiHandle, hdmatx, hdma_spi3_tx);

        /* DMA1_Channel2_IRQn interrupt configuration */
        HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);

        // /* SPI3 interrupt Init */
        // HAL_NVIC_SetPriority(SPI3_IRQn, 0, 0);
        // HAL_NVIC_EnableIRQ(SPI3_IRQn);

    } else if (spiHandle->Instance == SPI4) {
        /* SPI4 clock enable */
        __HAL_RCC_SPI4_CLK_ENABLE();
        __HAL_RCC_GPIOE_CLK_ENABLE();
        __HAL_RCC_DMA1_CLK_ENABLE();
        __HAL_RCC_DMAMUX1_CLK_ENABLE();

        /**SPI4 GPIO Configuration
            PE2     ------> SPI4_SCK
            PE4     ------> SPI4_NSS
            PE5     ------> SPI4_MISO
            PE6     ------> SPI4_MOSI
        */
        GPIO_InitStruct.Pin       = (GPIO_PIN_2 | GPIO_PIN_5 | GPIO_PIN_6);
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI4;
        HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

        GPIO_InitStruct       = {0};
        GPIO_InitStruct.Pin   = GPIO_PIN_3;
        GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull  = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

        HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);

        /* SPI4 DMA Init */
        /* SPI4_RX Init */
        hdma_spi4_rx.Instance                 = DMA1_Channel1;
        hdma_spi4_rx.Init.Request             = DMA_REQUEST_SPI4_RX;
        hdma_spi4_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
        hdma_spi4_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_spi4_rx.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_spi4_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_spi4_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
        hdma_spi4_rx.Init.Mode                = DMA_NORMAL;
        hdma_spi4_rx.Init.Priority            = DMA_PRIORITY_HIGH;
        if (HAL_DMA_DeInit(&hdma_spi4_rx) != HAL_OK) {
            Error_Handler();
        }
        if (HAL_DMA_Init(&hdma_spi4_rx) != HAL_OK) {
            Error_Handler();
        }

        __HAL_LINKDMA(spiHandle, hdmarx, hdma_spi4_rx);
        /* DMA1_Channel3_IRQn interrupt configuration */
        HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);

        /* SPI4_TX Init */
        hdma_spi4_tx.Instance                 = DMA1_Channel2;
        hdma_spi4_tx.Init.Request             = DMA_REQUEST_SPI4_TX;
        hdma_spi4_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
        hdma_spi4_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_spi4_tx.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_spi4_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_spi4_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
        hdma_spi4_tx.Init.Mode                = DMA_NORMAL;
        hdma_spi4_tx.Init.Priority            = DMA_PRIORITY_LOW;
        if (HAL_DMA_DeInit(&hdma_spi4_tx) != HAL_OK) {
            Error_Handler();
        }
        if (HAL_DMA_Init(&hdma_spi4_tx) != HAL_OK) {
            Error_Handler();
        }

        __HAL_LINKDMA(spiHandle, hdmatx, hdma_spi4_tx);

        /* DMA1_Channel4_IRQn interrupt configuration */
        HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);

        // /* SPI4 interrupt Init */
        // HAL_NVIC_SetPriority(SPI4_IRQn, 0, 0);
        // HAL_NVIC_EnableIRQ(SPI4_IRQn);
    }
}

/**
 * @brief 
 * @param  spiHandle        My Param doc
 * @date 2024-04-08
 * @author wangjv (wangjv0812@126.com)
 */
void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{
    if (spiHandle->Instance == SPI3) {
        /* Peripheral clock disable */
        __HAL_RCC_SPI3_CLK_DISABLE();

        /**SPI3 GPIO Configuration
        PA4     ------> SPI3_NSS
        PC10     ------> SPI3_SCK
        PC11     ------> SPI3_MISO
        PC12     ------> SPI3_MOSI
        */
#if BSP_SPI3_USE_HARD_CSS == 1
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4);
#endif

        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12);
    } else if (spiHandle->Instance == SPI4) {
        /* Peripheral clock disable */
        __HAL_RCC_SPI4_CLK_DISABLE();

        /**SPI4 GPIO Configuration
        PE2     ------> SPI4_SCK
        PE3     ------> SPI4_NSS
        PE5     ------> SPI4_MISO
        PE6     ------> SPI4_MOSI
        */
#if BSP_SPI3_USE_HARD_CSS == 1
        HAL_GPIO_DeInit(GPIOE, GPIO_PIN_3);
#endif
        HAL_GPIO_DeInit(GPIOE, GPIO_PIN_2 | GPIO_PIN_5 | GPIO_PIN_6);

        /* SPI4 DMA DeInit */
        HAL_DMA_DeInit(spiHandle->hdmarx);

        /* SPI4 interrupt Deinit */
        HAL_NVIC_DisableIRQ(SPI4_IRQn);
    }
}

/**
 * @brief 
 * @param  hspi             My Param doc
 * @date 2024-04-10
 * @author wangjv (wangjv0812@126.com)
 */
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef* hspi)
{
    if (hspi->Instance == SPI3) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    } else if (hspi->Instance == SPI4) {
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);
    }
}

/**
 * @brief 
 * @param  hspi             My Param doc
 * @date 2024-04-10
 * @author wangjv (wangjv0812@126.com)
 */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef* hspi)
{
    if (hspi->Instance == SPI3) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    } else if (hspi->Instance == SPI4) {
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);
    }
}

/**
  * @brief  SPI error callbacks.
  * @param  hspi: SPI handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef* hspi) {}

/**
 * @brief 
 * @return HAL_StatusTypeDef 
 * @date 2024-04-10
 * @author wangjv (wangjv0812@126.com)
 */
void SPI4_Receive()
{
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
    if (HAL_SPI_Receive(&hspi4, 
                        (uint8_t*)&SPI4_Rx_Buffer, 
                        2, 
                        100) != HAL_OK) {
        Error_Handler();
    }
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);
}

uint16_t SPI4_Receive_DMA(uint16_t transmit_data)
{
    uint16_t receive_data = 0;
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);

    if (HAL_SPI_TransmitReceive_DMA(&hspi4, 
                                    (uint8_t*)&transmit_data, 
                                    (uint8_t*)&receive_data, 
                                    2) != HAL_OK) {
        Error_Handler();
    }
    return receive_data;
}
