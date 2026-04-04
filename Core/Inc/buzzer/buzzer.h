/*
 * buzzer.h
 *
 *  Created on: Apr 1, 2026
 *      Author: shpar
 */

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

//htim(stm32의 타이머)을 이용한 부저를 다루는 라이브러리

#include "stm32l4xx_hal.h"

// 부저를 초기화하는 함수
bool init_buzzer(TIM_HandleTypeDef *htim);

// 부저를 키는 함수(아두이노와 동일)
void tone(int frequency);

// 부저를 끄는 함수(아두이노와 동일)
void noTone(void);

// ok(마리오 브금)을 내는 함수
void buzzer_play_ok(void);

// fail(띠로리로리)을 내는 함수
void buzzer_play_fail(void);
#endif /* INC_BUZZER_H_ */
