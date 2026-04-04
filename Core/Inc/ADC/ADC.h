/*
 * ADC.h
 *
 *  Created on: Apr 1, 2026
 *      Author: shpar
 */

#ifndef INC_ADC_ADC_H_
#define INC_ADC_ADC_H_

#include "stm32l4xx_hal.h"

#define DATABOARD_ADC_HANDLE hadc1
#define PYROBOARD_ADC_HANDLE hadc2

// ADC로부터 전압값을 읽는 함수
uint32_t analogRead(ADC_HandleTypeDef *hadc);

float convert_adc_to_voltate(uint32_t adc_value);

#endif /* INC_ADC_ADC_H_ */
