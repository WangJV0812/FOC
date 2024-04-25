#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_adc.h"


extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;

void MX_ADC1_Init(void);
void MX_ADC2_Init(void);
void ADC_calibration_start(void);

float ADC1_Channel_conversion(void);
void ADC2_Channel_Conversion(float _conversion_sequence_p[]);



#ifdef __cplusplus
}
#endif

#endif