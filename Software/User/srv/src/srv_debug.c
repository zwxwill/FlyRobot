/**
  ******************************************************************************
  * @file    srv_debug.c
  * @author  zwx
  * @version V1.8.0
  * @date    2019-01-29
  * @brief   
  ******************************************************************************
  */  

#include "srv_debug.h"
#include "bsp_uart.h"
#include "srv_sensor.h"

/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private struct  -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


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
uint8_t Srv_SendToUsart(void)
{
	uint8_t cnt = 0, i = 0, sum = 0;
	uint8_t buf[50] = {0};
	float tmp = 0;

	buf[cnt++] = 0x88;
	buf[cnt++] = 0xA1;
	buf[cnt++] = 0x00;


	tmp = Mpu6050.AccRaw.x;
	buf[cnt++] = BYTE1(tmp);
	buf[cnt++] = BYTE0(tmp);	


	buf[2]= cnt-3;    // 数据长度
	
	for(i=0; i<cnt; i++)
	{
		sum += buf[i];
	}
	buf[cnt++] = sum; // 校验和
				
	Uart_SendBuf(buf, cnt);	
}




/****************** 北京龙鼎源科技 http://www.bdrl.com.cn/ ********************/
