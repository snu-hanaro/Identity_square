/*
 * flight_state.cpp
 *
 *  Created on: Apr 1, 2026
 *      Author: shpar
 */

#include "flight_control/flight_state.h"

#include "utils/debug_printf.h"
#include "utils/common.h"
#include "utils/stopwatch.h"

static FLIGHT_STATE flight_state = FLIGHT_STATE_READY;

static float ground_ax, ground_ay, ground_az;
static float ground_a;

float ground_pres = 0;
static float last_pressure[GROUND_PRESSURE_NUM];
static int last_pressure_idx = 0;
static int last_pressure_count = 0;
static float max_alt = 0;
static float alt = 0.0;
static int lower_count = 0, outlier_count = 0;
static int burnout_count = 0;

static stopwatch_millis_t main_stopwatch;
static stopwatch_millis_t apogee_stopwatch;

FLIGHT_STATE init_flight_state_machine(const flight_state_machine_init_input &init_input){
	ground_ax = init_input.ground_ax;
	ground_ay = init_input.ground_ay;
	ground_az = init_input.ground_az;
	ground_a = norm(ground_ax, ground_ay, ground_az);
	flight_state = FLIGHT_STATE_READY;
	return flight_state;
}

FLIGHT_STATE run_flight_state_machine(const flight_state_machine_input &input){
	if(flight_state > FLIGHT_STATE_READY){
		//compute alt after ground_pres is obtained(after launch)
		alt = Pa2altM(input.pressure, ground_pres);
		//check outlier
		if(alt > max_alt + 10){
			outlier_count++;
			if(outlier_count >= 3) outlier_count = 0;
		}
		else outlier_count = 0;
		//outlier_count == 0 => apply to max_alt
		if(outlier_count == 0){
			if(alt > max_alt){
				max_alt = alt;
				lower_count = 0;
			}
			else if(alt < max_alt - 2.0) lower_count++;
		}
	}
	else{
		last_pressure[last_pressure_idx++] = input.pressure;
		if(last_pressure_idx == GROUND_PRESSURE_NUM) last_pressure_idx = 0;
		last_pressure_count++;
		alt = 0.0;
	}

	//================================================
	//burnout_count --> 2g 미만인 순간이 연속으로 몇번인지 확인
	if(flight_state >= FLIGHT_STATE_LAUNCH){
		if(norm(input.acc_x, input.acc_y, input.acc_z) <= 2000){
			burnout_count++;
		}else{
			burnout_count=0;
		}
	}

	//flight state management
	switch(flight_state){
	case FLIGHT_STATE_READY:
		//IMU의 배치 방향이 달라질 수 있으므로, 정사영 >= 2g로 판단
		if((input.acc_x * ground_ax + input.acc_y * ground_ay + input.acc_z * ground_az)
			/ground_a >= 2000){
			ground_pres = 0.0;
			for(int i = 0; i < min(last_pressure_count, GROUND_PRESSURE_NUM); i++){
				ground_pres += last_pressure[i];
			}
			ground_pres /= min(last_pressure_count, GROUND_PRESSURE_NUM);
			flight_state = FLIGHT_STATE_LAUNCH;
			lower_count = 0;
			outlier_count = 0;
			max_alt = 0;
#ifdef RECOVERY_TEST
			main_stopwatch.set();
#endif
		}
		break;

	case FLIGHT_STATE_LAUNCH:
		//공전회 --> 30초, 실제 발사에서 주석 부분
		if (
#ifdef RECOVERY_TEST
	#ifdef FC1
		main_stopwatch.elapsed() >= 1000*30
	#else
		main_stopwatch.elapsed() >= 1000*30 + 700
	#endif
#else
		lower_count >= 5
#ifdef GROUND_TEST
		&& alt >= MAIN_DEPLOY_MIN_ALT
#endif
#endif
		){
#ifdef FC1
			apogee_stopwatch.set();
			flight_state = FLIGHT_STATE_MAIN_PARACHUTE_DEPLOYED;
#else
			if(!main_stopwatch.is_set()){
				main_stopwatch.set();
			}
#endif
		}
#ifdef FC2
		if(main_stopwatch.is_set() && main_stopwatch.elapsed() >= 700){
			apogee_stopwatch.set();
			flight_state = FLIGHT_STATE_MAIN_PARACHUTE_DEPLOYED;
		}
#endif
		break;

	case FLIGHT_STATE_MAIN_PARACHUTE_DEPLOYED:
		if(apogee_stopwatch.is_set() && apogee_stopwatch.elapsed() > 10*1000){ //10*60*1000 for real flight
			flight_state = FLIGHT_STATE_TOUCHDOWN;
		}
		break;
	default:
		break;
	}
	
	return flight_state;
}
