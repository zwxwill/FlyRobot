/**
  ******************************************************************************
  * @file    bsp_flash.c
  * @author  zwx
  * @version V1.8.0
  * @date    2019-01-29
  * @brief
  ******************************************************************************
  */

#include "bsp_flash.h"
#include <string.h>

/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SPI_FLASH         SPI2
/* ƬѡGPIO�˿�  */
#define SF_CS_GPIO        GPIOB
#define SF_CS_PIN         GPIO_Pin_12
/* Ƭѡ�����õ�ѡ��  */
#define SF_CS_LOW()       SF_CS_GPIO->BSRRH = SF_CS_PIN
/* Ƭѡ�����ø߲�ѡ�� */
#define SF_CS_HIGH()      SF_CS_GPIO->BSRRL = SF_CS_PIN
#define ENABLE_SPI_RCC()  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE)
/*
    ��SPIʱ�������2��Ƶ����֧�ֲ���Ƶ��
    �����SPI1��2��ƵʱSCKʱ�� = 42M��4��ƵʱSCKʱ�� = 21M
    �����SPI2, 2��ƵʱSCKʱ�� = 21M
*/
#define SPI_BAUD          SPI_BaudRatePrescaler_2


#define CMD_AAI       0xAD      /* AAI �������ָ��(FOR SST25VF016B) */
#define CMD_DISWR     0x04      /* ��ֹд, �˳�AAI״̬ */
#define CMD_EWRSR     0x50      /* ����д״̬�Ĵ��������� */
#define CMD_WRSR      0x01      /* д״̬�Ĵ������� */
#define CMD_WREN      0x06      /* дʹ������ */
#define CMD_READ      0x03      /* ������������ */
#define CMD_RDSR      0x05      /* ��״̬�Ĵ������� */
#define CMD_RDID      0x9F      /* ������ID���� */
#define CMD_SE        0x20      /* ������������ */
#define CMD_BE        0xC7      /* ������������ */
#define CMD_PGWR      0x02      /* дҳ */

#define DUMMY_BYTE    0xA5      /* ���������Ϊ����ֵ�����ڶ����� */

#define WIP_FLAG      0x01      /* ״̬�Ĵ����е����ڱ�̱�־��WIP) */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static SFLASH_T g_tSF;
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  ����������һ���ֽڣ�ͬʱ��MISO���߲����������ص�����
  * @param  None
  * @retval None
  */
static uint8_t bsp_FlashSendByte(uint8_t _ucValue)
{
    /* �ȴ��ϸ�����δ������� */
    while(SPI_I2S_GetFlagStatus(SPI_FLASH, SPI_I2S_FLAG_TXE) == RESET);

    /* ͨ��SPIӲ������1���ֽ� */
    SPI_I2S_SendData(SPI_FLASH, _ucValue);

    /* �ȴ�����һ���ֽ�������� */
    while(SPI_I2S_GetFlagStatus(SPI_FLASH, SPI_I2S_FLAG_RXNE) == RESET);

    /* ���ش�SPI���߶��������� */
    return SPI_I2S_ReceiveData(SPI_FLASH);
}

/**
  * @brief
  * @param  None
  * @retval None
  */

static uint32_t bsp_FlashReadID(void)
{
    uint32_t uiID;
    uint8_t id1, id2, id3;

    SF_CS_LOW();                                          /* ʹ��Ƭѡ */
    bsp_FlashSendByte(CMD_RDID);                          /* ���Ͷ�ID���� */
    id1 = bsp_FlashSendByte(DUMMY_BYTE);                  /* ��ID�ĵ�1���ֽ� */
    id2 = bsp_FlashSendByte(DUMMY_BYTE);                  /* ��ID�ĵ�2���ֽ� */
    id3 = bsp_FlashSendByte(DUMMY_BYTE);                  /* ��ID�ĵ�3���ֽ� */
    SF_CS_HIGH();                                         /* ����Ƭѡ */

    uiID = ((uint32_t)id1 << 16) | ((uint32_t)id2 << 8) | id3;

    return uiID;
}

/**
  * @brief  ��ȡ����ID,�������������
  * @param  None
  * @retval None
  */
static void bsp_ReadFlashInfo(void)
{
    g_tSF.ChipID = bsp_FlashReadID(); /* оƬID */

    switch(g_tSF.ChipID)
    {
        case SST25VF016B_ID:
            strcpy(g_tSF.ChipName, "SST25VF016B");
            g_tSF.TotalSize = 2 * 1024 * 1024;  /* ������ = 2M */
            g_tSF.PageSize = 4 * 1024;          /* ҳ���С = 4K */
            break;

        case MX25L1606E_ID:
            strcpy(g_tSF.ChipName, "MX25L1606E");
            g_tSF.TotalSize = 2 * 1024 * 1024;  /* ������ = 2M */
            g_tSF.PageSize = 4 * 1024;          /* ҳ���С = 4K */
            break;

        case W25Q64BV_ID:
            strcpy(g_tSF.ChipName, "W25Q64BV");
            g_tSF.TotalSize = 8 * 1024 * 1024;  /* ������ = 16M */
            g_tSF.PageSize = 4 * 1024;          /* ҳ���С = 4K */
            break;

        case W25Q128BV_ID:
            strcpy(g_tSF.ChipName, "W25Q128BV");
            g_tSF.TotalSize = 16 * 1024 * 1024; /* ������ = 8M */
            g_tSF.PageSize = 4 * 1024;          /* ҳ���С = 4K */
            break;

        default:
            strcpy(g_tSF.ChipName, "Unknow Flash");
            g_tSF.TotalSize = 2 * 1024 * 1024;
            g_tSF.PageSize = 4 * 1024;
            break;
    }
}

/**
  * @brief  ����������дʹ������
  * @param  None
  * @retval None
  */
static void bsp_FlashWriteEnable(void)
{
    SF_CS_LOW();                                    /* ʹ��Ƭѡ */
    bsp_FlashSendByte(CMD_WREN);                    /* �������� */
    SF_CS_HIGH();                                   /* ����Ƭѡ */
}

/**
  * @brief  д״̬�Ĵ���
  * @param  None
  * @retval None
  */
static void bsp_FlashWriteStatus(uint8_t _ucValue)
{
    if(g_tSF.ChipID == SST25VF016B_ID)
    {
        /* ��1������ʹ��д״̬�Ĵ��� */
        SF_CS_LOW();                                    /* ʹ��Ƭѡ */
        bsp_FlashSendByte(CMD_EWRSR);                   /* ������� ����д״̬�Ĵ��� */
        SF_CS_HIGH();                                   /* ����Ƭѡ */

        /* ��2������д״̬�Ĵ��� */
        SF_CS_LOW();                                    /* ʹ��Ƭѡ */
        bsp_FlashSendByte(CMD_WRSR);                    /* ������� д״̬�Ĵ��� */
        bsp_FlashSendByte(_ucValue);                    /* �������ݣ�״̬�Ĵ�����ֵ */
        SF_CS_HIGH();                                   /* ����Ƭѡ */
    }
    else
    {
        SF_CS_LOW();                                    /* ʹ��Ƭѡ */
        bsp_FlashSendByte(CMD_WRSR);                    /* ������� д״̬�Ĵ��� */
        bsp_FlashSendByte(_ucValue);                    /* �������ݣ�״̬�Ĵ�����ֵ */
        SF_CS_HIGH();                                   /* ����Ƭѡ */
    }
}

/**
  * @brief  ����ѭ����ѯ�ķ�ʽ�ȴ������ڲ�д�������
  * @param  None
  * @retval None
  */
static void bsp_FlashWaitForWriteEnd(void)
{
    SF_CS_LOW();                                        /* ʹ��Ƭѡ */
    bsp_FlashSendByte(CMD_RDSR);                        /* ������� ��״̬�Ĵ��� */
    while((bsp_FlashSendByte(DUMMY_BYTE) & WIP_FLAG) == SET); /* �ж�״̬�Ĵ�����æ��־λ */
    SF_CS_HIGH();                                       /* ����Ƭѡ */
}

/**
  * @brief
  * @param  None
  * @retval None
  */
void bsp_InitSpiFlash(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;

    /* ʹ��GPIO ʱ�� */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    /* ���� SCK, MISO �� MOSI Ϊ���ù��� */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI1);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* ����Ƭѡ����Ϊ�������ģʽ */
    SF_CS_HIGH();       /* Ƭѡ�øߣ���ѡ�� */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_Init(GPIOB, &GPIO_InitStructure);


    /* ����SPIӲ���������ڷ��ʴ���Flash */
    /* ��SPIʱ�� */
    ENABLE_SPI_RCC();

    /* ����SPIӲ������ */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  /* ���ݷ���2��ȫ˫�� */
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;       /* STM32��SPI����ģʽ ������ģʽ */
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;   /* ����λ���� �� 8λ */
    /* SPI_CPOL��SPI_CPHA���ʹ�þ���ʱ�Ӻ����ݲ��������λ��ϵ��
       ��������: ���߿����Ǹߵ�ƽ,��2�����أ������ز�������)
    */
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;         /* ʱ�������ز������� */
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;        /* ʱ�ӵĵ�2�����ز������� */
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;           /* Ƭѡ���Ʒ�ʽ��������� */

    /* ���ò�����Ԥ��Ƶϵ�� */
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BAUD;

    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  /* ����λ������򣺸�λ�ȴ� */
    SPI_InitStructure.SPI_CRCPolynomial = 7;            /* CRC����ʽ�Ĵ�������λ��Ϊ7�������̲��� */
    SPI_Init(SPI_FLASH, &SPI_InitStructure);

    SPI_Cmd(SPI_FLASH, DISABLE);            /* �Ƚ�ֹSPI  */

    SPI_Cmd(SPI_FLASH, ENABLE);             /* ʹ��SPI  */

    bsp_ReadFlashInfo();              /* �Զ�ʶ��оƬ�ͺ� */

    SF_CS_LOW();                /* �����ʽ��ʹ�ܴ���FlashƬѡ */
    bsp_FlashSendByte(CMD_DISWR);     /* ���ͽ�ֹд�������,��ʹ�����д���� */
    SF_CS_HIGH();               /* �����ʽ�����ܴ���FlashƬѡ */

    bsp_FlashWaitForWriteEnd();       /* �ȴ�����Flash�ڲ�������� */

    bsp_FlashWriteStatus(0);          /* �������BLOCK��д���� */
}

/**
  * @brief
  * @param  None
  * @retval None
  */
uint8_t bsp_SpiReadWriteByte(uint8_t ucData)
{
    uint8_t ucReadData = 0;

    /* �ȴ����ͻ������� */
    while(SPI_I2S_GetFlagStatus(SPI_FLASH, SPI_I2S_FLAG_TXE) == RESET);

    /* ����һ���ֽ� */
    SPI_I2S_SendData(SPI_FLASH, ucData);

    /* �ȴ����ݽ������ */
    while(SPI_I2S_GetFlagStatus(SPI_FLASH, SPI_I2S_FLAG_RXNE) == RESET);

    /* ��ȡ���յ������� */
    ucReadData = SPI_I2S_ReceiveData(SPI_FLASH);

    return ucReadData;
}

/**
  * @brief
  * @param  None
  * @retval None
  */
void bsp_EraseSector(uint32_t _uiSectorAddr)
{
    bsp_FlashWriteEnable();                         /* ����дʹ������ */

    /* ������������ */
    SF_CS_LOW();                                    /* ʹ��Ƭѡ */
    bsp_SpiReadWriteByte(CMD_SE);                                /* ���Ͳ������� */
    bsp_SpiReadWriteByte((_uiSectorAddr & 0xFF0000) >> 16);  /* ����������ַ�ĸ�8bit */
    bsp_SpiReadWriteByte((_uiSectorAddr & 0xFF00) >> 8);     /* ����������ַ�м�8bit */
    bsp_SpiReadWriteByte(_uiSectorAddr & 0xFF);              /* ����������ַ��8bit */
    SF_CS_HIGH();                                   /* ����Ƭѡ */

    bsp_FlashWaitForWriteEnd();                     /* �ȴ�����Flash�ڲ�д������� */
}

/**
  * @brief
  * @param  None
  * @retval None
  */
void bsp_FlashRead(uint32_t sector_id, uint8_t *pData, uint32_t length)
{
    uint32_t addr = 0;

    /* �����ȡ�����ݳ���Ϊ0���߳�������Flash��ַ�ռ䣬��ֱ�ӷ��� */
    if((sector_id > SECTOR_NUM) ||(pData == (uint8_t *)0))
    {
        return;
    }
    addr = SECTOR_ADDR(sector_id);

    SF_CS_LOW();                                    /* ʹ��Ƭѡ */
    bsp_SpiReadWriteByte(CMD_READ);                         /* ���Ͷ����� */
    bsp_SpiReadWriteByte((addr & 0xFF0000) >> 16);  /* ����������ַ�ĸ�8bit */
    bsp_SpiReadWriteByte((addr & 0xFF00) >> 8);     /* ����������ַ�м�8bit */
    bsp_SpiReadWriteByte(addr & 0xFF);              /* ����������ַ��8bit */
    while(length--)
    {
        *pData++ = bsp_SpiReadWriteByte(ERASE_BYTE); /* ��һ���ֽڲ��洢��pBuf�������ָ���Լ�1 */
    }
    SF_CS_HIGH();                                   /* ����Ƭѡ */
}

/**
  * @brief
  * @param  None
  * @retval None
  */
int bsp_ClearSectors(int iSecorId, int length)
{
    int i = 0;
    int iSectorNum = 0;

    if((iSecorId > SECTOR_NUM) || (length < 0))
    {
        return FLASH_ERASE_ERROR;
    }

    /*Calculate the number of sectors needed*/
    iSectorNum = (uint32_t)length / SECTOR_SIZE;
    if(length % SECTOR_SIZE)
    {
        iSectorNum++;
    }

    /*Erase sectors before write data into*/
    for(i = 0; i < iSectorNum; i++)
    {
        bsp_EraseSector(SECTOR_ADDR(iSectorNum + i));
    }

    return FLASH_ERASE_OK;
}

/**
  * @brief
  * @param  None
  * @retval None
  */
void bsp_FlashPageWrite(uint8_t * buf, uint32_t addr, uint16_t length)
{
    uint32_t i = 0;

    bsp_FlashWriteEnable();
    SF_CS_LOW();
    bsp_SpiReadWriteByte(CMD_PGWR);
    bsp_SpiReadWriteByte((uint8_t)(addr >> 16));
    bsp_SpiReadWriteByte((uint8_t)(addr >> 8));
    bsp_SpiReadWriteByte((uint8_t)addr);
    for(i = 0; i < length; i++)
    {
        bsp_SpiReadWriteByte(buf[i]);
    }

    SF_CS_HIGH();
    bsp_FlashWaitForWriteEnd();
}
/**
  * @brief
  * @param  None
  * @retval None
  */
int bsp_FlashWrite(int sector_id, uint8_t *blob, int length)
{
    uint32_t num_sectors;
    uint32_t num_pages;
    uint32_t bytes_ramin;
    uint32_t addr;
    uint32_t i, j;

    if((sector_id >= SECTOR_NUM) || (blob == (uint8_t *)0) || (length < 0))
    {
        return FLASH_WRITE_ERROR;
    }

    /*Calculate the number of sectors needed*/
    num_sectors = (uint32_t)length / SECTOR_SIZE;
    if(length % SECTOR_SIZE)
    {
        num_sectors++;
    }

    /*Erase sectors before write data into*/
    for(i = 0; i < num_sectors; i++)
    {
        bsp_EraseSector(SECTOR_ADDR(sector_id + i));
    }
    /*Calculate the number of pages that will be fully used*/
    num_pages = (uint32_t)length / PAGE_SIZE;
    bytes_ramin = length % PAGE_SIZE;
    /*Write data into corresponding pages*/
    addr = SECTOR_ADDR(sector_id);//Calculate the start address
    for(j = 0; j < num_pages; j++)
    {
        bsp_FlashPageWrite(blob + j * PAGE_SIZE, addr + j * PAGE_SIZE, PAGE_SIZE);
    }
    if(bytes_ramin)
    {
        bsp_FlashPageWrite(blob + j * PAGE_SIZE, addr + j * PAGE_SIZE, bytes_ramin);
    }

    return FLASH_WRITE_OK;
}
















/****************** ��������Դ�Ƽ� http://www.bdrl.com.cn/ ********************/
