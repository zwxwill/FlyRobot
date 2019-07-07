/**
  ******************************************************************************
  * @file    test_tcp.h
  * @author  zwx
  * @version V1.8.0
  * @date    2019-01-29
  * @brief   
  ******************************************************************************
  */  

#ifndef __TEST_TCP_H__
#define __TEST_TCP_H__

#include <stdint.h>
#include "stm32f4xx.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported define -----------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
/* Exported struct  ----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void test_tcp_client(void);
void test_tcp_client_nob(void);
void test_tcp_server(void);	
extern void vTaskTestTcpClient(void *pvParameters);
extern void vTaskTestTcpServer(void *pvParameters);	
	
#ifdef __cplusplus
}
#endif

#endif

/****************** ��������Դ�Ƽ� http://www.bdrl.com.cn/ ********************/


