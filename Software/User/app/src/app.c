/**
  ******************************************************************************
  * @file    app.c
  * @author  zwx
  * @version V1.8.0
  * @date    2019-01-29
  * @brief
  ******************************************************************************
  */

#include "app.h"
#include "FreeRTOS.h"
#include "task.h"

#include "bsp_led.h"
#include "bsp_uart.h"
#include "bsp_mpu6050.h"

#include "app_led.h"
#include "app_check.h"
#include "app_test.h"

#include <stdio.h>

/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private struct  -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static TaskHandle_t xHandleTaskStart = NULL;
static TaskHandle_t xHandleTaskCheck = NULL;
static TaskHandle_t xHandleTaskLED = NULL;
static TaskHandle_t xHandleTaskTest = NULL;
/* Private functions ---------------------------------------------------------*/

/**
  * @brief
  * @param  None
  * @retval None
  */
static void App_HardWareInit(void)
{
    bsp_InitLed();      /* ����LED */
    bsp_InitUart();     /* ���Դ��ڳ�ʼ��  */
	
//	bsp_Mpu6050Init();  /* MPU6050��ʼ�� */

}


/**
  * @brief  ��ֵԽС�����ȼ�Խ��
  * @param  None
  * @retval None
  */
static void App_TaskCreate(void)
{
//    xTaskCreate(vTaskCheck,            /* function  */
//                "vTaskCheck",          /* name    */
//                256,                  /* stack size, unit: 4 byte */
//                NULL,                  /* task param */
//                2,                     /* priority */
//                &xHandleTaskCheck);    /* handle  */
	
    xTaskCreate(vTaskLED,              /* function  */
                "vTaskLED",            /* name    */
                128,                  /* stack size, unit: 4 byte */
                NULL,                  /* task param */
                3,                     /* priority */
                &xHandleTaskLED);      /* handle  */


    /* JDI ���н���ʹ�������ȼ�5 */

    xTaskCreate(vTaskTest,             /* function  */
                "vTaskTest",           /* name    */
                256,                  /* stack size, unit: 4 byte */
                NULL,                  /* task param */
                8,                     /* priority */
                &xHandleTaskTest);     /* handle  */
}

/**
  * @brief
  * @param  None
  * @retval None
  */
static void vTaskStart(void *pvParameters)
{
    App_HardWareInit();

    App_TaskCreate();

    vTaskDelete(NULL);
}

/**
  * @brief
  * @param  None
  * @retval None
  */
void app_Init(void)
{
    xTaskCreate(vTaskStart,            /* function  */
                "vTaskStart",          /* name    */
                256,                  /* stack size, unit: 4 byte */
                NULL,                  /* task param */
                configMAX_PRIORITIES-1,/* priority */
                &xHandleTaskStart);    /* handle  */
}


/****************** ��������Դ�Ƽ� http://www.bdrl.com.cn/ ********************/


