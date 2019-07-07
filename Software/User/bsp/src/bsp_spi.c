/**
  ******************************************************************************
  * @file    bsp_spi.c
  * @author  zwx
  * @version V1.8.0
  * @date    2019-01-29
  * @brief   
  ******************************************************************************
  */  

#include "bsp_spi.h"


/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private struct  -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
// Spi Communication Over Time Config Value 
#define SPI2_WR_TIMEOUT                (2000u)
#define SPI2_NULL					   (0u)

/*********************************************************************************************************
* Function Name : void (void)
* Description   : 
*
* Inputs        : void
*
* Outputs       : void
*
* Limitations   : PB13:SPI2_SCK        PB15:SPI2_MOSI      PB14:SPI2_MISO    PB12:NRF2401_CSN
*                 
*********************************************************************************************************/
void SPI2_Configuration(void)
{
	SPI_InitTypeDef SPI_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	 
	/*配置 SPI_NRF_SPI的 SCK,MISO,MOSI引脚 */ 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用功能 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;            // CSN
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	SPI2_CSN_HIGH();
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //双线全双工 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //主模式 
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //数据大小8位 
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; //时钟极性，空闲时为低 
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; //第1个边沿有效，上升沿为采样时刻 
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //NSS信号由软件产生 
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; //4分频，9MHz 
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //高位在前 
	SPI_InitStructure.SPI_CRCPolynomial = 7; 
	SPI_Init(SPI2, &SPI_InitStructure); 
	/* Enable SPI2 */ 
	SPI_Cmd(SPI2, ENABLE);
}

/*********************************************************************************************************
* Function Name : void (void)
* Description   : Send and Receive a byte from the SPI
*
* Inputs        : void
*
* Outputs       : void
*
* Limitations   : nothing
*********************************************************************************************************/
uint8_t SPI2_SendByte(uint8_t aByte_i)
{
    uint8_t  Temp = 0u;
    uint32_t WaitCounter = 0u;

    while((SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) && (WaitCounter < SPI2_WR_TIMEOUT))
    {
        WaitCounter++;
    }
    // Send byte through the SPI2 peripheral
    (void)SPI_I2S_ReceiveData(SPI2);
    SPI_I2S_SendData(SPI2, aByte_i);
    
    // Wait to receive a byte 
    WaitCounter = 0u;
    while((SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) && (WaitCounter < SPI2_WR_TIMEOUT))
    {
        WaitCounter++;
    } 
    Temp = (uint8_t)SPI_I2S_ReceiveData(SPI2);
    
    // Return the byte read from the SPI bus 
    return Temp;	
}

/*********************************************************************************************************
* Function Name : void (void)
* Description   : 
*
* Inputs        : void
*
* Outputs       : void
*
* Limitations   : 
*          bit 0: RW bit. When 0, the data DI(7:0) is written into the device. When 1, the data DO(7:0)
*                 from the device is read. In latter case, the chip will drive SDO at the start of bit 8.
*          bit 1: MS bit. When 0, the address will remain unchanged in multiple read/write commands.
*                 When 1, the address is auto incremented in multiple read/write commands.
*********************************************************************************************************/
uint8_t SPI2_ReadReg(uint8_t reg)
{
	uint8_t Temp = 0u;

	SPI2_CSN_LOW();
	(void)SPI2_SendByte(reg); // 0x80 for Read data from the device 
	Temp = SPI2_SendByte(0xffu);
	SPI2_CSN_HIGH();

	return Temp;	
}

/*********************************************************************************************************
* Function Name : void (void)
* Description   : 
*
* Inputs        : void
*
* Outputs       : void
*
* Limitations   : 
*          bit 0: RW bit. When 0, the data DI(7:0) is written into the device. When 1, the data DO(7:0)
*                 from the device is read. In latter case, the chip will drive SDO at the start of bit 8.
*          bit 1: MS bit. When 0, the address will remain unchanged in multiple read/write commands.
*                 When 1, the address is auto incremented in multiple read/write commands.
*********************************************************************************************************/
void SPI2_WriteReg(uint8_t aRegAdress_i, uint8_t aWrByte_i)
{
	SPI2_CSN_LOW();
	(void)SPI2_SendByte(aRegAdress_i); //  0x00: for write data to the device
	(void)SPI2_SendByte(aWrByte_i);
	SPI2_CSN_HIGH();
}

/*********************************************************************************************************
* Function Name : void (void)
* Description   : 
*
* Inputs        : void
*
* Outputs       : void
*
* Limitations   : nothing
*********************************************************************************************************/
uint8_t SPI2_WriteBuf(uint8_t reg, uint8_t *pBuf, uint8_t len)
{
	uint8_t i = 0;
	uint8_t status = 0;
	
	SPI2_CSN_LOW();
	status = SPI2_SendByte(reg);
	for(i=0; i<len; i++)
	{
		SPI2_SendByte(pBuf[i]);
	}
	SPI2_CSN_HIGH();
	return status;
}

/*********************************************************************************************************
* Function Name : void (void)
* Description   : 
*
* Inputs        : void
*
* Outputs       : void
*
* Limitations   : nothing
*********************************************************************************************************/
uint8_t SPI2_ReadBuf(uint8_t reg, uint8_t *pBuf, uint8_t len)
{
	uint8_t i = 0;
	uint8_t status = 0;
	
	SPI2_CSN_LOW();
	status = SPI2_SendByte(reg);
	for(i=0; i<len; i++)
	{
		pBuf[i] = SPI2_SendByte(SPI2_NULL);
	}
	SPI2_CSN_HIGH();
	return status;
}


/****************** 北京龙鼎源科技 http://www.bdrl.com.cn/ ********************/
