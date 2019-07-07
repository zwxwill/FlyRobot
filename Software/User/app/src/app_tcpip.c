/**
  ******************************************************************************
  * @file    app_tcpip.c
  * @author  zwx
  * @version V1.0.0
  * @date    01/20/2019
  * @brief
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "app_tcpip.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

#include "srv_netconf.h"

/* Export variables ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  vTaskTCPIP
  * @param  None
  * @retval None
  */
void vTaskTCPIP(void *pvParameters)
{
    while(1)
    {
        vTaskDelay(500);
    }
}


