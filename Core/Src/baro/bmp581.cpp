/*
 * baro.cpp
 *
 *  Created on: Apr 1, 2026
 *      Author: shpar
 */

#include "baro/bmp581.h"
#include "utils/debug_printf.h"
BMP581 bmp581;

bool init_baro(SPI_HandleTypeDef *hspi, GPIO_TypeDef* cs_gpio_port, uint16_t cs_pin){
	while(bmp581.beginSPI(hspi, cs_gpio_port, cs_pin) != BMP5_OK){
		DEBUG_PRINT("failed to initialize bmp, retrying..\r\n");
	}
	while(bmp581.setODRFrequency(BMP5_ODR_120_HZ) != BMP5_OK);
	DEBUG_PRINT("bmp initialized!\r\n");
	return true;
}
