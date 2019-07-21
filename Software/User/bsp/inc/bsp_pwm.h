/**
  ******************************************************************************
  * @file    bsp_pwm.h
  * @author  zwx
  * @version V1.0.0
  * @date    2019-07-17
  * @brief
  ******************************************************************************
  */

#ifndef __BSP_PWM_H__
#define __BSP_PWM_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Externs -------------------------------------------------------------------*/
void bsp_pwmInit(void);
void bsp_setPwm(u16 *pwm);


#endif

