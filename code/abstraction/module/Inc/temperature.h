#ifndef __TEMPERATURE_H
#define __TEMPERATURE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_adc.h"


class temperature
{
public:
    float value;
    temperature(ADC_HandleTypeDef * _adc_handle_p, uint32_t channel);

private:
    ADC_HandleTypeDef * adc_handle_p_;


};


#ifdef __cplusplus
}
#endif
#endif