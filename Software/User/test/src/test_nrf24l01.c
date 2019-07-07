/**
  ******************************************************************************
  * @file    test_nrf24l01.c
  * @author  zwx
  * @version V1.8.0
  * @date    2019-01-29
  * @brief   
  ******************************************************************************
  */  

#include "test_nrf24l01.h"
#include "bsp_nrf24l01.h"
#include "bsp_spi.h"

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


/* Exported variables --------------------------------------------------------*/
extern QueueHandle_t xQueueNrfMsg;
/* Exported functions --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private struct  -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void Test_NRF24L01TxWithPoll(void);
static void Test_NRF24L01TxWithIrq(void);
static void Test_NRF24L01RxWithIrq(void);
static void Test_NRF24L01RxWithPoll(void);

/**
  * @brief  
  * @param  None
  * @retval None
  */
void App_TestNRF24L01Tx(void *pvParameters)
{
	printf("Test NRF24L01 Tx\r\n");
	Test_NRF24L01TxWithIrq();
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void App_TestNRF24L01Rx(void *pvParameters)
{
	printf("Test NRF24L01 Rx\r\n");
	Test_NRF24L01RxWithIrq();
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
static void Test_NRF24L01TxWithIrq(void)
{
	u8 txbuf[DATA_LENGTH] = {1,2,3,4,5,6,7,8,9,10};
	
	NRF2401_Init();
	NRF2401_SetEnHancedTxMode();
	
	while(1)
	{
		NRF2401_TxPacket(txbuf, DATA_LENGTH);
		vTaskDelay(1000);
	}
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
static void Test_NRF24L01RxWithIrq(void)
{
	int i = 0;
	BaseType_t xResult;
	T_NrfRxMsg *pRxMsg;
	const TickType_t xMaxBlockTime = portMAX_DELAY; 
	
	NRF2401_Init();
//	NRF2401_SetEnHancedRxMode();
	NRF2401_SetEnHancedDynRxMode();	
	
	while(1)
	{
		xResult = xQueueReceive(xQueueNrfMsg, (void *)&pRxMsg,  (TickType_t)xMaxBlockTime);        		
		if(xResult == pdPASS)		
		{
			printf("recv:");
			for(i=0; i<pRxMsg->len; i++)
			{			
				printf(" %d", pRxMsg->buf[i]);
			}
			printf("\r\n");			
		}
	}
	
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
static void Test_NRF24L01TxWithPoll(void)
{
	u8 status = 0;
	u8 txbuf[DATA_LENGTH] = {1,2,3,4,5,6,7,8,9,10};
	
	NRF2401_Init();
	NRF2401_SetEnHancedTxMode();
	
	while(1)
	{
		NRF2401_TxPacket(txbuf, DATA_LENGTH);
		status = SPI2_ReadReg(NRF_READ_REG + NRFRegSTATUS);
		SPI2_WriteReg(NRF_WRITE_REG + NRFRegSTATUS, status);		
		vTaskDelay(1000);
	}
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
static void Test_NRF24L01RxWithPoll(void)
{
	u8 i = 0;
	u8 status = 0;
	u8 rxbuf[10] = {0};
	u8 len = 10;
	
	NRF2401_Init();
	NRF2401_SetEnHancedRxMode();	
	
	while(1)
	{
		status = SPI2_ReadReg(NRF_READ_REG + NRFRegSTATUS);
		if(status & RX_DR)    //  不知道这里是不知达到指定的发送位数才触发中断
		{
			SPI2_WriteReg(NRF_WRITE_REG + NRFRegSTATUS, RX_DR);
			SPI2_ReadBuf(RD_RX_PLOAD, rxbuf, len);//读取数据
			printf("recv:");
			for(i=0; i<DATA_LENGTH; i++)
			{				
				printf(" %d", rxbuf[i]);
			}
			printf("\r\n");
			SPI2_WriteReg(FLUSH_RX, NOP);    // 清除RX FIFO寄存器 
											 // 通知APP接收到数据
		}
		if(status & MAX_RT) /* 达到最大重发次数 */
		{	
			printf("max_rt!\r\n");
			SPI2_WriteReg(NRF_WRITE_REG + NRFRegSTATUS, MAX_RT); /*清除TX_DS或MAX_RT中断标志*/
			SPI2_WriteReg(FLUSH_TX, NOP);      /* 清除TX FIFO寄存器 */
		}
		if(status & TX_DS)/* 发送完成 */
		{
			printf("tx finish\r\n");
			SPI2_WriteReg(NRF_WRITE_REG+NRFRegSTATUS, TX_DS); /*清除TX_DS或MAX_RT中断标志*/ 
			SPI2_WriteReg(FLUSH_TX, NOP);      /* 清除TX FIFO寄存器 */		
		}

		vTaskDelay(100);
	}
	
}

/****************** 北京龙鼎源科技 http://www.bdrl.com.cn/ ********************/


