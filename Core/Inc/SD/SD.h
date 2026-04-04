/*
 * SD.h
 *
 *  Created on: Apr 1, 2026
 *      Author: shpar
 */

#ifndef INC_SD_H_
#define INC_SD_H_
// SD 관련 기능을 모아둔 모듈
// SD를 사용하기 위해서는 fatfs를 ioc에서 켜야 한다.
// 참고: https://blog.naver.com/eziya76/221188701172

#include "stm32l4xx_hal.h"
#include "SD/csv_format.h"

extern bool sd_fail_flag;

bool init_sd(SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin);

bool sync_SD(void);

bool init_data_csv(const char *file_prefix);

bool write_data_csv(const csv_format &data);

#endif /* INC_SD_H_ */
