/**
  ******************************************************************************
  * @file    bsp_ad7606.c
  * @author  zwx
  * @version V1.1.0
  * @date    31-July-2013
  * @brief
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp_ad7606.h"



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/**
  * @brief
  * @param  None
  * @retval None
  */
void Bsp_InitAd7606(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);       //ʹ��GPIOBʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);        //ʹ��SPI1ʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ;                  //PB7-BUSY
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;                //��ͨ����ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;           //����ģʽ
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;            //������
    GPIO_Init(GPIOB, &GPIO_InitStructure);                      //��ʼ��PB7

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_9; //PB6/CS, PB8/RESET, PB9/CONVEST
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                //���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;               //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;            //����ģʽ
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                 //����
    GPIO_Init(GPIOB, &GPIO_InitStructure);                       //��ʼ��PB6,PB8,PB9

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;    //PB3, PB4���ù������
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       //50MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;            //����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;            //���ù���
    GPIO_Init(GPIOB, &GPIO_InitStructure);                  //��ʼ��

    GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1); //PB3����Ϊ SPI1-SCK
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1); //PB4����Ϊ SPI1-MISO

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                   //����SPI����ģʽ:����Ϊ��SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;      //����SPI�����ݴ�С:SPI���ͽ���16λ֡�ṹ
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                     //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                    //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                           //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;    //���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;      //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI_InitStructure.SPI_CRCPolynomial = 7;                            //CRCֵ����Ķ���ʽ
    SPI_Init(SPI1, &SPI_InitStructure);                                     //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���

    SPI_Cmd(SPI1, ENABLE);                                                              //ʹ��SPI����
}

/**
  * @brief
  * @param  None
  * @retval None
  */
void bsp_Ad7606Reset(void)
{
    AD7606_RESET=0; /* �˳���λ״̬ */

    AD7606_RESET=1; /* ���븴λ״̬ */
    AD7606_RESET=1; /* �ӳ١� RESET��λ�ߵ�ƽ��������С50ns�� */
    AD7606_RESET=1;
    AD7606_RESET=1;

    AD7606_RESET=0; /* �˳���λ״̬ */
}

/**
  * @brief
  * @param  None
  * @retval None
  */
void bsp_Ad7606StartConv(void)
{
    /* datasheet page 7��  CONVST �ߵ�ƽ�����Ⱥ͵͵�ƽ��������� 25ns */
    AD7606_CONVST=0;
    AD7606_CONVST=0;
    AD7606_CONVST=0;

    /* CONVSTƽʱΪ�� */
    AD7606_CONVST=1;
}


/**
  * @brief
  * @param  None
  * @retval None
  */
void bsp_Ad7606ReadCurAdc(uint16_t* ADC_buff)
{
    uint8_t  Channelx=0;
    uint16_t ADC_Value=0;

    /* �ȴ�ADת����ɣ�BUSY��� */
    while(AD7606_BUSY == 1);
    /* SPI1Ƭѡʹ�� */
    AD7606_CS = 0;
    /* �ɼ�8��ͨ����ÿ��ͨ��16��ʱ������ */
    for(Channelx=0; Channelx<8; Channelx++)
    {
        /* �ȴ��������� */
        while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) {}
        /* ͨ������SPIx����һ��word ���� */
        SPI_I2S_SendData(SPI1, 0xffff);
        /* �ȴ�������һ��word */
        while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) {}
        ADC_Value=SPI_I2S_ReceiveData(SPI1);
        /* ����ѹ������ǿ��Ϊ0 */
        if((ADC_Value&0x8000)==0x8000)
        {
            ADC_Value=0;
        }
        ADC_buff[Channelx]=ADC_Value;
    }
    /* SPI1Ƭѡ���� */
    AD7606_CS = 1;
    /* ����ת�� */
    bsp_Ad7606StartConv();
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
