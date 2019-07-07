/**
  ******************************************************************************
  * @file    lib_math.h
  * @author  zwx
  * @version V1.8.0
  * @date    2019-01-29
  * @brief   
  ******************************************************************************
  */  

#ifndef __LIB_MATH_H__
#define __LIB_MATH_H__

#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported define -----------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
/* Exported struct  ----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
extern float Math_DegToRag(float deg);
extern float Math_Get_LPF_1st_Factor(float deltaT, float Fcut);
extern float Math_LPF_1st(float oldData, float newData, float lpf_factor);
extern float Math_Vector3Lenth(float a, float b, float c);
extern float Math_Vector4Lenth(float a, float b, float c, float d);
extern float Math_InvSqrt(float x);
extern float Math_RegToDeg(float reg);
extern uint16_t Math_Limit(uint16_t amt, uint16_t low, uint16_t high);

#ifdef __cplusplus
}
#endif

#endif

/****************** 北京龙鼎源科技 http://www.bdrl.com.cn/ ********************/


