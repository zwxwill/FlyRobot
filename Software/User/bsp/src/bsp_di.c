/**
  ******************************************************************************
  * @file    srv_di.c
  * @author  zwx
  * @version V1.1.0
  * @date    31-July-2013
  * @brief   DI
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp_di.h"

/*
*   DI1��������DI12��IO�ڷ��䣺
*
* DI1       DI2     DI3     DI4     DI5     DI6     DI7     DI8     DI9     DI10        DI11        DI12
* PC2       PC3     PA0     PA3     PA4     PA5     PA6     PB0     PB1     PF11        PG6         PG7
*
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes the DI
  * @param  None
  * @retval None
  */
void Bsp_InitDi(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* ��GPIOʱ�� */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

    /*
        ��������DI������IO��Ϊ����ģʽ
    */

    /* PC2��PC3 */
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3;       /* PC������ */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;             /* ��Ϊ����� */
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;         /* ���������費ʹ�� */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;        /* IO�ڵ��ٶ� */
    GPIO_Init(GPIOC,&GPIO_InitStructure);                    /* ��ʼ��GPIOC */

    /* PA0��PA3, PA4, PA5, PA6 */
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_3;       /* PA������ */
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;             /* ��Ϊ����� */
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;         /* ���������費ʹ�� */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;        /* IO�ڵ��ٶ� */
    GPIO_Init(GPIOA,&GPIO_InitStructure);                    /* ��ʼ��GPIOA */

    /* PB0��PB1 */
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;     /* PB������ */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;           /* ��Ϊ����� */
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;       /* ���������費ʹ�� */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;      /* IO�ڵ��ٶ� */
    GPIO_Init(GPIOB,&GPIO_InitStructure);                  /* ��ʼ��GPIOB */

    /* PF11 */
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;               /* PF������ */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;           /* ��Ϊ����� */
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;       /* ���������費ʹ�� */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;      /* IO�ڵ��ٶ� */
    GPIO_Init(GPIOF,&GPIO_InitStructure);                  /* ��ʼ��GPIOF */

    /* PG6, PG7 */
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;     /* PG������ */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;           /* ��Ϊ����� */
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;       /* ���������費ʹ�� */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;      /* IO�ڵ��ٶ� */
    GPIO_Init(GPIOG,&GPIO_InitStructure);                  /* ��ʼ��GPIOG */
}

/**
  * @brief  Get DI data 16λDIֵ��LSBΪDI1������ΪDI2-DI12��DI���źţ�ֵΪ1�����ź�ֵΪ0�����4λ��ЧΪ1
  * @param  None
  * @retval None
  */
uint16_t Bsp_GetDiData(void)
{
    uint16_t    bsp_DI_Value=0;


    return  bsp_DI_Value;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
