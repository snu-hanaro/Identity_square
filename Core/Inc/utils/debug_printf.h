/*
 * debug_printf.h
 *
 *  Created on: Apr 1, 2026
 *      Author: shpar
 */

#ifndef INC_UTILS_DEBUG_PRINTF_H_
#define INC_UTILS_DEBUG_PRINTF_H_

#include "config.h"

#include <cstdio>

#ifdef FC_DEBUG
#define DEBUG_PRINT(...) printf(__VA_ARGS__ )
#else
#define DEBUG_PRINT(...) do{ } while ( 0 )
#endif

#endif /* INC_UTILS_DEBUG_PRINTF_H_ */
