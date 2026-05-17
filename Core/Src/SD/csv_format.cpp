/*
 * csv_format.cpp
 *
 *  Created on: Apr 2, 2026
 *      Author: shpar
 */

#include "SD/csv_format.h"

#include <cstdio>

const char *csv_format_columns =
    "devicetime_ms,devicetime_us,"
    "flight_state,"
    "acc_x,acc_y,acc_z,"
    "gyr_x,gyr_y,gyr_z,"
    "mag_x,mag_y,mag_z,"
    "dmp_quat_w,dmp_quat_x,dmp_quat_y,dmp_quat_z,"
    "pressure,temperature\n";

void csv_format_to_str(const csv_format &data, char *buf, size_t buf_size){
    snprintf(buf, buf_size, 
        "%u,%u,"
        "%d,"
        "%.3f,%.3f,%.3f,"
        "%.3f,%.3f,%.3f,"
        "%.3f,%.3f,%.3f,"
        "%.5f,%.5f,%.5f,%.5f,"
        "%.2f,%.2f,"
        "%d\n",
        data.devicetime_ms, data.devicetime_us,
        data.flight_state,
        data.acc_x, data.acc_y, data.acc_z,
        data.gyr_x, data.gyr_y, data.gyr_z,
        data.mag_x, data.mag_y, data.mag_z,
        data.dmp_quat_w, data.dmp_quat_x, data.dmp_quat_y, data.dmp_quat_z,
        data.pressure, data.temperature
    );
}
