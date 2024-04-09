#include "bsp.h"
#include "stm32g4xx_hal_tim.h"

TIM_HandleTypeDef htim3 = {0};

/**
 * @brief 
 * @date 2024-04-09
 * @author wangjv (wangjv0812@126.com)
 */
void MX_TIM3_Init(void)
{
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef      sConfigOC     = {0};

    htim3.Instance               = TIM3;
    htim3.Init.Prescaler         = 17;          /* 170MHz / 17 = 10MHz */
    htim3.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim3.Init.Period            = (10000 - 1);       /* 1KHz to read the encoder */
    htim3.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    
    if (HAL_TIM_PWM_Init(&htim3) != HAL_OK) {
        Error_Handler();
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;

    if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK) {
        Error_Handler();
    }

    sConfigOC.OCMode     = TIM_OCMODE_PWM1;
    sConfigOC.Pulse      = (5000 - 1);                /* 50% Duty Ratio */
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

    if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK) {
        Error_Handler();
    }
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    HAL_TIM_MspPostInit(&htim3);
}

/**
 * @brief 
 * @param  tim_pwmHandle    My Param doc
 * @date 2024-04-09
 * @author wangjv (wangjv0812@126.com)
 */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* tim_pwmHandle)
{
    if (tim_pwmHandle->Instance == TIM3) {
        /* TIM3 clock enable */
        __HAL_RCC_TIM3_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM3_IRQn);
    }
}

/**
 * @brief 
 * @param  timHandle        My Param doc
 * @date 2024-04-09
 * @author wangjv (wangjv0812@126.com)
 */
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (timHandle->Instance == TIM3) {
        __HAL_RCC_GPIOE_CLK_ENABLE();
        /**TIM3 GPIO Configuration
            PE3     ------> TIM3_CH2
        */
        GPIO_InitStruct.Pin       = GPIO_PIN_3;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
        HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
    }
}

/**
 * @brief 
 * @param  tim_pwmHandle    My Param doc
 * @date 2024-04-09
 * @author wangjv (wangjv0812@126.com)
 */
void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* tim_pwmHandle)
{
    if (tim_pwmHandle->Instance == TIM3) {
        __HAL_RCC_TIM3_CLK_DISABLE();
    }
}

