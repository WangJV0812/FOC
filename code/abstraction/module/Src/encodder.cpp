#include "bsp_spi.h"
#include "module.h"
#include "status.h"
#include "stm32g4xx_hal_spi.h"

/**
 * @brief Construct a new Encoder:: Encoder object
 * 
 * @param _spi_handler Encoder communicate bus handler
 * @param _spi_bauderate spi 
 */
Encoder::Encoder(SPI_HandleTypeDef* _spi_handler, 
                 Encoder_Frequency _spi_bauderate = Encoder_Frequency_21_25MB)
{
	if (_spi_handler == &hspi3) {
		this->spi_handler_ = _spi_handler;
		MX_SPI3_Init();
	} else if (_spi_handler == &hspi4) {
		this->spi_handler_ = _spi_handler;
		MX_SPI4_Init();
	} else {
		this->state_ = Robot_State::offline;
		return;
	}

    switch (_spi_bauderate) {
        case Encoder_Frequency::Encoder_Frequency_21_25MB: 
                spi_baudrate_prescaler_ = SPI_BAUDRATEPRESCALER_8; break;
        case Encoder_Frequency::Encoder_Frequency_10_625MB: 
                spi_baudrate_prescaler_ = SPI_BAUDRATEPRESCALER_16; break;
        case Encoder_Frequency::Encoder_Frequency_5_3125MB: 
                spi_baudrate_prescaler_ = SPI_BAUDRATEPRESCALER_32; break;
        case Encoder_Frequency::Encoder_Frequency_2_2562MB: 
                spi_baudrate_prescaler_ = SPI_BAUDRATEPRESCALER_64; break;
        case Encoder_Frequency::Encoder_Frequency_1_3281MB: 
                spi_baudrate_prescaler_ = SPI_BAUDRATEPRESCALER_128; break;
        case Encoder_Frequency::Encoder_Frequency_664_kB: 
                spi_baudrate_prescaler_ = SPI_BAUDRATEPRESCALER_256; break;
        default: 
                spi_baudrate_prescaler_ = SPI_BAUDRATEPRESCALER_8; break;
    }
    /* set spi baud rate*/
    this->spi_handler_->Init.BaudRatePrescaler = spi_baudrate_prescaler_;
    
    this->zero_angle_ = 0;
    this->state_ = Robot_State::normal;
}

