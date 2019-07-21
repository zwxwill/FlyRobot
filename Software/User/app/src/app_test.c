/**
  ******************************************************************************
  * @file    app_test.c
  * @author  zwx
  * @version V1.8.0
  * @date    2019-01-29
  * @brief
  ******************************************************************************
  */

#include "app_test.h"

#include "FreeRTOS.h"
#include "task.h"

#include "test_tim.h"
#include "test_nrf24l01.h"
#include "test_mpu6050.h"

/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private struct  -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static TaskHandle_t xHandleTaskNRFRx = NULL;
static TaskHandle_t xHandleTaskNRFTx = NULL;
static TaskHandle_t xHandleTaskMpu = NULL;
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  vTaskHost
  * @param  None
  * @retval None
  */
void vTaskTest(void *pvParameters)
{
//	Test_TimInit();
	
//    xTaskCreate(App_TestNRF24L01Tx,    /* function  */
//                "vTaskTx",             /* name    */
//                256,                   /* stack size, unit: 4 byte */
//                NULL,                  /* task param */
//                4,                     /* priority */
//                &xHandleTaskNRFRx);    /* handle  */

//    xTaskCreate(App_TestNRF24L01Rx,    /* function  */
//                "vTaskRx",             /* name    */
//                256,                   /* stack size, unit: 4 byte */
//                NULL,                  /* task param */
//                5,                     /* priority */
//                &xHandleTaskNRFTx);    /* handle  */
	
    xTaskCreate(App_TestMpu6050,       /* function  */
                "TestMpu6050",         /* name    */
                256,                   /* stack size, unit: 4 byte */
                NULL,                  /* task param */
                5,                     /* priority */
                &xHandleTaskMpu);      /* handle  */	
	
    while(1)
    {
//		Test_SendMpu6050();
        vTaskDelay(50);
    }
}



/****************** 北京龙鼎源科技 http://www.bdrl.com.cn/ ********************/


