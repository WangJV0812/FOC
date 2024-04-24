#include "main.h"
#include "bsp.h"
#include "bsp_tim.h"
#include "stm32_hal_legacy.h"
#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_gpio.h"

uint16_t receive_data = 0;
int      main()
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
    }
    return 0;
}