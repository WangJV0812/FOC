#include "bsp.h"
#include "stm32g4xx_hal_spi.h"

SPI_HandleTypeDef hspi3;
SPI_HandleTypeDef hspi4;
DMA_HandleTypeDef hdma_spi4_rx;

uint16_t SPI3_Rx_Buffer = 0;
uint16_t SPI4_Rx_Buffer = 0;

/**
 * @brief 
 * @date 2024-04-08
 * @author wangjv (wangjv0812@126.com)
 */
void MX_SPI3_Init(void)
{
    hspi3.Instance               = SPI3;
    hspi3.Init.Mode              = SPI_MODE_MASTER;
    hspi3.Init.Direction         = SPI_DIRECTION_2LINES;
    hspi3.Init.DataSize          = SPI_DATASIZE_16BIT;
    hspi3.Init.CLKPolarity       = SPI_POLARITY_HIGH;
    hspi3.Init.CLKPhase          = SPI_PHASE_2EDGE;
    hspi3.Init.NSS               = SPI_NSS_SOFT;
    hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
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

        __HAL_RCC_GPIOC_CLK_ENABLE();
        /**SPI3 GPIO Configuration
        PC10     ------> SPI3_SCK
        PC11     ------> SPI3_MISO
        PC12     ------> SPI3_MOSI
        */

        GPIO_InitStruct.Pin       = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    } else if (spiHandle->Instance == SPI4) {
        /* SPI4 clock enable */
        __HAL_RCC_SPI4_CLK_ENABLE();

        __HAL_RCC_GPIOE_CLK_ENABLE();
        /**SPI4 GPIO Configuration
        PE2     ------> SPI4_SCK
        PE5     ------> SPI4_MISO
        PE6     ------> SPI4_MOSI
        */
        GPIO_InitStruct.Pin       = GPIO_PIN_2 | GPIO_PIN_5 | GPIO_PIN_6;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI4;
        HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

        /* SPI4 DMA Init */
        /* SPI4_RX Init */
        hdma_spi4_rx.Instance                 = DMA1_Channel1;        /* dma channel 1 */
        hdma_spi4_rx.Init.Request             = DMA_REQUEST_SPI4_RX;  /* spi rx request */
        hdma_spi4_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY; /* periph to memory */
        hdma_spi4_rx.Init.PeriphInc           = DMA_PINC_DISABLE;     /* */
        hdma_spi4_rx.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_spi4_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_spi4_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
        hdma_spi4_rx.Init.Mode                = DMA_CIRCULAR;
        hdma_spi4_rx.Init.Priority            = DMA_PRIORITY_HIGH;
        if (HAL_DMA_Init(&hdma_spi4_rx) != HAL_OK) {
            Error_Handler();
        }

        __HAL_LINKDMA(spiHandle, hdmarx, hdma_spi4_rx);

        /* SPI4 interrupt Init */
        HAL_NVIC_SetPriority(SPI4_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(SPI4_IRQn);
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
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4);

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
        HAL_GPIO_DeInit(GPIOE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5 | GPIO_PIN_6);

        /* SPI4 DMA DeInit */
        HAL_DMA_DeInit(spiHandle->hdmarx);

        /* SPI4 interrupt Deinit */
        HAL_NVIC_DisableIRQ(SPI4_IRQn);
    }
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef* hspi)
{
    if (hspi->Instance == SPI3) {
        HAL_SPI_Receive_DMA(hspi, (uint8_t*)&SPI3_Rx_Buffer, 2);
    } else if (hspi->Instance == SPI4) {
        HAL_SPI_Receive_DMA(hspi, (uint8_t*)&SPI4_Rx_Buffer, 2);
    }
}