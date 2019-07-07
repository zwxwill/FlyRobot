/**
  ******************************************************************************
  * @file    bsp_i2c.c
  * @author  zwx
  * @version V1.8.0
  * @date    2019-01-29
  * @brief   
  ******************************************************************************
  */  

#include "bsp_i2c.h"


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
* Description   : 
*
* Inputs        : void
*
* Outputs       : void
*
* Limitations   : nothing
*********************************************************************************************************/
void I2C1_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;               // I2C1 SCL
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;               // I2C1_SDA
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);			
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
void I2C1_Delay(u32 n)
{
	u8 i = 0, j = 0;
	
	while(j < n)
	{
		j++;
		while(i < 5) 
		{ 
			i++; 
		} 		
	}
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
void I2C1_SDA_Out(void)
{
	GPIOB->CRL &= 0X0FFFFFFF;
	GPIOB->CRL |= 0x70000000;
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
void I2C1_SDA_In(void)
{
	GPIOB->CRL &= 0X0FFFFFFF;
	GPIOB->CRL |= 0x80000000;	
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
u8 I2C1_SDA_Read(void)
{
	return (GPIOB->IDR &=  GPIO_Pin_7) != 0 ? 1:0;
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
void I2C1_Start(void)
{
	I2C1_SDA_Out();   //Set SDA as Out Mode
	I2C1_SDA_H();
	I2C1_SCL_H();
	I2C1_Delay(4);
	I2C1_SDA_L();
	I2C1_Delay(4);
	I2C1_SCL_L();
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
void I2C1_Stop(void)
{
	I2C1_SDA_Out();   //Set SDA as Out Mode
	I2C1_SCL_L();
	I2C1_SDA_L(); 
	I2C1_Delay(4);
	I2C1_SCL_H();
	I2C1_SDA_H();
	I2C1_Delay(4); 
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
void I2C1_Ack(void)
{ 
	I2C1_SCL_L();
	I2C1_SDA_Out();  
	I2C1_SDA_L();
	I2C1_Delay(2);
	I2C1_SCL_H();
	I2C1_Delay(2);
	I2C1_SCL_L();
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
void I2C1_NoAck(void)
{
	I2C1_SCL_L();
	I2C1_SDA_Out();   //Set SDA as Out Mode
	I2C1_SDA_H();
	I2C1_Delay(2);
	I2C1_SCL_H();
	I2C1_Delay(2);
	I2C1_SCL_L();
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
u8 I2C1_WaitAck(void)
{
	u16 Out_Time = 50; 
	
	I2C1_SDA_In();
	I2C1_SDA_H();  
	I2C1_Delay(1);	
	I2C1_SCL_H();  
	I2C1_Delay(1); 
	while(I2C1_SDA_Read())   // When Low Then Get ACK
	{
		if(--Out_Time)
		{     
			I2C1_Stop();
			return I2C1_NO_ACK;
		}      
	}
	I2C1_SCL_L();
	return I2C1_ACK;
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
void I2C1_SendByte(u8 Temp)
{
	u8 i = 0;
	
	I2C1_SDA_Out();   //Set SDA as Out Mode
	I2C1_SCL_L();
	for(i=0; i<8; i++) // High byte aHead
	{
		if((Temp & 0x80) != 0)
		{
			I2C1_SDA_H();
		}
		else
		{
			I2C1_SDA_L();
		}
		Temp <<= 1;
		I2C1_Delay(2);
		I2C1_SCL_H();
		I2C1_Delay(2);
		I2C1_SCL_L();
		I2C1_Delay(2);
	}
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
u8 I2C1_ReadByte(u8 ack)
{
	u8 i=0 ,Temp=0;
	
	I2C1_SDA_In();     // Set SDA as Read Mode
	for(i=0; i<8; i++)
	{
		I2C1_SCL_L();
		I2C1_Delay(2);
		I2C1_SCL_H();
		Temp <<= 1;
		if(I2C1_SDA_Read())
		{
		   Temp++;		
		}
		I2C1_Delay(2);
	}
	if(ack == I2C1_ACK)
		I2C1_Ack();
	else
		I2C1_NoAck();
	
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
* Limitations   : nothing
*********************************************************************************************************/
u8 I2C1_ReadOneData(u8 dev_addr, u8 reg_addr, u8 *data)
{
	I2C1_Start();

	I2C1_SendByte(dev_addr);
	if(I2C1_WaitAck() == I2C1_NO_ACK)
	{
		return I2C1_READ_ERROR;
	}

	I2C1_SendByte(reg_addr);
	if(I2C1_WaitAck() == I2C1_NO_ACK)
	{
		return I2C1_READ_ERROR;
	}

	// 重启 发送
	I2C1_Start();

	I2C1_SendByte(dev_addr+1);
	if(I2C1_WaitAck() == I2C1_NO_ACK)
	{
		return I2C1_READ_ERROR;
	}

	*data = I2C1_ReadByte(I2C1_NO_ACK);

	I2C1_Stop(); 

	return I2C1_READ_OK;    
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
u8 I2C1_ReadData(u8 dev_addr, u8 reg_addr, u8 count, u8 *pdata)
{
	u8 i = 0;

	I2C1_Start();

	I2C1_SendByte(dev_addr);
	if(I2C1_WaitAck() == I2C1_NO_ACK)
	{
		return I2C1_READ_ERROR;
	}

	I2C1_SendByte(reg_addr);
	if(I2C1_WaitAck() == I2C1_NO_ACK)
	{
		return I2C1_READ_ERROR;
	}

	// 重启 发送
	I2C1_Start();

	I2C1_SendByte(dev_addr+1);
	if(I2C1_WaitAck() == I2C1_NO_ACK)
	{
		return I2C1_READ_ERROR;
	}

	for(i=0; i<count; i++)
	{
		if(i != (count-1))
		{
			pdata[i] = I2C1_ReadByte(I2C1_ACK);
		}
		else
		{
			pdata[i] = I2C1_ReadByte(I2C1_NO_ACK);
		}
	}
	
	I2C1_Stop(); 

	return I2C1_READ_OK;    
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
u8 I2C1_WriteOneData(u8 dev_addr, u8 reg_addr, u8 data)
{	
	I2C1_Start();

	I2C1_SendByte(dev_addr);
	if(I2C1_WaitAck() == I2C1_NO_ACK)
	{
		return I2C1_WRITE_ERROR;
	}

	I2C1_SendByte(reg_addr);
	if(I2C1_WaitAck() == I2C1_NO_ACK)
	{
		return I2C1_WRITE_ERROR;
	}

	I2C1_SendByte(data);
	
	if(I2C1_WaitAck() == I2C1_NO_ACK)
	{
		return I2C1_WRITE_ERROR;
	}		

	I2C1_Stop();
	return I2C1_WRITE_OK;
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
u8 I2C1_WriteOneDataBit(u8 dev_addr, u8 reg_addr, u8 bitNum, u8 data)
{
    uint8_t byte;
    I2C1_ReadData(dev_addr, reg_addr, 1, &byte);
    byte = (data != 0) ? (byte | (1 << bitNum)) : (byte & ~(1 << bitNum));
    return I2C1_WriteOneData(dev_addr, reg_addr, byte);
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
u8 I2C1_WriteData(u8 dev_addr, u8 reg_addr, u8 count, u8 *pdata)
{
	uint8_t i=0;
	
	I2C1_Start();

	I2C1_SendByte(dev_addr);
	if(I2C1_WaitAck() == I2C1_NO_ACK)
	{
		return I2C1_WRITE_ERROR;
	}

	I2C1_SendByte(reg_addr);
	if(I2C1_WaitAck() == I2C1_NO_ACK)
	{
		return I2C1_WRITE_ERROR;
	}
	
	for(i=0; i<count; i++)
	{
		I2C1_SendByte(pdata[i]);
		if(I2C1_WaitAck() == I2C1_NO_ACK)
		{
			return I2C1_WRITE_ERROR;
		}		
	}

	I2C1_Stop();
	return I2C1_WRITE_OK;
}



/****************** 北京龙鼎源科技 http://www.bdrl.com.cn/ ********************/



