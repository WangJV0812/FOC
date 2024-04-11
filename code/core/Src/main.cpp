#include "main.h"
#include "bsp.h"
#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_gpio.h"

int main()
{
    bsp_init();
    while (1) {
        HAL_Delay(1);
        SPI4_Receive_DMA();
    }
    return 0;
}