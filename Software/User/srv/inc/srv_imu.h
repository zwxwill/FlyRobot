/**
  ******************************************************************************
  * @file    srv_imu.h
  * @author  zwx
  * @version V1.8.0
  * @date    2019-01-29
  * @brief   
  ******************************************************************************
  */  

#ifndef __SRV_IMU_H__
#define __SRV_IMU_H__

#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported define -----------------------------------------------------------*/
#define IMU_LOOP_TIME		5000.0f		//单位为uS
#define ACC_LPF_CUT 		10.0f		//加速度低通滤波器截止频率10Hz
/* Exported typedef ----------------------------------------------------------*/
/* Exported struct  ----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
extern void Imu_AHRSUpdate(void);
	
#ifdef __cplusplus
}
#endif

#endif

/****************** 北京龙鼎源科技 http://www.bdrl.com.cn/ ********************/


