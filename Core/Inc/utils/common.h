/*
 * common.h
 *
 *  Created on: Apr 2, 2026
 *      Author: shpar
 */

#ifndef INC_UTILS_COMMON_H_
#define INC_UTILS_COMMON_H_

#include <cmath>
#include "stm32l4xx_hal.h"

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#define MILLIS_TIMER_DEFINE(name) static bool name##_timer_set; \
	static uint32_t name##_timer_millis;

#define MILLIS_TIMER_ISSET(name) (name##_timer_set)

#define MILLIS_TIMER_SET(name) do{ \
	name##_timer_set = true; \
	name##_timer_millis = millis(); \
} while(0)

#define MILLIS_TIMER_UNSET(name) do{ \
	name##_timer_set = false; \
} while(0)

#define MILLIS_TIMER_CHECK(name, intv_millis) (name##_timer_set && millis() > name##_timer_millis + intv_millis)

static inline float Pa2altM(float Pa, float sea_level_Pa) {
  return 44307.69396*(1.0 - pow(Pa/sea_level_Pa, 0.190284));
}

static inline float norm(float x, float y, float z){
	return sqrt(x*x + y*y + z*z);
}

static inline uint32_t millis(void) {
	return HAL_GetTick();
}


#endif /* INC_UTILS_COMMON_H_ */
