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
#include "srv_sensor.h"
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
void Test_SendMpu6050(void)
{
	uint8_t cnt = 0, i = 0, sum = 0;
	uint8_t buf[50] = {0};
	int16_t tmp_i16;
	float   tmp_f;

	buf[cnt++] = 0xAA; /* 帧头 0xAA */
	buf[cnt++] = 0xAA; /* 帧头 0xAA */
	buf[cnt++] = 0xF1; /* 功能码 0xF1 */
	buf[cnt++] = 0x00; /* Len */

	tmp_i16 = Mpu6050.GyroRaw.x;
	buf[cnt++] = BYTE1(tmp_i16);
	buf[cnt++] = BYTE0(tmp_i16);	

	tmp_i16 = Mpu6050.GyroRaw.y;
	buf[cnt++] = BYTE1(tmp_i16);
	buf[cnt++] = BYTE0(tmp_i16);
	
	tmp_i16 = Mpu6050.GyroRaw.z;
	buf[cnt++] = BYTE1(tmp_i16);
	buf[cnt++] = BYTE0(tmp_i16);	
	
	tmp_f = Mpu6050.Gyro.x;
	buf[cnt++] = BYTE3(tmp_f);
	buf[cnt++] = BYTE2(tmp_f);
	buf[cnt++] = BYTE1(tmp_f);
	buf[cnt++] = BYTE0(tmp_f);	

	tmp_f = Mpu6050.Gyro.y;
	buf[cnt++] = BYTE3(tmp_f);
	buf[cnt++] = BYTE2(tmp_f);
	buf[cnt++] = BYTE1(tmp_f);
	buf[cnt++] = BYTE0(tmp_f);
	
	tmp_f = Mpu6050.Gyro.z;
	buf[cnt++] = BYTE3(tmp_f);
	buf[cnt++] = BYTE2(tmp_f);
	buf[cnt++] = BYTE1(tmp_f);
	buf[cnt++] = BYTE0(tmp_f);
	

	
	buf[3]= cnt-4; // 数据长度
	
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
	
//	MPU6050_Calibrate();

//	xLastWakeTime = xTaskGetTickCount();
	
	while(1)
	{
//		MPU6050_ReadData();
//		Imu_AHRSUpdate();
		ProcessAccGyroMeasurements();
		Test_SendMpu6050();
		vTaskDelay(5);
	//	vTaskDelayUntil(&xLastWakeTime, 5);
	}
}
	

/****************** 北京龙鼎源科技 http://www.bdrl.com.cn/ ********************/
