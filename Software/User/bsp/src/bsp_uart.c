/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  zwx
  * @version V1.8.0
  * @date    2019-01-29
  * @brief
  ******************************************************************************
  */


#include "bsp_uart.h"

/* Export variables ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define UART1_BAUD          256000//115200
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  InitHardUart
  * @param  None
  * @retval None
  */
void bsp_InitUart(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
    /* ���� USART Tx Ϊ���ù��� */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    /* ����ģʽ */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* ���� USART Rx Ϊ���ù��� */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* ��2���� ���ô���Ӳ������ */
    USART_InitStructure.USART_BaudRate = UART1_BAUD;    /* ������ */
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    USART_Cmd(USART1, ENABLE);      /* ʹ�ܴ��� */

    USART_ClearFlag(USART1, USART_FLAG_TC);     /* �巢����ɱ�־��Transmission Complete flag */
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
int fputc(int ch, FILE *f)
{
    /* �ȴ����ͽ��� */
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

    /* дһ���ֽڵ�USART1 */
    USART_SendData(USART1, (uint8_t) ch);

    return ch;
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void Uart_SendBuf(u8 *ch, u8 num)
{
	u8 i = 0;
	
	for(i=0; i<num; i++)
	{
		/* �ȴ����ͽ��� */
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

		/* дһ���ֽڵ�USART1 */
		USART_SendData(USART1, (uint8_t) ch[i]);
	}
}








/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
