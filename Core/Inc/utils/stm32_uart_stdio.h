/*
 * stm32_uart_stdio.h
 *
 *  Created on: Apr 1, 2026
 *      Author: shpar
 */

// stm32에서 UART를 통해 stdio(printf, scanf 등등)을 쓸 수 있게 해주는 유틸리티
// 실수 입출력(%f)의 경우 별도 프로젝트 설정 필요
// 실수 입출력 참조: https://swiftcam.tistory.com/144

#ifndef INC_STM32_UART_STDIO_H_
#define INC_STM32_UART_STDIO_H_

#define STDIO_UART_HANDLE huart2

// stdio에서 사용하는 버퍼링을 제거하여 stm32용 디버깅으로 설정하는 함수
void init_stdio(void);

#endif /* INC_STM32_UART_STDIO_H_ */
