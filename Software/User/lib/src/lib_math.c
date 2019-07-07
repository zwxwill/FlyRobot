/**
  ******************************************************************************
  * @file    lib_math.c
  * @author  zwx
  * @version V1.8.0
  * @date    2019-01-29
  * @brief   
  ******************************************************************************
  */  

#include "lib_math.h"
#include <math.h>

/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private struct  -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define M_PI 			3.141592653f
#define DEG_TO_RAD 		0.01745329f    // 3.1415926  180
#define RAD_TO_DEG 		57.29577951f
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/*********************************************************************************************************
* Function Name : void (void)
* Description   : 
*
* Inputs        : void
*
* Outputs       : void
*
* Limitations   : PE12:POWER_SWITCH  PG10:LED	
*********************************************************************************************************/
float Math_DegToRag(float deg)
{
	return (deg * DEG_TO_RAD);
}

/*********************************************************************************************************
* Function Name : void (void)
* Description   : 
*
* Inputs        : void
*
* Outputs       : void
*
* Limitations   : PE12:POWER_SWITCH  PG10:LED	
*********************************************************************************************************/
float Math_RegToDeg(float reg)
{
	return (reg * RAD_TO_DEG);
}

/*********************************************************************************************************
* Function Name : void (void)
* Description   : 
*
* Inputs        : void
*
* Outputs       : void
*
* Limitations   : 
*********************************************************************************************************/
float Math_Get_LPF_1st_Factor(float deltaT, float Fcut)
{
	return deltaT / (deltaT + 1 / (2 * M_PI * Fcut));
}

/*********************************************************************************************************
* Function Name : void (void)
* Description   : 
*
* Inputs        : void
*
* Outputs       : void
*
* Limitations   : 
*********************************************************************************************************/
float Math_LPF_1st(float oldData, float newData, float lpf_factor)
{
	return (oldData * (1 - lpf_factor) + newData * lpf_factor);
}

/*********************************************************************************************************
* Function Name : void (void)
* Description   : 
*
* Inputs        : void
*
* Outputs       : void
*
* Limitations   : 快速运算 1/sqrt(x)
*********************************************************************************************************/
float Math_InvSqrt(float x)
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

/*********************************************************************************************************
* Function Name : void (void)
* Description   : 
*
* Inputs        : void
*
* Outputs       : void
*
* Limitations   : 
*********************************************************************************************************/
uint16_t Math_Limit(uint16_t amt, uint16_t low, uint16_t high)
{
	return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}


/****************** 北京龙鼎源科技 http://www.bdrl.com.cn/ ********************/
