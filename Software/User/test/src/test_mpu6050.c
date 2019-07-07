/**
  ******************************************************************************
  * @file    test_mpu6050.c
  * @author  zwx
  * @version V1.8.0
  * @date    2019-01-29
  * @brief   
  ******************************************************************************
  */  

#include "test_mpu6050.h"
#include "bsp_mpu6050.h"
#include "srv_imu.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "srv_debug.h"
#include "bsp_uart.h"

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
void Test_Mpu6050Init(void)
{
	bsp_Mpu6050Init();
}	

void Test_SendMpu6050(void)
{
	uint8_t cnt = 0, i = 0, sum = 0;
	uint8_t buf[50] = {0};

	buf[cnt++] = 0x88;
	buf[cnt++] = 0xA1;
	buf[cnt++] = 0x00;

	buf[cnt++] = BYTE3(MPU6050.Ang.Pitch);
	buf[cnt++] = BYTE2(MPU6050.Ang.Pitch);	
	buf[cnt++] = BYTE1(MPU6050.Ang.Pitch);
	buf[cnt++] = BYTE0(MPU6050.Ang.Pitch);

	buf[cnt++] = BYTE3(MPU6050.Ang.Roll);
	buf[cnt++] = BYTE2(MPU6050.Ang.Roll);	
	buf[cnt++] = BYTE1(MPU6050.Ang.Roll);
	buf[cnt++] = BYTE0(MPU6050.Ang.Roll);
	
	buf[cnt++] = BYTE3(MPU6050.Ang.Yaw);
	buf[cnt++] = BYTE2(MPU6050.Ang.Yaw);	
	buf[cnt++] = BYTE1(MPU6050.Ang.Yaw);
	buf[cnt++] = BYTE0(MPU6050.Ang.Yaw);
	
	buf[2]= cnt-3; // 数据长度
	
	for(i=0; i<cnt; i++)
	{
		sum += buf[i];
	}
	buf[cnt++] = sum; // 校验和
				
	Uart_SendBuf(buf, cnt);		
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void App_TestMpu6050(void *pvParameters)
{
	TickType_t xLastWakeTime;
	
	Test_Mpu6050Init();
//	MPU6050_Calibrate();

	xLastWakeTime = xTaskGetTickCount();
	
	while(1)
	{
		MPU6050_ReadData();
		Imu_AHRSUpdate();	
		vTaskDelayUntil(&xLastWakeTime, 5);
	}
}
	

/****************** 北京龙鼎源科技 http://www.bdrl.com.cn/ ********************/
