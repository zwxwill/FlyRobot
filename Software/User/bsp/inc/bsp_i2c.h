/**
  ******************************************************************************
  * @file    bsp_i2c.h
  * @author  zwx
  * @version V1.8.0
  * @date    2019-01-29
  * @brief   
  ******************************************************************************
  */  

#ifndef __BSP_I2C_H__
#define __BSP_I2C_H__

#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported define -----------------------------------------------------------*/
#define I2C1_ACK			 (0u)
#define I2C1_NO_ACK			 (1u)
#define I2C1_WRITE_ERROR     (1u)
#define I2C1_WRITE_OK        (0u)
#define I2C1_READ_ERROR      (1u)
#define I2C1_READ_OK         (0u)

#define I2C1_SCL_H()         GPIOB->BSRR = GPIO_Pin_6
#define I2C1_SCL_L()         GPIOB->BRR = GPIO_Pin_6
#define I2C1_SDA_H()         GPIOB->BSRR = GPIO_Pin_7
#define I2C1_SDA_L()         GPIOB->BRR = GPIO_Pin_7
/* Exported typedef ----------------------------------------------------------*/
/* Exported struct  ----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
extern void I2C1_Configuration(void);
extern u8 I2C1_WriteData(u8 dev_addr, u8 reg_addr, u8 count, u8 *pdata);
extern u8 I2C1_ReadData(u8 dev_addr, u8 reg_addr, u8 count, u8 *pdata);
extern u8 I2C1_WriteOneData(u8 dev_addr, u8 reg_addr, u8 data);
extern u8 I2C1_ReadOneData(u8 dev_addr, u8 reg_addr, u8 *data);
extern u8 I2C1_WriteOneDataBit(u8 dev_addr, u8 reg_addr, u8 bitNum, u8 data);
	
#ifdef __cplusplus
}
#endif

#endif

/****************** 北京龙鼎源科技 http://www.bdrl.com.cn/ ********************/

