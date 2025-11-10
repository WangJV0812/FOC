#include "main.h"
#include "bsp.h"
#include "bsp_adc.h"
#include "bsp_tim.h"
#include "stm32_hal_legacy.h"
#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_gpio.h"

uint16_t receive_data = 0;
float adc_conversion = 0;
float adc2_conversion_sequence[2] = {0};

int main()
{
    bsp_init();
    
    // Start ADC with DMA (hardware-triggered by HRTIM at PWM period events)
    // ADC1: 3-phase currents (triggered by HRTIM TRG1 at ~50kHz)
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc1_dma_buffer, 3);
    
    // ADC2: Other signals like Vbus, temperature (triggered by HRTIM TRG2 at ~50kHz)
    HAL_ADC_Start_DMA(&hadc2, (uint32_t*)adc2_dma_buffer, 2);
    
    // Enable HRTIM timer outputs (this starts PWM generation and ADC triggering)
    hrtim_timerA_output_enable();
    hrtim_timerB_output_enable();
    hrtim_timerE_output_enable();


    while (1) {
        if (count_100us > 0) {
            receive_data = SPI4_Receive_DMA(0);
            count_100us  = 0;
        }
        
        // Note: ADC sampling is now handled automatically by HRTIM triggers and DMA
        // ADC data is available in adc1_dma_buffer[] and adc2_dma_buffer[]
        // For FOC control loop processing, implement HAL_ADC_ConvCpltCallback()
        if (count_1ms > 9) {
            count_1ms = 0;
            // Old software polling removed - now using hardware-triggered DMA
            // adc_conversion = ADC1_Channel_conversion();
            // ADC2_Channel_Conversion(adc2_conversion_sequence);
            
            // Example: Read phase A current from DMA buffer
            // uint16_t phase_a_raw = adc1_dma_buffer[0];
            // float phase_a_current = (phase_a_raw * 3.3f / 4096.0f - 1.65f) / 0.066f; // Example conversion
        }
    }
    return 0;
}