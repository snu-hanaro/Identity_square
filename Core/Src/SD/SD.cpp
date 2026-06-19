/*
 * SD.cpp
 *
 *  Created on: Apr 1, 2026
 *      Author: shpar
 */

#include "SD/SD.h"
#include "fatfs.h"

#include "config.h"
#include "utils/debug_printf.h"
#include <cstring>

#define SD_FAIL_COUNT_RUN(op, debug_op) do { \
	FRESULT res; \
    sd_fail_count = 0; \
    while (sd_fail_count < SD_FAIL_THRESHOLD && (res = op) != FR_OK) { \
        sd_fail_count++; \
        DEBUG_PRINT("failed to run " #op ", count: %d, result: %d\r\n", sd_fail_count, res); \
        debug_op; \
    } \
    if (sd_fail_count == SD_FAIL_THRESHOLD) { \
        sd_fail_flag = true; \
        return false; \
    } \
} while(0)

FATFS FatFs;		//Fatfs handle
FIL dataFile;

extern "C" {
	SPI_HandleTypeDef *SDHspi;
	GPIO_TypeDef *SDCsPort;
	uint16_t SDCsPin;
}

static int sd_fail_count;
bool sd_fail_flag = false;
extern const char *csv_format_columns;
char csv_filename[256];

bool init_sd(SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin){
	DEBUG_PRINT("SD Init Start...\r\n");
	SDHspi = hspi;
	SDCsPort = cs_port;
	SDCsPin = cs_pin;
	
	SD_FAIL_COUNT_RUN(f_mount(&FatFs, "", 1), HAL_Delay(200));
	if (sd_fail_flag) return false;
	DEBUG_PRINT("SD mounted successfully\r\n");
	return true;
}

bool sync_SD(void){
	if (sd_fail_flag) return false;
	SD_FAIL_COUNT_RUN(f_sync(&dataFile), HAL_Delay(200));
	if (sd_fail_flag) return false;
	return true;
}

bool init_data_csv(const char *file_prefix){
	if (sd_fail_flag) return false;

	int num;
	UINT bytesWrote;

	for(num = 0; num < 1000; num++){
		sprintf(csv_filename, "%s_%03d.csv", file_prefix, num);
		FRESULT res = f_stat(csv_filename, NULL);
		if(res == FR_NO_FILE) break;
		else if(res != FR_OK){
			sd_fail_flag = true;
			DEBUG_PRINT("failed to check existing files in SD!, result: %d\r\n", res);
			return false;
		}
	}
	if(num == 1000){
		sd_fail_flag = true;
		DEBUG_PRINT("too many existing files with the same prefix!\r\n");
		return false;
	}
	DEBUG_PRINT("creating file: %s\r\n", csv_filename);

	SD_FAIL_COUNT_RUN(f_open(&dataFile, csv_filename, FA_CREATE_ALWAYS | FA_WRITE), HAL_Delay(200));
	SD_FAIL_COUNT_RUN(f_write(&dataFile, csv_format_columns, strlen(csv_format_columns), &bytesWrote), HAL_Delay(200));

	DEBUG_PRINT("wrote: %d\r\n", bytesWrote);
	sync_SD();
	if (sd_fail_flag) return false;
	return true;
}

bool write_data_csv(const csv_format &data){
	if (sd_fail_flag) return false;

	char buf[1024];
	csv_format_to_str(data, buf, sizeof(buf));

	UINT bytesWrote;
	SD_FAIL_COUNT_RUN(f_write(&dataFile, buf, strlen(buf), &bytesWrote), HAL_Delay(200));
	sync_SD();
	if (sd_fail_flag) return false;
	return true;
}
