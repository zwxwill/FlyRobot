/**
  ******************************************************************************
  * @file    bsp_nrf24l01.c
  * @author  zwx
  * @version V1.8.0
  * @date    2019-01-29
  * @brief   
  ******************************************************************************
  */  

#include "bsp_nrf24l01.h"
#include "bsp_spi.h"
#include "bsp.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <stdio.h>



/* Exported functions --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/
QueueHandle_t xQueueNrfMsg = NULL;
/* Private variables --------------------------------------------------------*/
static T_NrfRxMsg g_tNrfRxMsg;
/* Private struct  -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* 发送端 */
u8 TxAddr[ADDR_LENGTH] = {0xC2,0xC2,0xC2,0xC2,0xC1};	// 发送端的发送地址 
u8 RxAddr0[ADDR_LENGTH] = {0xC2,0xC2,0xC2,0xC2,0xC1};	// 发送端的接收地址 -- 通道0接收应答
/* 接收端 */
u8 RxAddr1[ADDR_LENGTH] = {0xC2,0xC2,0xC2,0xC2,0xC1};   // 接收数据通道 P1
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void NRF2401_SetEnHancedTxMode(void);
void NRF2401_SetEnHancedRxMode(void);
void NRF2401_SetEnHancedDynRxMode(void);

/**
  * @brief  PA15:NRF2401_IQR     PA8:NRF2401_CE
  * @param  None
  * @retval None
  */
void NRF2401_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;  // IRQ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;  //上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;             // CE
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	NRF_CE_LOW();
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15);       //NRF24L01 IRQ PA15
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;                        //NRF24L01 IRQ PA15
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                //EXTI中断
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;            //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;                          
	EXTI_Init(&EXTI_InitStructure);	
	
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_EXIT15_PRE;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);	
	
	SPI2_Configuration();
	
	xQueueNrfMsg = xQueueCreate(10, sizeof(struct NRF_RxMsg *));
    if( xQueueNrfMsg == 0 )
    {
        /* 没有创建成功，用户可以在这里加入创建失败的处理机制 */
		printf(" xQueueCreate error !\r\n  ");
    }	
	
	//NRF2401_SetEnHancedTxMode();
	//NRF2401_SetEnHancedRxMode();
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
static void NRF2401_Delay(u16 cnt)
{
	while(cnt>0)
	{
		cnt--;
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
void NRF2401_TxPacket(u8 *txbuf, u8 len)
{
	SPI2_WriteBuf(WR_TX_PLOAD, txbuf, len); 	// 装载数据	
	
	NRF_CE_HIGH();		 //置高CE，激发数据发送  至少保持10us
	NRF2401_Delay(100);  
	NRF_CE_LOW();       //StandBy I模式
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
void NRF2401_TxAckPacket(u8 *txbuf, u8 len)
{
	SPI2_WriteBuf(W_ACK_PAYLOAD, txbuf, len); 	// 装载数据	
	
	NRF_CE_HIGH();		 //置高CE，激发数据发送  至少保持10us
	NRF2401_Delay(100);  
	NRF_CE_LOW();       //StandBy I模式
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
void NRF2401_SetEnHancedTxMode(void)
{
	NRF_CE_LOW();
	SPI2_WriteReg(NRF_WRITE_REG + CONFIG, 0x0e);   		// IRQ收发完成中断开启,16位CRC,主发送
	NRF2401_Delay(15000); /* delay 1.5ms */
	SPI2_WriteReg(NRF_WRITE_REG+EN_AA,0x01); 			//使能通道0的自动应答功能
	SPI2_WriteReg(NRF_WRITE_REG+EN_RXADDR,0x01);		//使能通道0的接收地址 ---  接收应答信号
	SPI2_WriteReg(NRF_WRITE_REG+SETUP_AW,0x03);			//地址Field width
	SPI2_WriteReg(NRF_WRITE_REG+SETUP_RETR,0x1a);		//设置自动重发间隔时间:500us;最大自动重发次数:10次  0001 1010
	SPI2_WriteReg(NRF_WRITE_REG+RF_CH, 0x02);           //设置RF通道为1
	SPI2_WriteReg(NRF_WRITE_REG+RF_SETUP,0x0e); 					//设置TX发射参数,0db增益,2Mbps,低噪声增益开启
	SPI2_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0, RxAddr0, ADDR_LENGTH);	//写RX节点地址 --  发送端数据通道0接收地址，接收应答信号
	SPI2_WriteBuf(NRF_WRITE_REG+TX_ADDR, TxAddr, ADDR_LENGTH); 		//写TX节点地址 --  发送端发送的目的地址  
	
	NRF_CE_HIGH();
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
void NRF2401_SetEnHancedRxMode(void)
{
	NRF_CE_LOW();
	SPI2_WriteReg(NRF_WRITE_REG + CONFIG, 0x0f);   		// IRQ收发完成中断开启,16位CRC,主接收
	NRF2401_Delay(15000); /* delay 1.5ms */
	SPI2_WriteReg(NRF_WRITE_REG+EN_AA,0x02); 			//使能通道1的自动应答功能
	SPI2_WriteReg(NRF_WRITE_REG+EN_RXADDR,0x02);		//使能通道1接收数据
	SPI2_WriteReg(NRF_WRITE_REG+SETUP_AW,0x03);			//地址Field width
	SPI2_WriteReg(NRF_WRITE_REG+SETUP_RETR,0x1a);		//设置自动重发间隔时间:500us;最大自动重发次数:10次  0001 1010
	SPI2_WriteReg(NRF_WRITE_REG+RF_CH, 0x02);           //设置RF通道为2
	SPI2_WriteReg(NRF_WRITE_REG+RF_SETUP,0x0e); 					//设置TX发射参数,0db增益,2Mbps,低噪声增益开启
	SPI2_WriteBuf(NRF_WRITE_REG+RX_ADDR_P1, RxAddr1, ADDR_LENGTH);	//写RX节点地址 --  发送端数据通道1接收地址，接收应答信号
	SPI2_WriteReg(NRF_WRITE_REG+RX_PW_P1, DATA_LENGTH);  // 通道1 接收数据的长度	
	NRF_CE_HIGH();
	NRF2401_Delay(1300);  // After 130us nRF24L01+ monitors the air for incoming communication
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
void NRF2401_SetEnHancedDynRxMode(void)
{
	NRF_CE_LOW();
	SPI2_WriteReg(NRF_WRITE_REG + CONFIG, 0x0f);   		// IRQ收发完成中断开启,16位CRC,主接收
	NRF2401_Delay(15000); /* delay 1.5ms */
	SPI2_WriteReg(NRF_WRITE_REG+EN_AA,0x02); 			//使能通道1的自动应答功能
	SPI2_WriteReg(NRF_WRITE_REG+EN_RXADDR,0x02);		//使能通道1接收数据
	SPI2_WriteReg(NRF_WRITE_REG+SETUP_AW,0x03);			//地址Field width
	SPI2_WriteReg(NRF_WRITE_REG+SETUP_RETR,0x1a);		//设置自动重发间隔时间:500us;最大自动重发次数:10次  0001 1010
	SPI2_WriteReg(NRF_WRITE_REG+RF_CH, 0x02);           //设置RF通道为2
	SPI2_WriteReg(NRF_WRITE_REG+RF_SETUP,0x0e); 					//设置TX发射参数,0db增益,2Mbps,低噪声增益开启
	SPI2_WriteBuf(NRF_WRITE_REG+RX_ADDR_P1, RxAddr1, ADDR_LENGTH);	//写RX节点地址 --  发送端数据通道1接收地址，接收应答信号	
	SPI2_WriteReg(NRF_WRITE_REG+DYNPD, 0x02);    // 使能DPL_P1
	SPI2_WriteReg(NRF_WRITE_REG+FEATURE, 0x04);  // 使能EN_DPL 
	NRF_CE_HIGH();
	NRF2401_Delay(1300);  // After 130us nRF24L01+ monitors the air for incoming communication
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
void NRF2401_SetEnHancedDynAckPayRxMode(void)
{
	NRF_CE_LOW();
	SPI2_WriteReg(NRF_WRITE_REG + CONFIG, 0x0f);   		// IRQ收发完成中断开启,16位CRC,主接收
	NRF2401_Delay(15000); /* delay 1.5ms */
	SPI2_WriteReg(NRF_WRITE_REG+EN_AA,0x02); 			//使能通道1的自动应答功能
	SPI2_WriteReg(NRF_WRITE_REG+EN_RXADDR,0x02);		//使能通道1接收数据
	SPI2_WriteReg(NRF_WRITE_REG+SETUP_AW,0x03);			//地址Field width
	SPI2_WriteReg(NRF_WRITE_REG+SETUP_RETR,0x1a);		//设置自动重发间隔时间:500us;最大自动重发次数:10次  0001 1010
	SPI2_WriteReg(NRF_WRITE_REG+RF_CH, 0x02);           //设置RF通道为2
	SPI2_WriteReg(NRF_WRITE_REG+RF_SETUP,0x0e); 					//设置TX发射参数,0db增益,2Mbps,低噪声增益开启
	SPI2_WriteBuf(NRF_WRITE_REG+RX_ADDR_P1, RxAddr1, ADDR_LENGTH);	//写RX节点地址 --  发送端数据通道1接收地址，接收应答信号	
	SPI2_WriteReg(NRF_WRITE_REG+DYNPD, 0x02);    // 使能DPL_P1
	SPI2_WriteReg(NRF_WRITE_REG+FEATURE, 0x06);  // 使能EN_DPL  EN_ACK_PAY
	NRF_CE_HIGH();
	NRF2401_Delay(1300);  // After 130us nRF24L01+ monitors the air for incoming communication
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
void NRF2401_Irq(void)
{
	u8 i = 0;
//	u8 len = 0;
	u8 sum = 0;
	u8 status = 0;
//	u8 buf[DATA_LENGTH] = {0};
	T_NrfRxMsg *pNrfMsg;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	
	
	status = SPI2_ReadReg(NRF_READ_REG + NRFRegSTATUS);
	if(status & RX_DR)    //  不知道这里是不知达到指定的发送位数才触发中断
	{
		SPI2_WriteReg(NRF_WRITE_REG + NRFRegSTATUS, RX_DR); // 清除标志位
//		len = SPI2_ReadReg(R_RX_PL_WID);    // only used in NRF2401_SetEnHancedDynRxMode else len = DATA_LENGTH
//		SPI2_ReadBuf(RD_RX_PLOAD, buf, len);//读取数据
		pNrfMsg = &g_tNrfRxMsg;
		pNrfMsg->len = SPI2_ReadReg(R_RX_PL_WID); 
		SPI2_ReadBuf(RD_RX_PLOAD, pNrfMsg->buf, g_tNrfRxMsg.len);//读取数据
		xQueueSendFromISR(xQueueNrfMsg, (void *)&pNrfMsg, &xHigherPriorityTaskWoken);		
//		printf("recv:");
//		for(i=0; i<len; i++)
//		{			
//			printf(" %d", buf[i]);
//		}
//		printf("\r\n");
		SPI2_WriteReg(FLUSH_RX, NOP);    // 清除RX FIFO寄存器 
                                         // 通知APP接收到数据
	}
	if(status & MAX_RT) /* 达到最大重发次数 */
	{	
		printf("max_rt\r\n");
		SPI2_WriteReg(NRF_WRITE_REG + NRFRegSTATUS, MAX_RT); /*清除TX_DS或MAX_RT中断标志*/
		SPI2_WriteReg(FLUSH_TX, NOP);      /* 清除TX FIFO寄存器 */
	}
	if(status & TX_DS)/* 发送完成 */
	{
		printf("tx_ds\r\n");
		SPI2_WriteReg(NRF_WRITE_REG+NRFRegSTATUS, TX_DS); /*清除TX_DS或MAX_RT中断标志*/ 
		SPI2_WriteReg(FLUSH_TX, NOP);      /* 清除TX FIFO寄存器 */		
	}
}


/****************** 北京龙鼎源科技 http://www.bdrl.com.cn/ ********************/
