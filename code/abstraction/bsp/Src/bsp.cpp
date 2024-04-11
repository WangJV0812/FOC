#include "bsp.h"
#include "bsp_dma.h"
#include "bsp_gpio.h"
#include "bsp_spi.h"
#include "bsp_tim.h"
#include "stm32g4xx_hal.h"

__weak void Error_Handler(void)
{
    for(;;);
}

/**
 * @brief HAL system init
 * @return HAL_StatusTypeDef 
 * @date 2024-04-10
 * @author wangjv (wangjv0812@126.com)
 */
HAL_StatusTypeDef system_init()
{
    HAL_StatusTypeDef status = HAL_OK;

    status = HAL_Init();
    if (status != HAL_OK) {
        Error_Handler();
    }

    SystemClock_Config();

    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    /** Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral
    */
    HAL_PWREx_DisableUCPDDeadBattery();
    return status;
}

/**
 * @brief init the muc system
 * @date 2024-04-07
 * @author wangjv (wangjv0812@126.com)
 */
void bsp_init()
{
    system_init();
 
    MX_GPIO_Init();
    // MX_DMA_Init();
    // MX_TIM3_Init();
    MX_SPI3_Init();
    MX_SPI4_Init();

    // spi_cs_set(GPIO_PIN_SET);
}