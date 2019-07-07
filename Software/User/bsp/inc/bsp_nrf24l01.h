/**
  ******************************************************************************
  * @file    bsp_nrf24l01.h
  * @author  zwx
  * @version V1.8.0
  * @date    2019-01-29
  * @brief   
  ******************************************************************************
  */  

#ifndef __BSP_NRF24L01_H__
#define __BSP_NRF24L01_H__

#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif
	
//*************************************** SPI Command *******************************************************
#define NRF_READ_REG    0x00  	// ���Ĵ���ָ��
#define NRF_WRITE_REG   0x20 	// д�Ĵ���ָ��
#define RD_RX_PLOAD     0x61  	// ��ȡ��������ָ��
#define WR_TX_PLOAD     0xA0  	// д��������ָ��
#define FLUSH_TX        0xE1 	// ��ϴ���� FIFOָ��
#define FLUSH_RX        0xE2  	// ��ϴ���� FIFOָ��
#define REUSE_TX_PL     0xE3  	// �����ظ�װ������ָ��
#define R_RX_PL_WID   	0x60    // Read RX payload width for the top R_RX_PAYLOAD in the RX FIFO
#define W_ACK_PAYLOAD   0xA8    // Write Payload + ACK PPP = 000 / 001 / 010 / 011 / 101
#define W_TX_PAYLOAD_NO_ACK  0xB0 // disable auto ack on this specific packet
#define NOP             0xFF  	// ����

	
//*************************************NRF24L01+�Ĵ�����ַ****************************************************
#define CONFIG          0x00  // �����շ�״̬��CRCУ��ģʽ�Լ��շ�״̬��Ӧ��ʽ
#define EN_AA           0x01  // �Զ�Ӧ��������
#define EN_RXADDR       0x02  // �����ŵ�����
#define SETUP_AW        0x03  // �շ���ַ�������
#define SETUP_RETR      0x04  // �Զ��ط���������
#define RF_CH           0x05  // ����Ƶ������
#define RF_SETUP        0x06  // �������ʡ����Ĺ�������
#define NRFRegSTATUS    0x07  // ״̬�Ĵ���
#define OBSERVE_TX      0x08  // ���ͼ�⹦��
#define CD              0x09  // ��ַ���           
#define RX_ADDR_P0      0x0A  // Ƶ��0�������ݵ�ַ
#define RX_ADDR_P1      0x0B  // Ƶ��1�������ݵ�ַ
#define RX_ADDR_P2      0x0C  // Ƶ��2�������ݵ�ַ
#define RX_ADDR_P3      0x0D  // Ƶ��3�������ݵ�ַ
#define RX_ADDR_P4      0x0E  // Ƶ��4�������ݵ�ַ
#define RX_ADDR_P5      0x0F  // Ƶ��5�������ݵ�ַ
#define TX_ADDR         0x10  // ���͵�ַ�Ĵ���
#define RX_PW_P0        0x11  // ����Ƶ��0�������ݳ���
#define RX_PW_P1        0x12  // ����Ƶ��1�������ݳ���
#define RX_PW_P2        0x13  // ����Ƶ��2�������ݳ���
#define RX_PW_P3        0x14  // ����Ƶ��3�������ݳ���
#define RX_PW_P4        0x15  // ����Ƶ��4�������ݳ���
#define RX_PW_P5        0x16  // ����Ƶ��5�������ݳ���
#define FIFO_STATUS     0x17  // FIFOջ��ջ��״̬�Ĵ�������
#define DYNPD			0x1C  // ��̬ payload ����
#define FEATURE         0x1D  // Feature register

/* NRFRegSTATUS �е�λ */
#define RX_DR				    0x40	   //RX �ж�
#define TX_DS				    0x20       //TX �ж�
#define MAX_RT			        0x10       //MAX TX �ж�
#define RX_P_NO                 0x0E       //Data Pipe Number
#define TX_FULL                 0x01       

/* NRF24L01 ����һЩ���� */
#define ADDR_LENGTH		 (5)
#define DATA_LENGTH      (10)

#define NRF_CE_HIGH()     GPIO_SetBits(GPIOA, GPIO_Pin_8)
#define NRF_CE_LOW()      GPIO_ResetBits(GPIOA, GPIO_Pin_8)

/* Exported define -----------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
#define NRF_RX_MAX_BUF_LEN   (32)
typedef struct NRF_RxMsg
{
	int len;
	u8 buf[NRF_RX_MAX_BUF_LEN];
}T_NrfRxMsg;
/* Exported struct  ----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void NRF2401_Init(void);
void NRF2401_SetEnHancedTxMode(void);
void NRF2401_SetEnHancedRxMode(void);
void NRF2401_TxPacket(u8 *txbuf, u8 len);
void NRF2401_SetEnHancedDynRxMode(void);
void NRF2401_TxAckPacket(u8 *txbuf, u8 len);

#ifdef __cplusplus
}
#endif

#endif

/****************** ��������Դ�Ƽ� http://www.bdrl.com.cn/ ********************/
