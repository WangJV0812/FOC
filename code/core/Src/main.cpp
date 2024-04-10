#include "main.h"
#include "bsp.h"
#include "stm32g4xx_hal.h"

int main()
{
    bsp_init();
    while (1) {
        led1_set(GPIO_PIN_SET);
        led2_set(GPIO_PIN_RESET);
        HAL_Delay(1000);
        led1_set(GPIO_PIN_RESET);
        led2_set(GPIO_PIN_SET);
        HAL_Delay(1000);
        SPI4_Receive_DMA();
    }

    return 0;
}