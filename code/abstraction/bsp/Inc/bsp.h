#ifndef __BSP_H
#define __BSP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32g4xx_hal.h"

__weak void Error_Handler(void);

#include "bsp_tim.h"
#include "bsp_dma.h"
#include "bsp_spi.h"
#include "bsp_rcc.h"
#include "bsp_adc.h"
#include "bsp_hrtim.h"

void bsp_init();

#ifdef __cplusplus
}
#endif

#endif