#include "bsp_dm9k.h"
#include "lwip/netif.h"
#include "bsp.h"

#include "FreeRTOS.h"
#include "semphr.h"

#include <stddef.h>

/* Exported variables --------------------------------------------------------*/
extern xSemaphoreHandle s_xInputSemaphore;
extern xSemaphoreHandle s_xLockSemaphore;
/* Exported functions --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private struct  -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//#define printk(...)
#define printk printf
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
*   ������: DM9K_GPIOConfig
*   ��  ��: ��
*   ��  ��: ��
*   ��  ��: ����DM9000AE���ƿ��ߣ�FSMC�ܽ�����Ϊ���ù���
*/
void DM9K_GPIOConfig(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* ʹ�� GPIOʱ�� */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOG, ENABLE);

    /* ���� PD.00(D2), PD.01(D3), PD.04(NOE), PD.05(NWE), PD.08(D13), PD.09(D14),
     PD.10(D15), PD.14(D0), PD.15(D1) Ϊ����������� */

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
                                  GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 |
                                  GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* ���� PE.07(D4), PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9), PE.13(D10),
     PE.14(D11), PE.15(D12) Ϊ����������� */
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource15, GPIO_AF_FSMC);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
                                  GPIO_Pin_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /* ���� PE.4(CMD)  Ϊ����������� */
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource4, GPIO_AF_FSMC);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /* ���� PG12 (CS)) Ϊ����������� */
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource12, GPIO_AF_FSMC);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_Init(GPIOG, &GPIO_InitStructure);
}

/*******************************************************************************
*   ������: DM9K_NVICConfig
*   ��  ��: ��
*   ��  ��: ��
*   ��  ��: ����DM9000AE���ƿ��ߣ�FSMC�ܽ�����Ϊ���ù���
*/
void DM9K_NVICConfig(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* ʹ�� GPIOʱ�� */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    /* ʹ��SYSFGʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    /* PC0 ��DM9000_INT�ж������ */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       //PC0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_Init(GPIOC,&GPIO_InitStructure);

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource0);

    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    EXTI_ClearITPendingBit(EXTI_Line0);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;                        //�ⲿ�ж���0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EXTI0_IRQ_PRI;   //��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                      //�����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/*******************************************************************************
*   ������: DM9K_FSMCConfig
*   ��  ��: ��
*   ��  ��: ��
*   ��  ��: ����FSMC���ڷ���ʱ��
*/
void DM9K_FSMCConfig(void)
{
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  p;

    /* ʹ��FSMCʱ�� */
    RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);

    /*-- FSMC Configuration ------------------------------------------------------*/
    /*----------------------- SRAM Bank 3 ----------------------------------------*/
    /*-- FSMC Configuration ------------------------------------------------------*/
    p.FSMC_AddressSetupTime = 5;        /* ����Ϊ2�����; 3���� */
    p.FSMC_AddressHoldTime = 1;
    p.FSMC_DataSetupTime = 10;          /* ����Ϊ1����2���� */
    p.FSMC_BusTurnAroundDuration = 5;
    p.FSMC_CLKDivision = 0;
    p.FSMC_DataLatency = 0;
    p.FSMC_AccessMode = FSMC_AccessMode_A;

    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;   // FSMC_MemoryType_PSRAM;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

    /*!< Enable FSMC Bank1_SRAM3 Bank */
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);
}

/*******************************************************************************
*   ������: dm9k_udelay
*   ��  ��: time �� �ӳ�ʱ�䣬����ȷ��us����
*   ��  ��: ��
*   ��  ��: �ӳٺ���
*/
void dm9k_udelay(uint16_t time)
{
    uint16_t i,k;

    for(i = 0; i < time; i++)
    {
        for(k = 0; k < 100; k++);
    }
    while(time--);
}

/*
*********************************************************************************************************
*   �� �� ��: dm9k_ReadReg
*   ����˵��: ����DM9000ָ���Ĵ�����ֵ
*   ��    ��: reg �Ĵ�����ַ
*   �� �� ֵ: �Ĵ���ֵ
*********************************************************************************************************
*/
uint8_t dm9k_ReadReg(uint8_t reg)
{
    NET_REG_ADDR = reg;
    return (uint8_t)(NET_REG_DATA);
}

/*
*********************************************************************************************************
*   �� �� ��: dm9k_WriteReg
*   ����˵��: ����DM9000ָ���Ĵ�����ֵ
*   ��    ��: reg ���Ĵ�����ַ
*            writedata : д�������
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void dm9k_WriteReg(uint8_t reg, uint8_t writedata)
{
    NET_REG_ADDR = reg;
    NET_REG_DATA = writedata;
}

/*******************************************************************************
*   ������: dm9k_phy_write
*   ��  ��: phy_reg �� �Ĵ�����ַ
*           writedata �� д�������
*   ��  ��: ��
*   ��  ��: дDM9000A PHY �Ĵ���
*/
void dm9k_phy_write(uint8_t phy_reg, uint16_t writedata)
{
    /* ����д�� PHY �Ĵ�����λ�� */
    dm9k_WriteReg(DM9000_REG_EPAR, phy_reg | DM9000_PHY);

    /* ����д�� PHY �Ĵ�����ֵ */
    dm9k_WriteReg(DM9000_REG_EPDRH, (writedata >> 8) & 0xff);
    dm9k_WriteReg(DM9000_REG_EPDRL, writedata & 0xff);

    dm9k_WriteReg(DM9000_REG_EPCR, 0x0a);                       /* ������д�� PHY �Ĵ��� */
    while(dm9k_ReadReg(DM9000_REG_EPCR) & 0x01);                /* ��Ѱ�Ƿ�ִ�н��� */
    dm9k_udelay(500);
    dm9k_WriteReg(DM9000_REG_EPCR, 0x00);                       /* ���д������ */
}

/*******************************************************************************
*   ������: dm9k_phy_read
*   ��  ��: phy_reg �� �Ĵ�����ַ
*   ��  ��: ��
*   ��  ��: ��DM9000A PHY �Ĵ���
*/
u16 dm9k_phy_read(uint8_t phy_reg)
{
    u16 temp = 0;

    /* ����д�� PHY �Ĵ�����λ�� */
    dm9k_WriteReg(DM9000_REG_EPAR, phy_reg | DM9000_PHY);

    dm9k_WriteReg(DM9000_REG_EPCR, 0x0C);                       /* �����϶��� PHY �Ĵ��� */
    while(dm9k_ReadReg(DM9000_REG_EPCR) & 0x01);                /* ��Ѱ�Ƿ�ִ�н��� */
    dm9k_udelay(500);
    dm9k_WriteReg(DM9000_REG_EPCR, 0x00);                       /* ���д������ */

    temp=(dm9k_ReadReg(DM9000_REG_EPDRH)<<8)|(dm9k_ReadReg(DM9000_REG_EPDRL));

    return temp;
}

/* Set PHY operationg mode */
void dm9k_phy_mode_set(uint32_t media_mode)
{
    uint16_t phy_reg4 = 0, phy_reg0 = 0;

    switch(media_mode)
    {
        case DM9000_10MHD:
            phy_reg4 = 0x21;
            phy_reg0 = 0x0000;
            break;
        case DM9000_10MFD:
            phy_reg4 = 0x41;
            phy_reg0 = 0x1100;
            break;
        case DM9000_100MHD:
            phy_reg4 = 0x81;
            phy_reg0 = 0x2000;
            break;
        case DM9000_100MFD:
            phy_reg4 = 0x101;
            phy_reg0 = 0x3100;
            break;
        default:
            phy_reg4 = 0x01e1;
            phy_reg0 = 0x1000;
            break;
    }
    dm9k_WriteReg(DM9000_REG_GPR, DM9000_PHY_OFF);  /* �ر� PHY ������ PHY ����*/

    dm9k_phy_write(DM9000_PHY_BMCR, 0x8000);    /* ���� PHY �ļĴ��� */
    while(dm9k_phy_read(DM9000_PHY_BMCR) & 0x8000);
    dm9k_udelay(100);
    dm9k_phy_write(DM9000_PHY_BMCR, phy_reg0);  /*  Tmp */
    dm9k_phy_write(DM9000_PHY_ANAR, phy_reg4);  /* Set PHY media mode */

    dm9k_WriteReg(DM9000_REG_GPCR, GPCR_GEP_CNTL);  /* Let GPIO0 output */
    dm9k_WriteReg(DM9000_REG_GPR, DM9000_PHY_ON);   /* ���� PHY ����, ���� PHY */
}

/*******************************************************************************
*   ������: dm9k_hash_table
*   ��  ��: ��
*   ��  ��: ��
*   ��  ��: ���� DM9000A MAC �� �㲥 �� �ಥ �Ĵ���
*/
void dm9k_set_mac(void)
{
    /* ���� ���� MAC λ�ã������ MyHardware */
    dm9k_WriteReg(DM9000_REG_PAR+0, DM9K_MAC_ADDR0);
    dm9k_WriteReg(DM9000_REG_PAR+1, DM9K_MAC_ADDR1);
    dm9k_WriteReg(DM9000_REG_PAR+2, DM9K_MAC_ADDR2);
    dm9k_WriteReg(DM9000_REG_PAR+3, DM9K_MAC_ADDR3);
    dm9k_WriteReg(DM9000_REG_PAR+4, DM9K_MAC_ADDR4);
    dm9k_WriteReg(DM9000_REG_PAR+5, DM9K_MAC_ADDR5);
}

/*******************************************************************************
*   ������: dm9k_hash_table
*   ��  ��: ��
*   ��  ��: ��
*   ��  ��: ���� DM9000A MAC �� �㲥 �� �ಥ �Ĵ���
*/
void dm9k_hash_table(void)
{
    uint8_t i;

    for(i = 0; i < 8; i++)                              /* ��� �����ಥ���� */
    {
        dm9k_WriteReg(DM9000_REG_MAR + i, 0x00);
    }
    dm9k_WriteReg(DM9000_REG_MAR + 7, 0x80);            /* ������ �㲥�� ���� */
}

/*******************************************************************************
*   ������: dm9k_initFunc
*   ��  ��: ��
*   ��  ��: ��
*   ��  ��: ��ʼ��DM9000AE
*/
void dm9k_initFunc(void)
{
    /* 1:�����λ */
    dm9k_WriteReg(DM9000_REG_NCR, NCR_RST);         /* �� DM9000A ����������� */
    dm9k_udelay(1000);                              /* delay 1ms */
    while((dm9k_ReadReg(DM9000_REG_NCR) & 0x01) != 0);     /* �ȴ���λ��� */

    /* 2 ʹ���ڲ�PHY */
    /* GPIO0 on pre-activate PHY */
    dm9k_WriteReg(DM9000_REG_GPR, 0x00);                /* power up PHY */
    dm9k_WriteReg(DM9000_REG_GPCR, GPCR_GEP_CNTL);      /* Let GPIO0 output */
    dm9k_WriteReg(DM9000_REG_GPR, 0x00);                /* power up PHY */

    /* 3 NCR ���� */
    /* Program operating register */
    dm9k_WriteReg(DM9000_REG_NCR, 0x0);     /* only intern phy supported by now */

    /* 4 Clear TX status */
    dm9k_WriteReg(DM9000_REG_NSR, NSR_WAKEST | NSR_TX2END | NSR_TX1END);    /* clear TX status */
    dm9k_WriteReg(DM9000_REG_ISR, 0x0f);    /* Clear interrupt status */

    /* 5 read eeporm data */
    dm9k_phy_mode_set(DM9000_AUTO);                 /* enable SRAM read/write */

    /* 6 set mac */
    dm9k_set_mac();                                 /* ���� DM9000A MAC */

    dm9k_WriteReg(DM9000_REG_TCR2, 0x80);   /* Switch LED to mode 1 */

    /* 7 set hash table */
    dm9k_hash_table();                              /* ���� �ಥ*/

    /* 8 set IMR */
    dm9k_WriteReg(DM9000_REG_IMR, IMR_PAR);

    /* 9 set NIC  */
    /* 10 set IMR for interrupt */
    dm9k_WriteReg(DM9000_REG_IMR, IMR_PAR | IMR_PRM);   /* Enable TX/RX interrupt mask */

    /* 11 program RXCR to enable RX */
    dm9k_WriteReg(DM9000_REG_RCR, RCR_DIS_LONG | RCR_DIS_CRC | RCR_RXEN);   /* RX enable */
}

/*******************************************************************************
*   ������: dm9k_receive_packet
*   ��  ��: _uip_buf : ���ջ�����
*   ��  ��: > 0 ��ʾ���յ����ݳ���, 0��ʾû������
*   ��  ��: ��ȡһ������
*/
struct pbuf *dm9k_receive_packet(void)
{
    struct pbuf* p = NULL;
    struct pbuf* q = NULL;
    u32 rxbyte = 0;
    vu16 rx_status, rx_length;
    u16* data = NULL;
    u16 dummy = 0;
    int len = 0;

    xSemaphoreTake(s_xLockSemaphore, portMAX_DELAY);  /* ���󻥳��� */

__error_retry:
    dm9k_ReadReg(DM9000_REG_MRCMDX);                    /* ��ȡ�ڴ����ݣ���ַ������ */
    rxbyte = dm9k_ReadReg(DM9000_REG_MRCMDX);           /* Got most updated data */

    if(rxbyte == DM9000_PKT_RDY)
    {
        /* A packet ready now  & Get status/length */
        NET_REG_ADDR = DM9000_REG_MRCMD;
        rx_status = NET_REG_DATA;
        rx_length = NET_REG_DATA;

        p = pbuf_alloc(PBUF_RAW, rx_length, PBUF_POOL); //pbufs�ڴ�ط���pbuf
        if(p != NULL)                                   //�ڴ�����ɹ�
        {
            for(q=p; q!=NULL; q=q->next)
            {
                data = (u16*)q->payload;
                len = q->len;
                while(len > 0)
                {
                    *data = NET_REG_DATA;
                    data++;
                    len -= 2;
                }
            }
        }
        else                                        //�ڴ�����ʧ��
        {
            printk("Dm9k pbuf�ڴ�����ʧ��:%d\r\n",rx_length);
            data = &dummy;
            len = rx_length;
            while(len > 0)
            {
                *data = NET_REG_DATA;
                len -= 2;
            }
        }
        //����rx_status�жϽ��������Ƿ�������´���FIFO�����CRC����
        //���������������������κ�һ�����ֵĻ�����������֡��
        //��rx_lengthС��64���ߴ���������ݳ��ȵ�ʱ��Ҳ����������֡
        if((rx_status & 0XBF00) || (rx_length < 0X40) || (rx_length > DM9K_PKT_MAX))
        {
            if(rx_status & 0x100)
            {
                printk("Dm9k rx fifo error\r\n");
            }
            if(rx_status & 0x200)
            {
                printk("Dm9k rx crc error\r\n");
            }
            if(rx_status & 0x8000)
            {
                printk("Dm9k rx length error\r\n");
            }
            if(rx_length > DM9K_PKT_MAX)
            {
                printk("Dm9k rx length too big\r\n");
                dm9k_WriteReg(DM9000_REG_NCR, NCR_RST); //��λDM9000
                dm9k_udelay(5000);
            }
            if(p != NULL)
            {
                pbuf_free((struct pbuf*)p);    //�ͷ��ڴ�
            }
            p = NULL;
            goto __error_retry;
        }
    }
    else  //[ZWX]
    {
        if(rxbyte == DM9000_PKT_NORDY)
        {
            /* clear packet received latch status */
            dm9k_WriteReg(DM9000_REG_ISR, ISR_PRS);

            /* restore receive interrupt */
            dm9k_WriteReg(DM9000_REG_IMR, IMR_PAR | IMR_PRM);
        }
        else
        {
            printk("dm9000 rx: rx error 0x%x, stop device\n", rxbyte);

            dm9k_WriteReg(DM9000_REG_RCR, 0x00);    /* Stop Device */
            dm9k_WriteReg(DM9000_REG_ISR, 0x80);    /* Stop INT request */
            while(1);
        }
    }

    xSemaphoreGive(s_xLockSemaphore);   /* ���� */

    return (struct pbuf*)p;
}

/*******************************************************************************
*   ������: dm9k_send_packet
*   ��  ��: p_char : �������ݻ�����
*           length : ���ݳ���
*   ��  ��: ��
*   ��  ��: ����һ������
*/
void dm9k_send_packet(struct pbuf *p)
{
    struct pbuf *q = NULL;
    static uint8_t SendPackOk = 0;
    u16 pbuf_index = 0;
    u8 word[2], word_index = 0;
    int cnt = 0;

    q = p;

    xSemaphoreTake(s_xLockSemaphore, portMAX_DELAY);  /* ���󻥳��� */

    if(SendPackOk == Max_Send_Pack)
    {
        while(dm9k_ReadReg(DM9000_REG_TCR) & TCR_TXREQ)
        {
            dm9k_udelay(5);
        }
        SendPackOk = 0;
    }
    SendPackOk++;

    dm9k_WriteReg(DM9000_REG_IMR, IMR_PAR);             /* �ر� DM9000A �ж� */

    /*��DM9000��TX SRAM��д�����ݣ�һ��д�������ֽ�����
      ��Ҫ���͵����ݳ���Ϊ������ʱ��������Ҫ�����һ���ֽڵ���д��DM9000��TX SRAM�� */
    /* ��ʼ��ϵͳ�����ϰᵽ���ڴ��У�ÿ���ƶ�һ�� word */
    /* Move data to DM9000 TX RAM */
    NET_REG_ADDR = DM9000_REG_MWCMD;
    while(q)
    {
        cnt++;
        if(pbuf_index < q->len)
        {
            word[word_index++] = ((u8_t*)q->payload)[pbuf_index++];
            if(word_index == 2)
            {
                NET_REG_DATA = (uint16_t)((word[1] << 8) | word[0]);
                word_index = 0;
            }
        }
        else
        {
            q = q->next;
            pbuf_index = 0;
        }
    }
    if(word_index == 1)
    {
        NET_REG_DATA = (uint16_t)word[0];
    }
    dm9k_WriteReg(DM9000_REG_TXPLL, p->tot_len & 0xff);
    dm9k_WriteReg(DM9000_REG_TXPLH, (p->tot_len >> 8) & 0xff);  /* ���ô��ͷ���ĳ��� */
    dm9k_WriteReg(DM9000_REG_TCR, TCR_TXREQ);                   /* ���д��� */
    dm9k_WriteReg(DM9000_REG_IMR, IMR_PAR | IMR_PRM);           /* �����ж� */

    xSemaphoreGive(s_xLockSemaphore);     /* ���� */
}


/**
  * @brief  �жϴ�����
  * @param  None
  * @retval None
  */
void Dm9k_IrqHandle(void)
{
    uint8_t  save_reg;
    uint16_t isr_status;
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    save_reg = NET_REG_ADDR;                                /* �ݴ���ʹ�õ�λ�� */

    isr_status = dm9k_ReadReg(DM9000_REG_ISR);              /* ȡ���жϲ���ֵ */
    dm9k_WriteReg(DM9000_REG_ISR, isr_status);              /* ��� DM9000A �ж� */


    /* receive overflow */
    if(isr_status & ISR_ROS)
    {
        printk("dm9k overflow\n");
    }

    if(isr_status & ISR_ROOS)
    {
        printk("dm9k overflow counter overflow\n");
    }

    if(isr_status & ISR_PRS) /* ����Ƿ�Ϊ�����ж� */
    {
        /* Give the semaphore to wakeup LwIP task */
        xSemaphoreGiveFromISR(s_xInputSemaphore, &xHigherPriorityTaskWoken);
    }

    NET_REG_ADDR = save_reg;                            /* �ָ���ʹ�õ�λ�� */
}

/*******************************************************************************
*   ������: dm9k_ReadID
*   ��  ��: ��
*   ��  ��: ��
*   ��  ��: ��ȡоƬID
*/
uint32_t dm9k_ReadID(void)
{
    uint8_t vid1,vid2,pid1,pid2;

    vid1 = dm9k_ReadReg(DM9000_REG_VID_L) & 0xFF;
    vid2 = dm9k_ReadReg(DM9000_REG_VID_H) & 0xFF;
    pid1 = dm9k_ReadReg(DM9000_REG_PID_L) & 0xFF;
    pid2 = dm9k_ReadReg(DM9000_REG_PID_H) & 0xFF;

    return (vid2 << 24) | (vid1 << 16) | (pid2 << 8) | pid1;
}

/**
  * @brief
  * @param  None
  * @retval None
  */
u8 bsp_Dm9kGetMode(void)
{
    u8 temp = 0, i = 0;

    while(!(dm9k_phy_read(0X01) & 0X0020))  //�ȴ��Զ�Э�����
    {
        dm9k_udelay(50000);
        i++;
        if(i > 100)
        {
            return 0XFF;                    //�Զ�Э��ʧ��
        }
    }
    temp  = ((dm9k_ReadReg(DM9000_REG_NSR)>>6)&0X02);   //��ȡDM9000�������ٶ� 00:100M   10:10M
    temp |= ((dm9k_ReadReg(DM9000_REG_NCR)>>3)&0X01);   //��ȡDM9000��˫��״̬ 1:Full    0 : Half

    return temp;
}


/**
  * @brief
  * @param  None
  * @retval None
  */
int bsp_InitDm9k(void)
{
    uint32_t id = 0;
    uint8_t temp = 0;

    DM9K_GPIOConfig();
    DM9K_FSMCConfig();
    DM9K_NVICConfig();
    id = dm9k_ReadID();
    if(id == DM9000A_ID_OK)
    {
        dm9k_initFunc();

        temp = bsp_Dm9kGetMode();
        if(temp != 0XFF)                            //���ӳɹ���ͨ��������ʾ�����ٶȺ�˫��״̬
        {
            printk("DM9000 Speed:%dMbps,Duplex:%s duplex mode\r\n",(temp&0x02)?10:100,(temp&0x01)?"Full":"Half");
        }
        else
        {
            printk("DM9000 Establish Link Failed!\r\n");
        }
    }
    else
    {
        printf("Dm9000 init fail\r\n");
        return DM9000A_InitError;
    }
	return DM9000A_InitOK;
}

