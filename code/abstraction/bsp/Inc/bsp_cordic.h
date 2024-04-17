#ifndef __BSP_CORDIC_H
#define __BSP_CORDIC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_cordic.h"

extern CORDIC_HandleTypeDef hcordic;

void MX_CORDIC_Init(void);

#ifdef __cplusplus
}
#endif

#endif