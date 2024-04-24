#include "bsp.h"
#include "stm32g4xx_hal_tim.h"

TIM_HandleTypeDef htim3 = {0};
TIM_HandleTypeDef htim5 = {0};

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
    htim3.Init.Prescaler         = 17; /* 170MHz / 17 = 10MHz */
    htim3.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim3.Init.Period            = 10000; /* 1KHz to read the encoder */
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
    sConfigOC.Pulse      = (5000 - 1); /* 50% Duty Ratio */
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

    if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK) {
        Error_Handler();
    }
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    HAL_TIM_MspPostInit(&htim3);
}

void MX_TIM5_Init(void)
{
    TIM_ClockConfigTypeDef  sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig      = {0};

    htim5.Instance               = TIM5;
    htim5.Init.Prescaler         = 17;
    htim5.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim5.Init.Period            = 1000;
    htim5.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&htim5) != HAL_OK) {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK) {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK) {
        Error_Handler();
    }

    HAL_TIM_Base_Start_IT(&htim5);
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

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{
    if (tim_baseHandle->Instance == TIM5) {
        /* TIM5 clock enable */
        __HAL_RCC_TIM5_CLK_ENABLE();

        /* TIM5 interrupt Init */
        HAL_NVIC_SetPriority(TIM5_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM5_IRQn);
    }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{
    if (tim_baseHandle->Instance == TIM5) {
        /* Peripheral clock disable */
        __HAL_RCC_TIM5_CLK_DISABLE();

        /* TIM5 interrupt Deinit */
        HAL_NVIC_DisableIRQ(TIM5_IRQn);
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

uint32_t count_100us = 0;
uint32_t count_1ms   = 0;
void     HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    if (htim->Instance == TIM5) {
        count_100us += 1;
    } else if (htim->Instance == TIM3) {
    }
}