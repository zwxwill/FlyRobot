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

/* ���Ͷ� */
u8 TxAddr[ADDR_LENGTH] = {0xC2,0xC2,0xC2,0xC2,0xC1};	// ���Ͷ˵ķ��͵�ַ 
u8 RxAddr0[ADDR_LENGTH] = {0xC2,0xC2,0xC2,0xC2,0xC1};	// ���Ͷ˵Ľ��յ�ַ -- ͨ��0����Ӧ��
/* ���ն� */
u8 RxAddr1[ADDR_LENGTH] = {0xC2,0xC2,0xC2,0xC2,0xC1};   // ��������ͨ�� P1
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;  //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;             // CE
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	NRF_CE_LOW();
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15);       //NRF24L01 IRQ PA15
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;                        //NRF24L01 IRQ PA15
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                //EXTI�ж�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;            //�½��ش���
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
        /* û�д����ɹ����û�������������봴��ʧ�ܵĴ������ */
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
	SPI2_WriteBuf(WR_TX_PLOAD, txbuf, len); 	// װ������	
	
	NRF_CE_HIGH();		 //�ø�CE���������ݷ���  ���ٱ���10us
	NRF2401_Delay(100);  
	NRF_CE_LOW();       //StandBy Iģʽ
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
	SPI2_WriteBuf(W_ACK_PAYLOAD, txbuf, len); 	// װ������	
	
	NRF_CE_HIGH();		 //�ø�CE���������ݷ���  ���ٱ���10us
	NRF2401_Delay(100);  
	NRF_CE_LOW();       //StandBy Iģʽ
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
	SPI2_WriteReg(NRF_WRITE_REG + CONFIG, 0x0e);   		// IRQ�շ�����жϿ���,16λCRC,������
	NRF2401_Delay(15000); /* delay 1.5ms */
	SPI2_WriteReg(NRF_WRITE_REG+EN_AA,0x01); 			//ʹ��ͨ��0���Զ�Ӧ����
	SPI2_WriteReg(NRF_WRITE_REG+EN_RXADDR,0x01);		//ʹ��ͨ��0�Ľ��յ�ַ ---  ����Ӧ���ź�
	SPI2_WriteReg(NRF_WRITE_REG+SETUP_AW,0x03);			//��ַField width
	SPI2_WriteReg(NRF_WRITE_REG+SETUP_RETR,0x1a);		//�����Զ��ط����ʱ��:500us;����Զ��ط�����:10��  0001 1010
	SPI2_WriteReg(NRF_WRITE_REG+RF_CH, 0x02);           //����RFͨ��Ϊ1
	SPI2_WriteReg(NRF_WRITE_REG+RF_SETUP,0x0e); 					//����TX�������,0db����,2Mbps,���������濪��
	SPI2_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0, RxAddr0, ADDR_LENGTH);	//дRX�ڵ��ַ --  ���Ͷ�����ͨ��0���յ�ַ������Ӧ���ź�
	SPI2_WriteBuf(NRF_WRITE_REG+TX_ADDR, TxAddr, ADDR_LENGTH); 		//дTX�ڵ��ַ --  ���Ͷ˷��͵�Ŀ�ĵ�ַ  
	
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
	SPI2_WriteReg(NRF_WRITE_REG + CONFIG, 0x0f);   		// IRQ�շ�����жϿ���,16λCRC,������
	NRF2401_Delay(15000); /* delay 1.5ms */
	SPI2_WriteReg(NRF_WRITE_REG+EN_AA,0x02); 			//ʹ��ͨ��1���Զ�Ӧ����
	SPI2_WriteReg(NRF_WRITE_REG+EN_RXADDR,0x02);		//ʹ��ͨ��1��������
	SPI2_WriteReg(NRF_WRITE_REG+SETUP_AW,0x03);			//��ַField width
	SPI2_WriteReg(NRF_WRITE_REG+SETUP_RETR,0x1a);		//�����Զ��ط����ʱ��:500us;����Զ��ط�����:10��  0001 1010
	SPI2_WriteReg(NRF_WRITE_REG+RF_CH, 0x02);           //����RFͨ��Ϊ2
	SPI2_WriteReg(NRF_WRITE_REG+RF_SETUP,0x0e); 					//����TX�������,0db����,2Mbps,���������濪��
	SPI2_WriteBuf(NRF_WRITE_REG+RX_ADDR_P1, RxAddr1, ADDR_LENGTH);	//дRX�ڵ��ַ --  ���Ͷ�����ͨ��1���յ�ַ������Ӧ���ź�
	SPI2_WriteReg(NRF_WRITE_REG+RX_PW_P1, DATA_LENGTH);  // ͨ��1 �������ݵĳ���	
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
	SPI2_WriteReg(NRF_WRITE_REG + CONFIG, 0x0f);   		// IRQ�շ�����жϿ���,16λCRC,������
	NRF2401_Delay(15000); /* delay 1.5ms */
	SPI2_WriteReg(NRF_WRITE_REG+EN_AA,0x02); 			//ʹ��ͨ��1���Զ�Ӧ����
	SPI2_WriteReg(NRF_WRITE_REG+EN_RXADDR,0x02);		//ʹ��ͨ��1��������
	SPI2_WriteReg(NRF_WRITE_REG+SETUP_AW,0x03);			//��ַField width
	SPI2_WriteReg(NRF_WRITE_REG+SETUP_RETR,0x1a);		//�����Զ��ط����ʱ��:500us;����Զ��ط�����:10��  0001 1010
	SPI2_WriteReg(NRF_WRITE_REG+RF_CH, 0x02);           //����RFͨ��Ϊ2
	SPI2_WriteReg(NRF_WRITE_REG+RF_SETUP,0x0e); 					//����TX�������,0db����,2Mbps,���������濪��
	SPI2_WriteBuf(NRF_WRITE_REG+RX_ADDR_P1, RxAddr1, ADDR_LENGTH);	//дRX�ڵ��ַ --  ���Ͷ�����ͨ��1���յ�ַ������Ӧ���ź�	
	SPI2_WriteReg(NRF_WRITE_REG+DYNPD, 0x02);    // ʹ��DPL_P1
	SPI2_WriteReg(NRF_WRITE_REG+FEATURE, 0x04);  // ʹ��EN_DPL 
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
	SPI2_WriteReg(NRF_WRITE_REG + CONFIG, 0x0f);   		// IRQ�շ�����жϿ���,16λCRC,������
	NRF2401_Delay(15000); /* delay 1.5ms */
	SPI2_WriteReg(NRF_WRITE_REG+EN_AA,0x02); 			//ʹ��ͨ��1���Զ�Ӧ����
	SPI2_WriteReg(NRF_WRITE_REG+EN_RXADDR,0x02);		//ʹ��ͨ��1��������
	SPI2_WriteReg(NRF_WRITE_REG+SETUP_AW,0x03);			//��ַField width
	SPI2_WriteReg(NRF_WRITE_REG+SETUP_RETR,0x1a);		//�����Զ��ط����ʱ��:500us;����Զ��ط�����:10��  0001 1010
	SPI2_WriteReg(NRF_WRITE_REG+RF_CH, 0x02);           //����RFͨ��Ϊ2
	SPI2_WriteReg(NRF_WRITE_REG+RF_SETUP,0x0e); 					//����TX�������,0db����,2Mbps,���������濪��
	SPI2_WriteBuf(NRF_WRITE_REG+RX_ADDR_P1, RxAddr1, ADDR_LENGTH);	//дRX�ڵ��ַ --  ���Ͷ�����ͨ��1���յ�ַ������Ӧ���ź�	
	SPI2_WriteReg(NRF_WRITE_REG+DYNPD, 0x02);    // ʹ��DPL_P1
	SPI2_WriteReg(NRF_WRITE_REG+FEATURE, 0x06);  // ʹ��EN_DPL  EN_ACK_PAY
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
	if(status & RX_DR)    //  ��֪�������ǲ�֪�ﵽָ���ķ���λ���Ŵ����ж�
	{
		SPI2_WriteReg(NRF_WRITE_REG + NRFRegSTATUS, RX_DR); // �����־λ
//		len = SPI2_ReadReg(R_RX_PL_WID);    // only used in NRF2401_SetEnHancedDynRxMode else len = DATA_LENGTH
//		SPI2_ReadBuf(RD_RX_PLOAD, buf, len);//��ȡ����
		pNrfMsg = &g_tNrfRxMsg;
		pNrfMsg->len = SPI2_ReadReg(R_RX_PL_WID); 
		SPI2_ReadBuf(RD_RX_PLOAD, pNrfMsg->buf, g_tNrfRxMsg.len);//��ȡ����
		xQueueSendFromISR(xQueueNrfMsg, (void *)&pNrfMsg, &xHigherPriorityTaskWoken);		
//		printf("recv:");
//		for(i=0; i<len; i++)
//		{			
//			printf(" %d", buf[i]);
//		}
//		printf("\r\n");
		SPI2_WriteReg(FLUSH_RX, NOP);    // ���RX FIFO�Ĵ��� 
                                         // ֪ͨAPP���յ�����
	}
	if(status & MAX_RT) /* �ﵽ����ط����� */
	{	
		printf("max_rt\r\n");
		SPI2_WriteReg(NRF_WRITE_REG + NRFRegSTATUS, MAX_RT); /*���TX_DS��MAX_RT�жϱ�־*/
		SPI2_WriteReg(FLUSH_TX, NOP);      /* ���TX FIFO�Ĵ��� */
	}
	if(status & TX_DS)/* ������� */
	{
		printf("tx_ds\r\n");
		SPI2_WriteReg(NRF_WRITE_REG+NRFRegSTATUS, TX_DS); /*���TX_DS��MAX_RT�жϱ�־*/ 
		SPI2_WriteReg(FLUSH_TX, NOP);      /* ���TX FIFO�Ĵ��� */		
	}
}


/****************** ��������Դ�Ƽ� http://www.bdrl.com.cn/ ********************/
