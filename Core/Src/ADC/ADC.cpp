/*
 * ADC.cpp
 *
 *  Created on: Apr 1, 2026
 *      Author: shpar
 */

#include "ADC/ADC.h"

uint32_t analogRead(ADC_HandleTypeDef *hadc) {
	uint32_t ADCvalue = 0;
	HAL_ADC_Start(hadc);

	HAL_ADCEx_Calibration_Start(hadc, ADC_SINGLE_ENDED);

	if (HAL_ADC_PollForConversion(hadc, 1000000) == HAL_OK) {
		ADCvalue = HAL_ADC_GetValue(hadc);
	}
	else{
		ADCvalue = -1;
	}

	HAL_ADC_Stop(hadc);

	return ADCvalue;
}

float convert_adc_to_voltate(uint32_t adc_value){
	return adc_value / 10 * (10 + 47) * 3.3 / 4095;
}
