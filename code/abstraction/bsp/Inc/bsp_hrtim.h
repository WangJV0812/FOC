#ifndef __BSP_HRTIM_H
#define __BSP_HRTIM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_hrtim.h"


extern HRTIM_HandleTypeDef hhrtim1;

void MX_HRTIM1_Init(void);

void HAL_HRTIM_MspPostInit(HRTIM_HandleTypeDef *hhrtim);



void hrtim_timerA_output_enable();
void hrtim_timerB_output_enable();
void hrtim_timerE_output_enable();

void hrtim_timerA_output_disable();
void hrtim_timerB_output_disable();
void hrtim_timerE_output_disable();
#ifdef __cplusplus
}
#endif

#endif