#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_adc.h"


void bsp_adc_init(ADC_HandleTypeDef* adc_handle);



#ifdef __cplusplus
}
#endif

#endif