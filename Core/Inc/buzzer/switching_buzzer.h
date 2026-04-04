/*
 * switching_buzzer.h
 *
 *  Created on: Apr 1, 2026
 *      Author: shpar
 */

#ifndef INC_SWITCHING_BUZZER_H_
#define INC_SWITCHING_BUZZER_H_

// 특정 간격으로 스위칭하는(on/off하는) 부저를 관리하는 모듈

#include "stm32l4xx_hal.h"
#include "buzzer.h"

extern volatile bool buzzer_switch;
extern TIM_HandleTypeDef *buzzer_switch_htim;

// 스위칭 부저 초기화 함수
void init_switching_buzzer(TIM_HandleTypeDef *htim);

// 스위칭 부저의 스위칭 간격을 변경하는 함수
void set_buzzer_switching_intv(uint32_t intv_millis);

#endif /* INC_SWITCHING_BUZZER_H_ */
