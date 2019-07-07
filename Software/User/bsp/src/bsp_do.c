/**
  ******************************************************************************
  * @file    bsp_do.c
  * @author  zwx
  * @version V1.1.0
  * @date    31-July-2013
  * @brief   DI
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp_do.h"


/*
*   DO1��������DO6��IO�ڷ��䣺
*
* DO1       DO2     DO3     DO4     DO5     DO6
* PC8       PG8     PC9     PA8     PA11    PA12
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
void Bsp_InitDo(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* ��GPIOʱ�� */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

    /*
         ��������DO������IO��Ϊ���ģʽ
    */

    /* PA8��PA11, PA12 */
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12; /* PA������ */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;          /* ��Ϊ����� */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;       /* �������    */
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;             /* ��������ʹ�� */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;  /* IO�ڵ��ٶ� */
    GPIO_Init(GPIOA,&GPIO_InitStructure);                    /* ��ʼ��GPIOA */
    GPIO_SetBits(GPIOA,GPIO_Pin_8 | GPIO_Pin_11| GPIO_Pin_12);  /* �ϵ�Ĭ��ʹDO�ض���� */

    /* PC8, PC9 */
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9; /* PC������ */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;          /* ��Ϊ����� */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;       /* �������    */
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;             /* ��������ʹ�� */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;  /* IO�ڵ��ٶ� */
    GPIO_Init(GPIOC,&GPIO_InitStructure);                    /* ��ʼ��GPIOC */
    GPIO_SetBits(GPIOC,GPIO_Pin_8 | GPIO_Pin_9);             /* �ϵ�Ĭ��ʹDO�ض���� */

    /* PG8 */
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;                      /* PG������ */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;          /* ��Ϊ����� */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;       /* �������    */
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;             /* ��������ʹ�� */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;  /* IO�ڵ��ٶ� */
    GPIO_Init(GPIOG,&GPIO_InitStructure);                    /* ��ʼ��GPIOG */
    GPIO_SetBits(GPIOG,GPIO_Pin_8);
}

/**
  * @brief  ��������ĳһDOͨ�������ֵ
  * @param  DOx�������õ�DOͨ����1-6
            state��DO���״̬��1������������0����ض����
  * @retval None
  */
void Bsp_SetDo(uint8_t DOx,uint8_t state)
{
    switch(DOx)
    {
        case 1:
            DO1 = ~state;
            break;
        case 2:
            DO2 = ~state;
            break;
        case 3:
            DO3 = ~state;
            break;
        case 4:
            DO4 = ~state;
            break;
        case 5:
            DO5 = ~state;
            break;
        case 6:
            DO6 = ~state;
            break;
        case 7:
            //        Send_Ext_Data(DOx-7,state);
            break;
        case 8:
            //       Send_Ext_Data(DOx-7,state);
            break;
        default:
            break;
    }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
