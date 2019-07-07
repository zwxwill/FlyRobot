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
#define IMU_LOOP_TIME		5000.0f		//��λΪuS
#define ACC_LPF_CUT 		10.0f		//���ٶȵ�ͨ�˲�����ֹƵ��10Hz
/* Exported typedef ----------------------------------------------------------*/
/* Exported struct  ----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
extern void Imu_AHRSUpdate(void);
	
#ifdef __cplusplus
}
#endif

#endif

/****************** ��������Դ�Ƽ� http://www.bdrl.com.cn/ ********************/


