#ifndef __ENCODER_H
#define __ENCODER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_spi.h"
#include "bsp_tim.h"

class Encoder
{
    Encoder(SPI_HandleTypeDef* spi_handler, uint16_t frequency);
    public:
        

    private:
        SPI_HandleTypeDef* spi_handler_;


};

#ifdef __cplusplus
}
#endif

#endif
