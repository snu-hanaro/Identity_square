/*
 * csv_format.h
 *
 *  Created on: Apr 2, 2026
 *      Author: shpar
 */

#ifndef INC_SD_CSV_FORMAT_H_
#define INC_SD_CSV_FORMAT_H_

#include <cstdint>
#include <cstddef>
#include "flight_control/flight_state.h"

struct csv_format{
    //devicetime
    uint32_t devicetime_ms;
    uint32_t devicetime_us;
    //flight state
    FLIGHT_STATE flight_state;
    //imu(icm20948)
    float acc_x, acc_y, acc_z;
    float gyr_x, gyr_y, gyr_z;
    float mag_x, mag_y, mag_z;
    //imu(dmp, icm20948)
    float dmp_quat_w, dmp_quat_x, dmp_quat_y, dmp_quat_z;
    //baro(bmp581)
    float pressure, temperature;
};

extern const char *csv_format_columns;

void csv_format_to_str(const csv_format &data, char *buf, size_t buf_size);

#endif /* INC_SD_CSV_FORMAT_H_ */
