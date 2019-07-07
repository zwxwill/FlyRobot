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
        APB1 ��ʱ���� TIM2, TIM3 ,TIM4 
        APB2 ��ʱ���� TIM1
    */
    if(TIMx == TIM1)
    {
        rcc = RCC_APB2Periph_TIM1;
    }
    /* ������ APB1ʱ�� */
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

    /* ʹ��TIMʱ�� */
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
        TIM_Cmd(TIMx, DISABLE);     /* �رն�ʱ��� */
        return;
    }

    /*-----------------------------------------------------------------------
        system_stm32f10x.c �ļ��� void SetSysClockTo72(void) ������ʱ�ӵ��������£�

        HCLK = SYSCLK / 1     (AHB1Periph)
        PCLK2 = HCLK  / 1     (APB2Periph)
        PCLK1 = HCLK  / 2     (APB1Periph)

        ��ΪAPB1 prescaler != 1, ���� APB1�ϵ�TIMxCLK = PCLK1 x 2 = SystemCoreClock / 2 * 2;
        ��ΪAPB2 prescaler == 1, ���� APB2�ϵ�TIMxCLK = PCLK2     = SystemCoreClock;

        APB1 ��ʱ���� TIM2, TIM3 ,TIM4
        APB2 ��ʱ���� TIM1

    ----------------------------------------------------------------------- */
    if(TIMx == TIM1)
    {
        /* APB2 ��ʱ�� */
        uiTIMxCLK = SystemCoreClock;
    }
    else    /* APB1 ��ʱ�� */
    {
        uiTIMxCLK = SystemCoreClock / 2 * 2;
    }

    if(_ulFreq < 100)
    {
        usPrescaler = 10000 - 1;                            /* ��Ƶ�� = 1000 */
        usPeriod = (uiTIMxCLK / 10000) / _ulFreq  - 1;      /* �Զ���װ��ֵ */
    }
    else if(_ulFreq < 3000)
    {
        usPrescaler = 100 - 1;                              /* ��Ƶ�� = 100 */
        usPeriod = (uiTIMxCLK / 100) / _ulFreq  - 1;        /* �Զ���װ��ֵ */
    }
    else    /* ����4K��Ƶ�ʣ������Ƶ */
    {
        usPrescaler = 0;                    /* ��Ƶ�� = 1 */
        usPeriod = uiTIMxCLK / _ulFreq - 1; /* �Զ���װ��ֵ */
    }

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = usPeriod;
    TIM_TimeBaseStructure.TIM_Prescaler = usPrescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0000;       /* TIM1 �� TIM8 �������� */

    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);

    TIM_ARRPreloadConfig(TIMx, ENABLE);

    /* TIM Interrupts enable */
    TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);

    /* TIMx enable counter */
    TIM_Cmd(TIMx, ENABLE);

    /* ����TIM��ʱ�����ж� (Update) */
    {
        NVIC_InitTypeDef NVIC_InitStructure;    /* �жϽṹ���� misc.h �ж��� */
        uint8_t irq = 0;    /* �жϺ�, ������ stm32f4xx.h */

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


/****************** ��������Դ�Ƽ� http://www.bdrl.com.cn/ ********************/

