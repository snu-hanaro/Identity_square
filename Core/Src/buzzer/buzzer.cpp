/*
 * buzzer.cpp
 *
 *  Created on: Apr 1, 2026
 *      Author: shpar
 */

#include "buzzer/buzzer.h"

// 설정한 타이머의 frequency
// todo: htim으로부터 동적으로 얻을 수 있도록 변경하기
#define TIM_FREQ 40000000
// 사용할 htim
static TIM_HandleTypeDef *buzzer_htim;

bool init_buzzer(TIM_HandleTypeDef *htim){
	buzzer_htim = htim;
	return true;
}

static int pres_for_frequency(int frequency)
{
	if (frequency == 0) return 0;
	return ((TIM_FREQ/(1000*frequency))-1);	// 1 is added in the register
}

void tone(int frequency){
	__HAL_TIM_SET_PRESCALER(buzzer_htim, pres_for_frequency(frequency));
	HAL_TIM_PWM_Start(buzzer_htim, TIM_CHANNEL_1);
}

void noTone(void){
	HAL_TIM_PWM_Stop(buzzer_htim, TIM_CHANNEL_1);
}


void buzzer_play_ok(void){
    tone(1319);
    HAL_Delay(100);
    noTone();
    HAL_Delay(50);
    tone(1319);
    HAL_Delay(100);
    noTone();
    HAL_Delay(50);
    HAL_Delay(150);
    tone(1319);
    HAL_Delay(100);
    noTone();
    HAL_Delay(50);
    HAL_Delay(150);
    tone(1047);
    HAL_Delay(100);
    noTone();
    HAL_Delay(50);
    tone(1319);
    HAL_Delay(300);
    tone(1568);
    HAL_Delay(300);
    tone(784);
    HAL_Delay(300);
    noTone();
    HAL_Delay(300);
}

void buzzer_play_fail(void){
    tone(1319);
    HAL_Delay(300);
    tone(1245);
    HAL_Delay(300);
    tone(1319);
    HAL_Delay(300);
    tone(1245);
    HAL_Delay(300);
    tone(1319);
    HAL_Delay(300);
    tone(988);
    HAL_Delay(300);
    tone(1175);
    HAL_Delay(300);
    tone(1047);
    HAL_Delay(300);
    tone(880);
    HAL_Delay(1200);
    noTone();
}
