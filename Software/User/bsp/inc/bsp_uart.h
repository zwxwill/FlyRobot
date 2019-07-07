/**
  ******************************************************************************
  * @file    bsp_led.h
  * @author  zwx
  * @version V1.8.0
  * @date    2019-01-29
  * @brief
  ******************************************************************************
  */

#ifndef __BSP_UART_H__
#define __BSP_UART_H__

#include "stm32f10x.h"
#include <stdio.h>

/* PLC usart param */

/* Exported functions ---------------------------------------------------------*/
void bsp_InitUart(void);
void Uart_SendBuf(u8 *ch, u8 num);

#endif

/************************************** end *************************************/
