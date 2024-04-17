#include "bsp.h"

CORDIC_HandleTypeDef hcordic;

/* CORDIC init function */
void MX_CORDIC_Init(void)
{
    hcordic.Instance = CORDIC;
    if (HAL_CORDIC_Init(&hcordic) != HAL_OK) {
        Error_Handler();
    }
}

void HAL_CORDIC_MspInit(CORDIC_HandleTypeDef* cordicHandle)
{
    if (cordicHandle->Instance == CORDIC) {
        /* CORDIC clock enable */
        __HAL_RCC_CORDIC_CLK_ENABLE();
    }
}

void HAL_CORDIC_MspDeInit(CORDIC_HandleTypeDef* cordicHandle)
{
    if (cordicHandle->Instance == CORDIC) {
        /* Peripheral clock disable */
        __HAL_RCC_CORDIC_CLK_DISABLE();
    }
}