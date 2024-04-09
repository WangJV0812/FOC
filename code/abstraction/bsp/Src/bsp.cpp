#include "bsp.h"
#include "bsp_dma.h"
#include "bsp_spi.h"
#include "bsp_tim.h"
#include "stm32g4xx_hal.h"

__weak void Error_Handler(void)
{
    return ;
}

/**
 * @brief init the muc system
 * @date 2024-04-07
 * @author wangjv (wangjv0812@126.com)
 */
void bsp_init ()
{
    if(HAL_Init() !=  HAL_OK) {
        Error_Handler();
    }
    SystemClock_Config();

    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    /** Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral
    */
    HAL_PWREx_DisableUCPDDeadBattery();
    
    MX_DMA_Init();
    MX_TIM3_Init();
    MX_SPI3_Init();
    MX_SPI4_Init();
}