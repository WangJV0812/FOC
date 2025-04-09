#ifndef __ENCODER_H
#define __ENCODER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_spi.h"
#include "bsp_tim.h"
#include "status.h"

enum Encoder_Frequency
{
    Encoder_Frequency_21_25MB = 0,
    Encoder_Frequency_10_625MB,
    Encoder_Frequency_5_3125MB,
    Encoder_Frequency_2_2562MB,
    Encoder_Frequency_1_3281MB,
    Encoder_Frequency_664_kB,
    Encoder_Frequency_Enum_Length
};

class Encoder
{
    public:
        Robot_State state_;
        Encoder(SPI_HandleTypeDef* _spi_handler, Encoder_Frequency _spi_bauderate);
        float angle_read();

    private:
        SPI_HandleTypeDef* spi_handler_;
        uint16_t mechanical_angle_;
        uint16_t zero_angle_;
        uint32_t spi_baudrate_prescaler_;

        float angle_; 
        float read_frequency_;

    protected:


};

#ifdef __cplusplus
}
#endif

#endif
