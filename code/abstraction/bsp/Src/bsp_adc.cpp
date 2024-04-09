#include "bsp.h"


void bsp_adc_init(ADC_HandleTypeDef* adc_handle)
{
    ADC_MultiModeTypeDef multimode = {0};
    ADC_ChannelConfTypeDef sConfig = {0};

    adc_handle->Instance = ADC1;
    adc_handle->Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    adc_handle->Init.Resolution = ADC_RESOLUTION_12B;
    adc_handle->Init.DataAlign = ADC_DATAALIGN_RIGHT;
    adc_handle->Init.GainCompensation = 0;
    adc_handle->Init.ScanConvMode = ADC_SCAN_DISABLE;
    adc_handle->Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    adc_handle->Init.LowPowerAutoWait = DISABLE;
    adc_handle->Init.ContinuousConvMode = DISABLE;
    adc_handle->Init.NbrOfConversion = 1;
    adc_handle->Init.DiscontinuousConvMode = DISABLE;
    adc_handle->Init.ExternalTrigConv = ADC_SOFTWARE_START;
    adc_handle->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    adc_handle->Init.DMAContinuousRequests = DISABLE;
    adc_handle->Init.Overrun = ADC_OVR_DATA_PRESERVED;
    adc_handle->Init.OversamplingMode = DISABLE;
    if (HAL_ADC_Init(adc_handle) != HAL_OK) {
        Error_Handler();
    }

    /** Configure the ADC multi-mode
    */
    multimode.Mode = ADC_MODE_INDEPENDENT;
    if (HAL_ADCEx_MultiModeConfigChannel(adc_handle, &multimode) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Regular Channel
    */
    sConfig.Channel = ADC_CHANNEL_1;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
    sConfig.SingleDiff = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset = 0;
    if (HAL_ADC_ConfigChannel(adc_handle, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
}
