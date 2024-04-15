#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_gpio.h"

#define LED1_Pin       GPIO_PIN_3
#define LED1_GPIO_Port GPIOC
#define LED2_Pin       GPIO_PIN_2
#define LED2_GPIO_Port GPIOF

void MX_GPIO_Init(void);
void led1_set(GPIO_PinState state);
void led2_set(GPIO_PinState state);

#ifdef __cplusplus
}
#endif

#endif