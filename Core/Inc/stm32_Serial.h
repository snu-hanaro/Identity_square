/*
 * stm32_Serial.h
 *
 *  Created on: Aug 18, 2024
 *      Author: shpar
 */

#ifndef INC_STM32_SERIAL_H_
#define INC_STM32_SERIAL_H_

#include "stm32l4xx.h"

#include <cstring>

#define STM32SERIAL_RX_BUFFER_SIZE 4096
#define STM32SERIAL_TX_BUFFER_SIZE 4096

class STM32Serial{
private:
    // use ring buffer and interrupt handler for receive
    volatile size_t _rxBufferHead, _rxBufferTail;
    volatile size_t _txBufferHead, _txBufferTail;
    uint8_t _rxBuffer[STM32SERIAL_RX_BUFFER_SIZE];
    uint8_t _txBuffer[STM32SERIAL_TX_BUFFER_SIZE];

public:
    UART_HandleTypeDef *_huart;
    STM32Serial();
    bool init(UART_HandleTypeDef *huart);
    // interrupt handler for receive
    void RxCpltCallback(void);
    void TxCpltCallback(void);
    bool changeBaudrate(uint32_t baudrate);
    // use hal polling(HAL_UART_Transmit) for transmit
    size_t writeBytes(uint8_t *data, size_t length);
    size_t write(uint8_t data);
    size_t readBytes(uint8_t *data, size_t length);
    size_t read();
    size_t available();
};




#endif /* INC_STM32_SERIAL_H_ */
