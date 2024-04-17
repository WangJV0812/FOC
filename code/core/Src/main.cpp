#include "main.h"
#include "bsp.h"
#include "stm32_hal_legacy.h"
#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_gpio.h"

uint16_t receive_data = 0;
int main()
{
    bsp_init();
    hrtim_timerA_output_enable();
    hrtim_timerB_output_enable();
    hrtim_timerE_output_enable();

    while (1) {
        HAL_Delay(1);
        
        receive_data = SPI4_Receive_DMA(0);
    }
    return 0;
}