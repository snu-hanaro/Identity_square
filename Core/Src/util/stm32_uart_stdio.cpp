/*
 * stm32_uart_stdio.cpp
 *
 *  Created on: Apr 1, 2026
 *      Author: shpar
 */

#include "utils/stm32_uart_stdio.h"
#include <cstdio>
#include "stm32l4xx_hal.h"

extern UART_HandleTypeDef STDIO_UART_HANDLE;

#ifdef __cplusplus
extern "C" {
#endif
	int _write(int fd, char *ptr, int len) {
		HAL_UART_Transmit(&STDIO_UART_HANDLE, (unsigned char*) ptr, len, HAL_MAX_DELAY);
		return len;
	}

	int _read(int file, char *ptr, int len) {
		HAL_UART_Receive(&STDIO_UART_HANDLE, (unsigned char*) ptr, len, HAL_MAX_DELAY);
		return len;
	}
	int __io_putchar(int ch) {
		HAL_UART_Transmit(&STDIO_UART_HANDLE, (unsigned char*) &ch, 1, HAL_MAX_DELAY);
		return ch;
	}

	int __io_getchar(void) {
		uint8_t ch = 0;
		HAL_UART_Receive(&STDIO_UART_HANDLE, (uint8_t*) &ch, 1, HAL_MAX_DELAY);
		return ch;
	}

#ifdef __cplusplus
}
#endif

void init_stdio(void){
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
}
