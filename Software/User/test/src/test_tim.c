/**
  ******************************************************************************
  * @file    test_tim.c
  * @author  zwx
  * @version V1.8.0
  * @date    2019-01-29
  * @brief   
  ******************************************************************************
  */  

#include "test_tim.h"
#include "bsp_tim.h"
#include "bsp_led.h"
#include "bsp_tim_pwm.h"

/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private struct  -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  
  * @param  None
  * @retval None
  */
void Test_TimInit(void)
{
//	bsp_TimInit(TIM4, 1000, 6, 0);
//	bsp_SetTIMOutPWM(GPIOA, GPIO_Pin_0, TIM2, 1, 24000, 5000);
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void TIM1_UP_IRQHandler(void)
{
	static int i = 0;
	
	if(TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
		if(i++ >= 500)
		{
			i = 0;
			bsp_LedToggle(1);
		}
	}
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
	static int i = 0;
	
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		if(i++ >= 500)
		{
			i = 0;
			bsp_LedToggle(1);
		}
	}
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
	static int i = 0;
	
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		if(i++ >= 500)
		{
			i = 0;
			bsp_LedToggle(1);
		}
	}
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void TIM4_IRQHandler(void)
{
	static int i = 0;
	
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		if(i++ >= 500)
		{
			i = 0;
			bsp_LedToggle(1);
		}
	}
}


/****************** 北京龙鼎源科技 http://www.bdrl.com.cn/ ********************/

