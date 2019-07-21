/**
  ******************************************************************************
  * @file    bsp_pwm.c
  * @author  zwx
  * @version V1.0.0
  * @date    2019-07-17
  * @brief
  ******************************************************************************
  */
#include "bsp_pwm.h"

#define PWM_FRE   24000

/*********************************************************************************************************
* Function Name : void (void)
* Description   : 
*
* Inputs        : void
*
* Outputs       : void
*
* Limitations   : nothing
*********************************************************************************************************/
void bsp_pwmInit(void)
{
	uint16_t PrescalerValue = 0;
	
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  			TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) (SystemCoreClock / (PWM_FRE*1000)) - 1;
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 999;		//
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;	//pwm时钟分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//向上计数
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;//初始占空比为0
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	
	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) (SystemCoreClock / (PWM_FRE*1000)) - 1;
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 999;		//
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;	//pwm时钟分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//向上计数
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;//初始占空比为0
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_Cmd(TIM3, ENABLE);	
}

/*********************************************************************************************************
* Function Name : void (void)
* Description   : 
*
* Inputs        : void
*
* Outputs       : void
*
* Limitations   : nothing
*********************************************************************************************************/
void bsp_setPwm(u16 *pwm)
{
	uint8_t i=0;
	
	for(i=0; i<6; i++)
	{
		if(pwm[i] < 1000)
			pwm[i] = 1000;
		else if(pwm[i] > 2000)
			pwm[i] = 2000;
	}
	TIM2->CCR1 = pwm[0] - 1000;
	TIM3->CCR3 = pwm[1] - 1000;
	TIM2->CCR3 = pwm[2] - 1000;
	TIM3->CCR4 = pwm[3] - 1000;
	TIM2->CCR2 = pwm[4] - 1000;
	TIM2->CCR4 = pwm[5] - 1000;	
}







 