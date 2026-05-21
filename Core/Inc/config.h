/*
 * config.h
 *
 *  Created on: Apr 1, 2026
 *      Author: shpar
 */

#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

// 설정 매크로를 모아둔 파일
#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif

// printf 디버깅을 설정/해제하는 매크로
#define FC_DEBUG
// 중력가속도
#define G_MS2 (9.80665f)
// 발사 전 지상에서 지면 압력 측정에 사용할 샘플 수
#define GROUND_PRESSURE_NUM 128
// SD 실패 허용 횟수
#define SD_FAIL_THRESHOLD 10
//FC1 / FC2
#define BOARD_ID 1   // 1단 보드는 1, 2단 보드는 2로 수정

#if (BOARD_ID == 1)
    #define FC1
#elif (BOARD_ID == 2)
    #define FC2
#endif

// [안전장치] 컴파일 시점에 FC1과 FC2 중 정확히 하나만 켜졌는지 검사
#if defined(FC1) && defined(FC2)
    #error "Only one of FC1 and FC2 can be defined"
#elif !defined(FC1) && !defined(FC2)
    #error "One of FC1 and FC2 must be defined"
#endif

// 공전회 테스트 시 켜는 매크로(타이머)
//#define RECOVERY_TEST
// 공전회 이외 지상 테스트 시 켜는 매크로(아래 최소 고도 조건 바활성화)
//#define GROUND_TEST
// 실제 발사 시 켜는 매크로
#define REAL_LAUNCH

#define DROGUE_DEPLOY_MIN_ALT 20.0f //20.0f for real launch
#define MAIN_DEPLOY_ALT 150.0f
#define IGNITE_MIN_ALT 20.0f //20.0f for real launch

// 실제 발사시에는 디버깅 해제하여 성능 향상
//#ifdef REAL_LAUNCH
//#undef FC_DEBUG
//#endif


// dmp 사용시 켜기
#define ICM20948_USE_DMP

// 주의!! : DMP 사용 시 DLPF를 끄면 동작이 보장되지 않음. 따라서 DMP 사용 시 무조건 DLPF는 켜짐
// 라이브러리 코드는 DLPF가 켜진 상태에서 동작하도록 초기화함.
// 또한 DLPF가 꺼질 경우, 가속도, 자이로의 샘플레이트가 4.5kHz, 9kHz로 고정되는데,
// 이 때문에 dmp running rate가 알 수 없는 큰 값(약 1kHz 으로 추정됨)으로 변경됨.

// icm20948의 DLPF(Digital Low Pass Filter) 사용시 켜기
// raw data 수집을 위해 끄는 것이 디폴트
//#define ICM20948_ENABLE_DLPF
static const uint32_t state_buzzer_intv_millis[10] = {0,
	300, // FLIGHT_STATE_READY
	100, // FLIGHT_STATE_LAUNCH
	2000, //FLIGHT_STATE_DROGUE_PARACHUTE_DEPLOYED
	1000,// FLIGHT_STATE_MAIN_PARACHUTE_DEPLOYED
	4000 // FLIGHT_STATE_TOUCHDOWN
};
static const uint32_t ignite_buzzer_intv_millis = 10; // FLIGHT_STATE_LAUNCH 중 점화 시

#endif /* INC_CONFIG_H_ */
