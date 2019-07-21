/**
  ******************************************************************************
  * @file    bsp_motor.c
  * @author  zwx
  * @version V1.0.0
  * @date    2019-07-17
  * @brief
  ******************************************************************************
  */
#include "bsp_motor.h"
#include "bsp_tim_pwm.h"

/*匿名小六轴PWM引脚定义
	PWM1 = TIM2_CH1 PA0
	PWM2 = TIM3_CH3 PB0
	PWM3 = TIM2_CH3 PA2
	PWM4 = TIM3_CH4 PB1
	PWM5 = TIM2_CH2 PA1
	PWM6 = TIM2_CH4 PA3
*/

/*********************************************************************************************************
* Function Name : void (void)
* Description   : motor control frequency : 24KHz
*                 motor speed duty cycle : 0-1000
* Inputs        : void
*
* Outputs       : void
*
* Limitations   : nothing
*********************************************************************************************************/
void bsp_motorInit(void)
{
	bsp_SetTIMOutPWM(GPIOA, GPIO_Pin_0, TIM2, 1, 24000, 500);
	bsp_SetTIMOutPWM(GPIOB, GPIO_Pin_0, TIM3, 3, 24000, 200);
	bsp_SetTIMOutPWM(GPIOA, GPIO_Pin_2, TIM2, 3, 24000, 200);
	bsp_SetTIMOutPWM(GPIOB, GPIO_Pin_1, TIM3, 4, 24000, 200);
	bsp_SetTIMOutPWM(GPIOA, GPIO_Pin_1, TIM2, 2, 24000, 200);
	bsp_SetTIMOutPWM(GPIOA, GPIO_Pin_3, TIM2, 4, 24000, 200);
}








