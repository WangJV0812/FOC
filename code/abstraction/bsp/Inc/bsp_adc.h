#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_adc.h"


extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;

// DMA buffers for ADC data (accessible from main.cpp)
extern uint16_t adc1_dma_buffer[3];  // 3-phase currents (Phase A, B, C)
extern uint16_t adc2_dma_buffer[2];  // Other signals (e.g., Vbus, temperature)

void MX_ADC1_Init(void);
void MX_ADC2_Init(void);
void ADC_calibration_start(void);

// Legacy software polling functions (deprecated - use DMA buffers instead)
float ADC1_Channel_conversion(void);
void ADC2_Channel_Conversion(float _conversion_sequence_p[]);



#ifdef __cplusplus
}
#endif

#endif