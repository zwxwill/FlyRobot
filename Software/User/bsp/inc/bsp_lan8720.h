/**
  ******************************************************************************
  * @file    bsp_lan8720.h
  * @author  zwx
  * @version V1.8.0
  * @date    2019-01-29
  * @brief
  ******************************************************************************
  */

#ifndef __BSP_LAN8720_H
#define __BSP_LAN8720_H


#include "stm32f4xx.h"
#include "stm32f4x7_eth.h"


/* Exported functions ---------------------------------------------------------*/
void bsp_InitLan8720(void);
u8 Lan8720_GetSpeed(void);


#endif

/************************************** end *************************************/
