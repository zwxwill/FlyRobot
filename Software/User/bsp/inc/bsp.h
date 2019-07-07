/**
  ******************************************************************************
  * @file    bsp.h
  * @author  zwx
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Main program body
  ******************************************************************************
  */

#ifndef _BSP_H_
#define _BSP_H_


#include "stm32f10x.h"


#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/* 中断优先级 */
#define NVIC_EXIT15_PRE     5    


/* Exported functions ---------------------------------------------------------*/
void bsp_Init(void);


#endif

/************************************** end *************************************/
