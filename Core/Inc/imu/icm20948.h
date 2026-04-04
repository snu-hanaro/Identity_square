/*
 * imu.h
 *
 *  Created on: Apr 1, 2026
 *      Author: shpar
 */

#ifndef INC_IMU_H_
#define INC_IMU_H_

// IMU(ICM20948)을 관리하는 모듈

#include "stm32l4xx_hal.h"
#include "ICM_20948/ICM_20948.hpp"
#include "config.h"

struct icm20948_agmt_t{
	float acc_x, acc_y, acc_z;
	float gyr_x, gyr_y, gyr_z;
	float mag_x, mag_y, mag_z;
	float temp;
};

typedef icm_20948_DMP_data_t icm20948_dmp_data_t;

struct icm20948_dmp_quat_t{
	double w, x, y, z;
};
// IMU 초기화 함수, I2C 핸들러 참조 전달
bool init_imu(I2C_HandleTypeDef *hi2c);
// AGMT(accelerometer, gyroscope, magnetometer, temperature) 데이터가 준비됐는지 여부 반환
bool imu_is_agmt_data_ready();
// AGMT 데이터 읽어서 전달받은 포인터가 가리키는 구조체에 저장
bool imu_get_agmt(icm20948_agmt_t *agmt);
// DMP(Digital Motion Processor) 데이터 읽어서 전달받은 포인터가 가리키는 구조체에 저장, 읽기 성공 여부 반환
bool imu_get_dmp_data(icm_20948_DMP_data_t *dmp_data);
// IMU 상태 체크용
ICM_20948_Status_e imu_get_status();
// DMP 데이터에서 쿼터니언 계산해서 전달받은 포인터가 가리키는 구조체에 저장
bool get_quaternion_from_dmp_data(const icm20948_dmp_data_t &dmp_data, icm20948_dmp_quat_t *quat);

#endif /* INC_IMU_H_ */
