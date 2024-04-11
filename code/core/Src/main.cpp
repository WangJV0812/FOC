#include "main.h"
#include "bsp.h"
#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_gpio.h"

int main()
{
    bsp_init();
    while (1) {
        spi_cs_set(GPIO_PIN_SET);
        HAL_Delay(10);
        spi_cs_set(GPIO_PIN_RESET);
        SPI4_Receive_DMA();
    }
    return 0;
}