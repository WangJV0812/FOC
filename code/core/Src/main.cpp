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
    hrtim_timerA_output_enable();
    hrtim_timerB_output_enable();
    hrtim_timerE_output_enable();


    while (1) {
        if (count_100us > 0) {
            receive_data = SPI4_Receive_DMA(0);
            count_100us  = 0;
        }
        if (count_1ms > 9) {
            count_1ms = 0;
            adc_conversion = ADC1_Channel_conversion();
            ADC2_Channel_Conversion(adc2_conversion_sequence);
        }
    }
    return 0;
}