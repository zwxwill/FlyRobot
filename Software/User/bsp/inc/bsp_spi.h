/**
  ******************************************************************************
  * @file    bsp_spi.h
  * @author  zwx
  * @version V1.8.0
  * @date    2019-01-29
  * @brief   
  ******************************************************************************
  */  

#ifndef __BSP_SPI_H__
#define __BSP_SPI_H__

#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SPI2_CSN_HIGH()    GPIO_SetBits(GPIOB, GPIO_Pin_12)
#define SPI2_CSN_LOW()     GPIO_ResetBits(GPIOB, GPIO_Pin_12)

/* Exported define -----------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
/* Exported struct  ----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
extern void SPI2_Configuration(void);
extern uint8_t SPI2_SendByte(uint8_t aByte_i);
extern uint8_t SPI2_ReadReg(uint8_t reg);
extern void SPI2_WriteReg(uint8_t aRegAdress_i, uint8_t aWrByte_i);
extern uint8_t SPI2_WriteBuf(uint8_t reg, uint8_t *pBuf, uint8_t len);
extern uint8_t SPI2_ReadBuf(uint8_t reg, uint8_t *pBuf, uint8_t len);
	
	
	
#ifdef __cplusplus
}
#endif

#endif

/****************** 北京龙鼎源科技 http://www.bdrl.com.cn/ ********************/
