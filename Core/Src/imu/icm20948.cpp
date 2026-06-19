/*
 * imu.cpp
 *
 *  Created on: Apr 1, 2026
 *      Author: shpar
 */

#include "imu/icm20948.h"

#include "utils/debug_printf.h"
#include <cmath>

ICM_20948_I2C icm20948;

#define ICM20948_CHECK_FAILURE(op) do { \
	ICM_20948_Status_e status; \
	if ((status = (op)) != ICM_20948_Stat_Ok) { \
		DEBUG_PRINT("failed to run " #op ", status: %d\r\n", status); \
		return false; \
	} \
} while(0)

bool init_imu(I2C_HandleTypeDef *hi2c){
	DEBUG_PRINT("Initializing IMU...\r\n");

#ifdef FC_DEBUG
	icm20948.enableDebugging();
#else
	icm20948.disableDebugging();
#endif

	ICM20948_CHECK_FAILURE(icm20948.begin(hi2c));
#ifdef ICM20948_USE_DMP
	// DMP 설정
	// 참조: https://github.com/sparkfun/SparkFun_ICM-20948_ArduinoLibrary/blob/main/examples/Arduino/Example6_DMP_Quat9_Orientation/Example6_DMP_Quat9_Orientation.ino
	ICM20948_CHECK_FAILURE(icm20948.initializeDMP());
	// DMP sensor options are defined in ICM_20948_DMP.h
	//    INV_ICM20948_SENSOR_ACCELEROMETER               (16-bit accel)
	//    INV_ICM20948_SENSOR_GYROSCOPE                   (16-bit gyro + 32-bit calibrated gyro)
	//    INV_ICM20948_SENSOR_RAW_ACCELEROMETER           (16-bit accel)
	//    INV_ICM20948_SENSOR_RAW_GYROSCOPE               (16-bit gyro + 32-bit calibrated gyro)
	//    INV_ICM20948_SENSOR_MAGNETIC_FIELD_UNCALIBRATED (16-bit compass)
	//    INV_ICM20948_SENSOR_GYROSCOPE_UNCALIBRATED      (16-bit gyro)
	//    INV_ICM20948_SENSOR_STEP_DETECTOR               (Pedometer Step Detector)
	//    INV_ICM20948_SENSOR_STEP_COUNTER                (Pedometer Step Detector)
	//    INV_ICM20948_SENSOR_GAME_ROTATION_VECTOR        (32-bit 6-axis quaternion)
	//    INV_ICM20948_SENSOR_ROTATION_VECTOR             (32-bit 9-axis quaternion + heading accuracy)
	//    INV_ICM20948_SENSOR_GEOMAGNETIC_ROTATION_VECTOR (32-bit Geomag RV + heading accuracy)
	//    INV_ICM20948_SENSOR_GEOMAGNETIC_FIELD           (32-bit calibrated compass)
	//    INV_ICM20948_SENSOR_GRAVITY                     (32-bit 6-axis quaternion)
	//    INV_ICM20948_SENSOR_LINEAR_ACCELERATION         (16-bit accel + 32-bit 6-axis quaternion)
	//    INV_ICM20948_SENSOR_ORIENTATION                 (32-bit 9-axis quaternion + heading accuracy)
	ICM20948_CHECK_FAILURE(icm20948.enableDMPSensor(INV_ICM20948_SENSOR_ORIENTATION));
  	// DMP is capable of outputting multiple sensor data at different rates to FIFO.
	// dmp odr = (dmp running rate) / (odr + 1)
	// dmp running rate == gyro_odr
	// gyro_odr != acc_odr 일 때는 확실하지 않음(하지만 웬만해선 gyro_odr == acc_odr 로 둬야 함)
	ICM20948_CHECK_FAILURE(icm20948.setDMPODRrate(DMP_ODR_Reg_Quat9, 0));
	//enable FIFO for dmp data
	ICM20948_CHECK_FAILURE(icm20948.enableFIFO());
	ICM20948_CHECK_FAILURE(icm20948.enableDMP());
	ICM20948_CHECK_FAILURE(icm20948.resetDMP());
	ICM20948_CHECK_FAILURE(icm20948.resetFIFO());
	// initializeDMP에서 가속도계와 자이로스코프의 Fullscale과 Sample rate를 설정하므로 DMP를 켤 경우 설정하지 않음.
	// DMP 사용시 Sample rate를 특정 값으로 맞춰야 정상 동작이 예상되므로 주의
	// 주의!! : DMP 사용시 DLPF를 끄면 동작이 보장되지 않음. 따라서 DMP 사용 시 무조건 DLPF는 켜짐
	// 라이브러리 코드는 DLPF가 켜진 상태에서 동작하도록 초기화함.
	// 또한 DLPF가 꺼질 경우, 가속도, 자이로의 샘플레이트가 4.5kHz, 9kHz로 고정되는데,
	// 이 때문에 dmp running rate가 알 수 없는 큰 값(약 1kHz 으로 추정됨)으로 변경됨.
#else
	// 측정 범위(full scale) 무조건 최대로
	ICM_20948_fss_t FSS;
	FSS.a = gpm16;
	FSS.g = dps2000;
	ICM20948_CHECK_FAILURE(icm20948.setFullScale((ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr), FSS));
	ICM_20948_smplrt_t SMPLRT;
	SMPLRT.a = 0;//sample rate = 1125/(smplrt+1) if dlpf is on else 4.5kHz
	SMPLRT.g = 0;//sample rate = 1125/(smplrt+1) if dlpf is on else 9kHz
	ICM20948_CHECK_FAILURE(icm20948.setSampleRate((ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr), SMPLRT));

#ifdef ICM20948_ENABLE_DLPF
	ICM_20948_dlpcfg_t DLP;
	DLP.a = 0;
	DLP.g = 0;
	ICM20948_CHECK_FAILURE(icm20948.setDLPFcfg((ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr), DLP));
#else
	ICM20948_CHECK_FAILURE(icm20948.enableDLPF((ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr), false));
#endif
#endif

	ICM20948_CHECK_FAILURE(icm20948.setSampleMode((ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr), ICM_20948_Sample_Mode_Continuous));

	icm20948.disableDebugging();

	DEBUG_PRINT("IMU initialized!\r\n");
	return true;
}

bool imu_is_agmt_data_ready(){
	return icm20948.dataReady();
}

bool imu_get_agmt(icm20948_agmt_t *agmt){
	icm20948.getAGMT();
	agmt->acc_x = icm20948.accX();
	agmt->acc_y = icm20948.accY();
	agmt->acc_z = icm20948.accZ();
	agmt->gyr_x = icm20948.gyrX();
	agmt->gyr_y = icm20948.gyrY();
	agmt->gyr_z = icm20948.gyrZ();
	agmt->mag_x = icm20948.magX();
	agmt->mag_y = icm20948.magY();
	agmt->mag_z = icm20948.magZ();
	agmt->temp = icm20948.temp();
	return true;
}

bool imu_get_dmp_data(icm20948_dmp_data_t *dmp_data){
	ICM_20948_Status_e status;
	do{
		status = icm20948.readDMPdataFromFIFO((icm_20948_DMP_data_t*)dmp_data);
		if (status == ICM_20948_Stat_Ok) {
			return true;
		}
	} while (status == ICM_20948_Stat_FIFOMoreDataAvail);
	if (status != ICM_20948_Stat_FIFONoDataAvail) {
		DEBUG_PRINT("failed to read DMP data from FIFO: %d\r\n", status);
	}
	return false;
}

ICM_20948_Status_e imu_get_status(){
	return icm20948.status;
}

bool get_quaternion_from_dmp_data(const icm20948_dmp_data_t &dmp_data, icm20948_dmp_quat_t *quat){
	if((dmp_data.header & DMP_header_bitmap_Quat9) == 0) return false;
	quat->x = ((double)dmp_data.Quat9.Data.Q1) / 1073741824.0;
	quat->y = ((double)dmp_data.Quat9.Data.Q2) / 1073741824.0;
	quat->z = ((double)dmp_data.Quat9.Data.Q3) / 1073741824.0;
	quat->w = sqrt(1.0 - (quat->x * quat->x + quat->y * quat->y + quat->z * quat->z));
	return true;
}
