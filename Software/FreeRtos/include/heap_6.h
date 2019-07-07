/**
  ******************************************************************************
  * @file    heap_6.h
  * @author  zwx
  * @version V1.8.0
  * @date    2019-01-29
  * @brief
  ******************************************************************************
  */

#ifndef __HEAP_6_H__
#define __HEAP_6_H__

#include "stm32f10x.h"
#include <stddef.h>

/* Exported define -----------------------------------------------------------*/
#define EN_SRAM_IN                  (1)
#define EN_SRAM_EX                  (1)
#define EN_SRAM_CCM                 (1)
#define COUT_OF_MEM                 (3)                 //�ж��ٿ��ڴ�

#define configTOTAL_HEAP_SIZE1      ( ( size_t ) ( 10 * 1024 ) )  //�ڲ��ڴ�ش�С
#define configTOTAL_HEAP_SIZE2      ( ( size_t ) ( 500 * 1024 ) )  //�ⲿ�ڴ�ش�С
#define configTOTAL_HEAP_SIZE3      ( ( size_t ) ( 30 * 1024 ) )   //CCM�ڴ�ش�С

/* Exported typedef ----------------------------------------------------------*/
//�ڴ�����
typedef enum
{
    SRAM_NONE   = (uint8_t)0,
#if (EN_SRAM_IN == 1)
    SRAM_IN = 0,                                             //�ڲ��ڴ��
#endif

#if (EN_SRAM_EX == 1)
    SRAM_EX,                                            //�ⲿ�ڴ��
#endif

#if (EN_SRAM_CCM == 1)
    SRAM_CCM,                                            //CCM �ڴ��(�˲��� SRAM ���� CPU ���Է���!!!)
#endif
} E_MemType;

/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void *pvMyPortMalloc(size_t xWantedSize,uint8_t eMemType);
void vMyPortFree(void *pv,uint8_t eMemType);
size_t xMyPortGetFreeHeapSize(uint8_t eMemType);
size_t xMyPortGetMinimumEverFreeHeapSize(uint8_t eMemType);
void vMyPortInitialiseBlocks(void);


#endif

/****************** ��������Դ�Ƽ� http://www.bdrl.com.cn/ ********************/

