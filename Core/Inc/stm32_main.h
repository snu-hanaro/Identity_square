/*
 * stm32_main.h
 *
 *  Created on: Apr 1, 2026
 *      Author: shpar
 */

#ifndef INC_STM32_MAIN_H_
#define INC_STM32_MAIN_H_

#include <utils/stm32_uart_stdio.h>

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void begin(void);
void loop(void);

#ifdef __cplusplus
}
#endif

#endif /* INC_STM32_MAIN_H_ */
