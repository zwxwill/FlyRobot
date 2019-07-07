/**
  ******************************************************************************
  * @file    srv_debug.h
  * @author  zwx
  * @version V1.8.0
  * @date    2019-01-29
  * @brief   
  ******************************************************************************
  */  

#ifndef __SRV_DEBUG_H__
#define __SRV_DEBUG_H__

#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported define -----------------------------------------------------------*/
#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))
/* Exported typedef ----------------------------------------------------------*/
/* Exported struct  ----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif

/****************** 北京龙鼎源科技 http://www.bdrl.com.cn/ ********************/
