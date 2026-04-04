/*
 * switching_buzzer.cpp
 *
 *  Created on: Apr 1, 2026
 *      Author: shpar
 */

#include "buzzer/switching_buzzer.h"


volatile bool buzzer_switch;
TIM_HandleTypeDef *buzzer_switch_htim;

void init_switching_buzzer(TIM_HandleTypeDef *htim){
	buzzer_switch_htim = htim;
}

void set_buzzer_switching_intv(uint32_t intv_millis){
	HAL_TIM_Base_Stop_IT(buzzer_switch_htim);
	__HAL_TIM_SET_COUNTER(buzzer_switch_htim, 0);
	__HAL_TIM_SET_AUTORELOAD(buzzer_switch_htim, intv_millis);
	HAL_TIM_Base_Start_IT(buzzer_switch_htim);
}
