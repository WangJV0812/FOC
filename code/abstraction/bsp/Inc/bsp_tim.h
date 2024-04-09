#ifndef __BSP_TIM_H
#define __BSP_TIM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_tim.h"

extern TIM_HandleTypeDef htim3;

void MX_TIM3_Init(void);
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* tim_pwmHandle);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle);
void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* tim_pwmHandle);

#ifdef __cplusplus
}
#endif

#endif