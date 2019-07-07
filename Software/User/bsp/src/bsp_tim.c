/**
  ******************************************************************************
  * @file    bsp_tim.c
  * @author  zwx
  * @version V1.8.0
  * @date    2019-01-29
  * @brief
  ******************************************************************************
  */

#include "bsp_tim.h"


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
uint32_t bsp_GetRCCofTIM(TIM_TypeDef* TIMx)
{
    uint32_t rcc = 0;

    /*
        APB1 定时器有 TIM2, TIM3 ,TIM4 
        APB2 定时器有 TIM1
    */
    if(TIMx == TIM1)
    {
        rcc = RCC_APB2Periph_TIM1;
    }
    /* 下面是 APB1时钟 */
    else if(TIMx == TIM2)
    {
        rcc = RCC_APB1Periph_TIM2;
    }
    else if(TIMx == TIM3)
    {
        rcc = RCC_APB1Periph_TIM3;
    }
    else if(TIMx == TIM4)
    {
        rcc = RCC_APB1Periph_TIM4;
    }


    return rcc;
}

/**
  * @brief
  * @param  None
  * @retval None
  */
void bsp_TimInit(TIM_TypeDef* TIMx, uint32_t _ulFreq, uint8_t _PreemptionPriority, uint8_t _SubPriority)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    uint16_t usPeriod;
    uint16_t usPrescaler;
    uint32_t uiTIMxCLK;

    /* 使能TIM时钟 */
    if(TIMx == TIM1)
    {
        RCC_APB2PeriphClockCmd(bsp_GetRCCofTIM(TIMx), ENABLE);
    }
    else
    {
        RCC_APB1PeriphClockCmd(bsp_GetRCCofTIM(TIMx), ENABLE);
    }

    if(_ulFreq == 0)
    {
        TIM_Cmd(TIMx, DISABLE);     /* 关闭定时输出 */
        return;
    }

    /*-----------------------------------------------------------------------
        system_stm32f10x.c 文件中 void SetSysClockTo72(void) 函数对时钟的配置如下：

        HCLK = SYSCLK / 1     (AHB1Periph)
        PCLK2 = HCLK  / 1     (APB2Periph)
        PCLK1 = HCLK  / 2     (APB1Periph)

        因为APB1 prescaler != 1, 所以 APB1上的TIMxCLK = PCLK1 x 2 = SystemCoreClock / 2 * 2;
        因为APB2 prescaler == 1, 所以 APB2上的TIMxCLK = PCLK2     = SystemCoreClock;

        APB1 定时器有 TIM2, TIM3 ,TIM4
        APB2 定时器有 TIM1

    ----------------------------------------------------------------------- */
    if(TIMx == TIM1)
    {
        /* APB2 定时器 */
        uiTIMxCLK = SystemCoreClock;
    }
    else    /* APB1 定时器 */
    {
        uiTIMxCLK = SystemCoreClock / 2 * 2;
    }

    if(_ulFreq < 100)
    {
        usPrescaler = 10000 - 1;                            /* 分频比 = 1000 */
        usPeriod = (uiTIMxCLK / 10000) / _ulFreq  - 1;      /* 自动重装的值 */
    }
    else if(_ulFreq < 3000)
    {
        usPrescaler = 100 - 1;                              /* 分频比 = 100 */
        usPeriod = (uiTIMxCLK / 100) / _ulFreq  - 1;        /* 自动重装的值 */
    }
    else    /* 大于4K的频率，无需分频 */
    {
        usPrescaler = 0;                    /* 分频比 = 1 */
        usPeriod = uiTIMxCLK / _ulFreq - 1; /* 自动重装的值 */
    }

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = usPeriod;
    TIM_TimeBaseStructure.TIM_Prescaler = usPrescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0000;       /* TIM1 和 TIM8 必须设置 */

    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);

    TIM_ARRPreloadConfig(TIMx, ENABLE);

    /* TIM Interrupts enable */
    TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);

    /* TIMx enable counter */
    TIM_Cmd(TIMx, ENABLE);

    /* 配置TIM定时更新中断 (Update) */
    {
        NVIC_InitTypeDef NVIC_InitStructure;    /* 中断结构体在 misc.h 中定义 */
        uint8_t irq = 0;    /* 中断号, 定义在 stm32f4xx.h */

        if((TIMx == TIM1) || (TIMx == TIM10))
        {
            irq = TIM1_UP_IRQn;
        }
        else if(TIMx == TIM2)
        {
            irq = TIM2_IRQn;
        }
        else if(TIMx == TIM3)
        {
            irq = TIM3_IRQn;
        }
        else if(TIMx == TIM4)
        {
            irq = TIM4_IRQn;
        }

        NVIC_InitStructure.NVIC_IRQChannel = irq;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = _PreemptionPriority;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = _SubPriority;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }
}

/**
  * @brief
  * @param  None
  * @retval None
  */
void bsp_TimClose(TIM_TypeDef* TIMx)
{
    TIM_Cmd(TIMx, DISABLE);
    TIM_ITConfig(TIMx, TIM_IT_Update, DISABLE);
}


/****************** 北京龙鼎源科技 http://www.bdrl.com.cn/ ********************/

