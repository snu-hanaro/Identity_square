/*
 * callbacks.cpp
 *
 *  Created on: Apr 1, 2026
 *      Author: shpar
 */

// 인터럽트 콜백 함수를 모아둔 파일
// 콜백 함수를 모아야 하는 이유:
// 이를테면 Timer의 콜백 함수는 htim2, htim3의 인터럽트를 모두 받는다
// 따라서 콜백 함수를 기능별로 정의하기 어렵고, 하나의 파일 안에서 모든 콜백 함수를 관리해야 한다.

#include "stm32l4xx_hal.h"
// 콜백 함수를 사용하는 모듈을 모두 include 한다.
#include "buzzer/switching_buzzer.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == buzzer_switch_htim->Instance){
		if(buzzer_switch){
			tone(500);
			buzzer_switch = false;
		}
		else{
			noTone();
			buzzer_switch = true;
		}
	}
}

