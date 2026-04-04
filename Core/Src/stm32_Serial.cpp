/*
 * stm32_Serial.cpp
 *
 *  Created on: Aug 18, 2024
 *      Author: shpar
 */

#include "stm32_Serial.h"
#include <cstdio>

STM32Serial::STM32Serial():_huart{nullptr}{};

bool STM32Serial::init(UART_HandleTypeDef *huart)
{
    _huart = huart;
    _rxBufferHead = _rxBufferTail = 0;
    _txBufferHead = _txBufferTail = 0;
    if(HAL_UART_Receive_IT(_huart, _rxBuffer, 1) != HAL_OK) return false;
    return true;
}
// interrupt handler for receive
void STM32Serial::RxCpltCallback(void)
{
	//putchar(_rxBuffer[_rxBufferTail]);
    _rxBufferTail++;
    if(_rxBufferTail >= sizeof(_rxBuffer))
        _rxBufferTail -= sizeof(_rxBuffer);
    if(_rxBufferHead == _rxBufferTail){
        _rxBufferHead++;
    	if(_rxBufferHead >= sizeof(_rxBuffer)){
    		_rxBufferHead -= sizeof(_rxBuffer);
    	}
    }
    HAL_UART_Receive_IT(_huart, _rxBuffer + _rxBufferTail, 1);
}
// interrupt handler for transmit
void STM32Serial::TxCpltCallback(void)
{
    _txBufferHead += _huart->TxXferSize;
    if(_txBufferHead >= sizeof(_txBuffer)) _txBufferHead -= sizeof(_txBuffer);
    size_t size;
    if(_txBufferTail >= _txBufferHead) size = _txBufferTail - _txBufferHead;
    else size = sizeof(_txBuffer) - _txBufferHead;
    if(size > 0) HAL_UART_Transmit_IT(_huart, _txBuffer + _txBufferHead, size);
}
bool STM32Serial::changeBaudrate(uint32_t baudrate){
  //deinit uart
  if(HAL_UART_Abort_IT(_huart) != HAL_OK) return false;
  if(HAL_UART_DeInit(_huart) != HAL_OK) return false;
  //set baudrate
  _huart->Init.BaudRate = baudrate;
  //init, start receiving
  if(HAL_UART_Init(_huart) != HAL_OK) return false;
  if(HAL_UART_Receive_IT(_huart, _rxBuffer, 1) != HAL_OK) return false;
  _rxBufferHead = _rxBufferTail = 0;
  _txBufferHead = _txBufferTail = 0;
  return true;
}
size_t STM32Serial::writeBytes(uint8_t *data, size_t length)
{
	size_t size = length;
	uint32_t reg = READ_REG(_huart->Instance->CR1);
	__HAL_UART_DISABLE_IT(_huart, UART_IT_TXE);
	__HAL_UART_DISABLE_IT(_huart, UART_IT_TC);
    if(_txBufferTail + length >= sizeof(_txBuffer)){
    	size = sizeof(_txBuffer) - _txBufferTail;
    	memcpy(_txBuffer + _txBufferTail, data, size);
    	memcpy(_txBuffer, data + size, length - size);
    	_txBufferTail = length - size;
    }
    else{
    	memcpy(_txBuffer + _txBufferTail, data, size);
    	_txBufferTail += size;
    }
    WRITE_REG(_huart->Instance->CR1, reg);
    if(HAL_UART_Transmit_IT(_huart, _txBuffer + _txBufferHead, size) != HAL_OK) return 0;
    return length;
}
size_t STM32Serial::write(uint8_t data)
{
    return writeBytes(&data, 1);
}
size_t STM32Serial::readBytes(uint8_t *data, size_t length)
{
    size_t length_left = length;
    while (length_left > 0)
    {
    	if(available()){
        	*(data++) = read();
        	length_left--;
    	}
    }
    return length;
}
size_t STM32Serial::read()
{
	if (_rxBufferHead == _rxBufferTail) return 0xFF;
    uint8_t c = _rxBuffer[_rxBufferHead++];
    if (_rxBufferHead >= sizeof(_rxBuffer))
        _rxBufferHead -= sizeof(_rxBuffer);
    return c;
}
size_t STM32Serial::available()
{
	if(_txBufferTail >= _txBufferHead) return _rxBufferTail - _rxBufferHead;
	else return _rxBufferTail - _rxBufferHead + sizeof(_rxBuffer);
}
