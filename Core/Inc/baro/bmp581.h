/*
 * baro.h
 *
 *  Created on: Apr 1, 2026
 *      Author: shpar
 */

#ifndef INC_BARO_H_
#define INC_BARO_H_

#include "bmp/bmp581.h"

extern BMP581 bmp581;

bool init_baro(SPI_HandleTypeDef *hspi, GPIO_TypeDef* cs_gpio_port, uint16_t cs_pin);

#endif /* INC_BARO_H_ */
