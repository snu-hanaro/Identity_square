/*
 * flight_state.h
 *
 *  Created on: Apr 1, 2026
 *      Author: shpar
 */

#ifndef INC_FLIGHT_CONTROL_FLIGHT_STATE_H_
#define INC_FLIGHT_CONTROL_FLIGHT_STATE_H_

#include "config.h"

//flight state: READY->LAUNCH->IGNITE->MAIN->TOUCHDOWN

/*
 * flight state
 * 1 = power on
 * 2 = launched
 * 3 = 2nd stage ignition
 *  = drogue fired
 * 4 = main fired
 * 5 = touchdown
 */
enum FLIGHT_STATE{
	FLIGHT_STATE_READY = 1,
	FLIGHT_STATE_LAUNCH,
	FLIGHT_STATE_DROGUE_PARACHUTE_DEPLOYED,
	FLIGHT_STATE_MAIN_PARACHUTE_DEPLOYED,
	FLIGHT_STATE_TOUCHDOWN,
};

// 아래 두 데이터는 센서 의존성을 줄이기 위한 flight state machine에서 참조 가능한 데이터 구조체이다. 
// 상황에 따라 수정하고, 외부에서 센서 데이터에서 필요한 부분만 뽑아서 저장해서 전달
// flight state machine 초기화 시 참조 가능한 데이터
struct flight_state_machine_init_input{
	float ground_ax, ground_ay, ground_az;
};

// flight state machine에서 참조 가능한 데이터
struct flight_state_machine_input{
	uint32_t time_millis;
	float acc_x, acc_y, acc_z;
	float gyr_x, gyr_y, gyr_z;
	float mag_x, mag_y, mag_z;
	float quat_w, quat_x, quat_y, quat_z;
	float pressure, temperature;
	int passive;
};

// flight state machine 초기화 함수
FLIGHT_STATE init_flight_state_machine(const flight_state_machine_init_input &init_input);
// flight state machine 실행 함수
FLIGHT_STATE run_flight_state_machine(const flight_state_machine_input &input);

#endif /* INC_FLIGHT_CONTROL_FLIGHT_STATE_H_ */
