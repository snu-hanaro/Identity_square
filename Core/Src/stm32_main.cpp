/*
 * stm32_main.cpp
 *
 *  Created on: Apr 1, 2026
 *      Author: shpar
 */

#include "stm32_main.h"
#include "main.h"

#include "baro/bmp581.h"
#include "imu/icm20948.h"
#include "SD/SD.h"
#include "flight_control/flight_state.h"
#include "utils/micros.h"
#include "utils/stm32_uart_stdio.h"
#include "utils/debug_printf.h"
#include "utils/common.h"
#include "config.h"
#include "buzzer/switching_buzzer.h"
#include "buzzer/buzzer.h"
#include "ADC/ADC.h"
#include "quaternion.h"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;

extern I2C_HandleTypeDef hi2c1;

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

extern UART_HandleTypeDef huart2;

static int imu_cnt = 0;
static int imu_dmp_cnt = 0;
static int bmp_cnt = 0;
static uint32_t hzprint_timer_millis;

static icm20948_agmt_t icm20948_last_agmt;
static icm20948_dmp_quat_t icm20948_last_dmp_quat;
static bmp5_sensor_data bmp581_last_data;
extern float ground_pres;

static int ignite_counter =0;
static int acc_outlier_count;
MILLIS_TIMER_DEFINE(ignite);
bool is_ignited;

static FLIGHT_STATE flight_state;

void measure_databoard_voltage(){
	//initial sampling is not accurate
	for(int i = 0;i < 10;i++){
		analogRead(&DATABOARD_ADC_HANDLE);
		HAL_Delay(100);
	}

	float voltage = convert_adc_to_voltate(analogRead(&DATABOARD_ADC_HANDLE));
	DEBUG_PRINT("volt: %f\r\n", voltage);

	int volt_n = floor(voltage*10);
	int volt_cnt1 = volt_n/10, volt_cnt2 = volt_n%10;
	for(int i=0;i<volt_cnt1;i++){
		tone(400);
		HAL_Delay(250);
		noTone();
		HAL_Delay(250);
	}
	noTone();
	HAL_Delay(3000);
	for(int i=0;i<volt_cnt2;i++){
		tone(400);
		HAL_Delay(500);
		noTone();
		HAL_Delay(250);
	}
	noTone();
	HAL_Delay(3000);
}


void begin(void){
	DWT_Init();
	init_stdio();
	DEBUG_PRINT("\r\n--- Hanaro FC%d Starting ---\r\n", BOARD_ID);
	DEBUG_PRINT("start initializing...\r\n");
	init_buzzer(&htim3);
	tone(150);
	while(!init_imu(&hi2c1)) {
		DEBUG_PRINT("IMU initialize failed. Trying again...\r\n");
	}
	tone(200);
	init_baro(&hspi2, BMP_CS_GPIO_Port, BMP_CS_Pin);
	tone(250);
	init_sd(&hspi1, SD_CS_GPIO_Port, SD_CS_Pin);
	tone(300);
	
	char log_name[32];
    snprintf(log_name, sizeof(log_name), "FC%d_0405", BOARD_ID); 
    init_data_csv(log_name);
	
	measure_databoard_voltage();

	HAL_GPIO_WritePin(IGNITE_GPIO_Port, IGNITE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DROGUE_SRAD_GPIO_Port, DROGUE_SRAD_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MAIN_SRAD_GPIO_Port, MAIN_SRAD_Pin, GPIO_PIN_RESET);

	if(!sd_fail_flag) buzzer_play_ok();
	else buzzer_play_fail();

	init_switching_buzzer(&htim2);
	set_buzzer_switching_intv(1000);
	DEBUG_PRINT("initialization success!\r\n");

	hzprint_timer_millis = millis();

	//measure ground g vector
	float ground_ax, ground_ay, ground_az, ground_a;
	icm20948_agmt_t icm20948_agmt;
	do {
		ground_ax = ground_ay = ground_az = 0;
		for(int i = 0; i < 16; i++){
			while(!imu_is_agmt_data_ready()) ;
			imu_get_agmt(&icm20948_agmt);
			ground_ax += icm20948_agmt.acc_x;
			ground_ay += icm20948_agmt.acc_y;
			ground_az += icm20948_agmt.acc_z;
			HAL_Delay(100);
		}
		ground_ax /= 16;
		ground_ay /= 16;
		ground_az /= 16;
		ground_a = norm(ground_ax, ground_ay, ground_az);
	} while(fabs(ground_a - 1000) > 200);

	flight_state_machine_init_input init_input = {
		.ground_ax = ground_ax,
		.ground_ay = ground_ay,
		.ground_az = ground_az
	};
	flight_state = init_flight_state_machine(init_input);
	set_buzzer_switching_intv(state_buzzer_intv_millis[flight_state]);

	while(!imu_is_agmt_data_ready()) ;
	imu_get_agmt(&icm20948_last_agmt);

	bmp5_sensor_data bmp_data;
	while(bmp581.getSensorData(&bmp_data) != BMP5_OK);
	bmp581_last_data = bmp_data;

	is_ignited = false;
}



static float acc=0;
static float max_acc=0;





void loop(void){

	if(imu_is_agmt_data_ready()){
		imu_get_agmt(&icm20948_last_agmt);
		imu_cnt++;
		if (flight_state == STATE_INCREASING) {
			float alt = Pa2altM(bmp581_last_data.pressure, ground_pres);
#ifdef REAL_LAUNCH
			if (alt > IGNITE_MIN_ALT) {
#endif
				//check outlier
				acc=norm(icm20948_last_agmt.acc_x, icm20948_last_agmt.acc_y, icm20948_last_agmt.acc_z);
				printf("acc: %f\r\n", acc);
				if(acc > max_acc + 6000){
					acc_outlier_count++;
					if(acc_outlier_count >= 3) acc_outlier_count = 0;
				}
				else acc_outlier_count = 0;
				//outlier_count == 0 => apply to max_acc
				if(acc_outlier_count == 0){
					if(acc > max_acc){
						max_acc = acc;
						ignite_counter = 0;
					}
					else if(acc < max_acc - 35) ignite_counter++;
				}
#ifdef REAL_LAUNCH
			}
#endif
		}
	}

#ifdef ICM20948_USE_DMP
	icm20948_dmp_data_t icm20948_dmp_data;
	if(imu_get_dmp_data(&icm20948_dmp_data) && get_quaternion_from_dmp_data(icm20948_dmp_data, &icm20948_last_dmp_quat)){
//		DEBUG_PRINT("dmp quat: w: %f, x: %f, y: %f, z: %f\r\n", icm20948_last_dmp_quat.w, icm20948_last_dmp_quat.x, icm20948_last_dmp_quat.y, icm20948_last_dmp_quat.z);
#ifdef FC_DEBUG
		Quaternion q = Quaternion(icm20948_last_dmp_quat.w, icm20948_last_dmp_quat.x, icm20948_last_dmp_quat.y, icm20948_last_dmp_quat.z);
		float zx = 0, zy = 0, zz = 1;
		q.rotateVector(zx, zy, zz);
#endif
		imu_dmp_cnt++;
	}
#endif

	bmp5_sensor_data bmp_data;
	if(bmp581.getSensorData(&bmp_data) == BMP5_OK){
		bmp581_last_data = bmp_data;
		bmp_cnt++;
	}

	flight_state_machine_input input = {
		.time_millis = millis(),
		.acc_x = icm20948_last_agmt.acc_x,
		.acc_y = icm20948_last_agmt.acc_y,
		.acc_z = icm20948_last_agmt.acc_z,
		.gyr_x = icm20948_last_agmt.gyr_x,
		.gyr_y = icm20948_last_agmt.gyr_y,
		.gyr_z = icm20948_last_agmt.gyr_z,
		.mag_x = icm20948_last_agmt.mag_x,
		.mag_y = icm20948_last_agmt.mag_y,
		.mag_z = icm20948_last_agmt.mag_z,
		.quat_w = icm20948_last_dmp_quat.w,
		.quat_x = icm20948_last_dmp_quat.x,
		.quat_y = icm20948_last_dmp_quat.y,
		.quat_z = icm20948_last_dmp_quat.z,
		.pressure = bmp581_last_data.pressure,
		.temperature = bmp581_last_data.temperature
	};

	FLIGHT_STATE next_flight_state = run_flight_state_machine(input);

	if(flight_state != next_flight_state){
		flight_state = next_flight_state;
		set_buzzer_switching_intv(state_buzzer_intv_millis[flight_state]);
		if(flight_state == STATE_DROGUE_DEPLOY) {
			HAL_GPIO_WritePin(DROGUE_SRAD_GPIO_Port, DROGUE_SRAD_Pin, GPIO_PIN_SET);
		}
		else if(flight_state == STATE_MAIN_DEPLOY) {
			HAL_GPIO_WritePin(DROGUE_SRAD_GPIO_Port, DROGUE_SRAD_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(MAIN_SRAD_GPIO_Port, MAIN_SRAD_Pin, GPIO_PIN_SET);
		}
		else if(flight_state == STATE_TOUCHDOWN) {
			HAL_GPIO_WritePin(MAIN_SRAD_GPIO_Port, MAIN_SRAD_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(IGNITE_GPIO_Port, IGNITE_Pin, GPIO_PIN_RESET);
		}
	}
	if (!is_ignited) {
		if(flight_state >= STATE_CONF_DONE && ignite_counter >= 7 && !MILLIS_TIMER_ISSET(ignite)){
			MILLIS_TIMER_SET(ignite);
		}
		if (MILLIS_TIMER_CHECK(ignite, 470)) {
			HAL_GPIO_WritePin(IGNITE_GPIO_Port, IGNITE_Pin, GPIO_PIN_SET);
			is_ignited = true;
			set_buzzer_switching_intv(ignite_buzzer_intv_millis);
		}
	}
	
// 1. 3축 가속도 데이터의 벡터 크기(절대 가속도 값) 연산
    float acc_absolute = norm(icm20948_last_agmt.acc_x, icm20948_last_agmt.acc_y, icm20948_last_agmt.acc_z);

// 2. 현재 기압과 지상 기준 기압(ground_pres) 데이터를 비교하여 상대 고도 계산
    float relative_altitude = 0.0f; 

    // ground_pres가 0보다 클 때(즉, 2G 발사 조건이 충족되어 기압이 세팅된 후)만 계산
    if (ground_pres > 0.0f && bmp581_last_data.pressure > 0.0f) {
    float current_abs_alt = Pa2altM(bmp581_last_data.pressure, 101325.0f);
    float ground_abs_alt  = Pa2altM(ground_pres, 101325.0f);
    
    relative_altitude = current_abs_alt - ground_abs_alt;
}

	
	csv_format csv_data = {
		.devicetime_ms = HAL_GetTick(),
		.devicetime_us = micros(),
		.flight_state = flight_state,
		.a_abs = acc_absolute,
		.acc_x = icm20948_last_agmt.acc_x,
		.acc_y = icm20948_last_agmt.acc_y,
		.acc_z = icm20948_last_agmt.acc_z,
		.gyr_x = icm20948_last_agmt.gyr_x,
		.gyr_y = icm20948_last_agmt.gyr_y,
		.gyr_z = icm20948_last_agmt.gyr_z,
		.mag_x = icm20948_last_agmt.mag_x,
		.mag_y = icm20948_last_agmt.mag_y,
		.mag_z = icm20948_last_agmt.mag_z,
		.rel_alt = relative_altitude,
		.dmp_quat_w = (float)icm20948_last_dmp_quat.w,
		.dmp_quat_x = (float)icm20948_last_dmp_quat.x,
		.dmp_quat_y = (float)icm20948_last_dmp_quat.y,
		.dmp_quat_z = (float)icm20948_last_dmp_quat.z,
		.pressure = bmp581_last_data.pressure,
		.temperature = bmp581_last_data.temperature
	};

	write_data_csv(csv_data);

//	DEBUG_PRINT("flight state: %d\r\n", flight_state);

#ifdef FC_DEBUG
	if(millis() - hzprint_timer_millis > 3000){
		int dt = millis() - hzprint_timer_millis;
		hzprint_timer_millis = millis();
		DEBUG_PRINT("IMU: %.1f(Hz)\r\n", imu_cnt/(float)dt*1000);
		DEBUG_PRINT("BMP: %.1f(Hz)\r\n", bmp_cnt/(float)dt*1000);
		DEBUG_PRINT("DMP: %.1f(Hz)\r\n", imu_dmp_cnt/(float)dt*1000);
		imu_cnt = 0;
		bmp_cnt = 0;
		imu_dmp_cnt = 0;
	}
#endif

}
