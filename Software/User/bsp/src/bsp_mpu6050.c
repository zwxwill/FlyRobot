/**
  ******************************************************************************
  * @file    bsp_mpu6050.c
  * @author  zwx
  * @version V1.8.0
  * @date    2019-01-29
  * @brief   
  ******************************************************************************
  */  

#include "bsp_mpu6050.h"
#include "bsp_i2c.h"
#include "lib_math.h"
#include "srv_imu.h"
#include "bsp_mpu6050reg.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lib_filter.h"

#include "bsp_uart.h"

MPU6050_str MPU6050;

/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private struct  -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*低通滤波参数*/
#define GYRO_LPF_CUTOFF_FREQ  80
#define ACCEL_LPF_CUTOFF_FREQ 30
static lpf2pData accLpf[3];
static lpf2pData gyroLpf[3];

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
static void MPU6050_Delay(uint32_t n)
{
//	uint32_t i = 0, j=0;
//	
//	for(j=0;j<n;j++)
//		for(i=0; i<100; i++);
	vTaskDelay(n);	
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
static u8 MPU6050_ModeConfiguration(void)
{
	u8 temp_data = 0, i = 0;
	
	if(I2C1_ReadData(MPU6050_DEVICE_ADDRESS, WHO_AM_I, 1, &temp_data) == I2C1_READ_OK) // Check Whether MPU6050 is on I2C1
	{
		if(temp_data != MPU6050_ID)
		{
			printf("MPU6050 connection [FAIL].\n");
			return MPU6050_INIT_ERROR;  
		}
	}
	else
	{
		return MPU6050_INIT_ERROR; 
	}
	
	if(I2C1_WriteOneData(MPU6050_DEVICE_ADDRESS, PWR_MGMT_1, 0x80) == I2C1_WRITE_ERROR)    //  复位MPU6050
	{
		return MPU6050_INIT_ERROR; 
	}
	
	MPU6050_Delay(10);
	
	if(I2C1_WriteOneData(MPU6050_DEVICE_ADDRESS, PWR_MGMT_1, 0x00) == I2C1_WRITE_ERROR)    //  解除休眠状态
	{
		return MPU6050_INIT_ERROR; 
	}	
	
	MPU6050_Delay(10);
	
	if(I2C1_WriteOneData(MPU6050_DEVICE_ADDRESS, PWR_MGMT_1, 0x01) == I2C1_WRITE_ERROR)      // 设置备用时钟源，陀螺仪X轴
    {
        return MPU6050_INIT_ERROR;
    }	
	
	MPU6050_Delay(10);
	
	if(I2C1_WriteOneDataBit(MPU6050_DEVICE_ADDRESS, MPU6500_RA_PWR_MGMT_1, MPU6500_PWR1_TEMP_DIS_BIT, 0) == I2C1_WRITE_ERROR)      // 使能溫度传感器
    {
        return MPU6050_INIT_ERROR;
    }
	
	if(I2C1_WriteOneData(MPU6050_DEVICE_ADDRESS, MPU6500_RA_INT_ENABLE, 0x00) == I2C1_WRITE_ERROR)      // 关闭中断
    {
        return MPU6050_INIT_ERROR;
    }
	
    if(I2C1_WriteOneData(MPU6050_DEVICE_ADDRESS, INT_PIN_CFG, 0x02) == I2C1_WRITE_ERROR)    // ByPass EN 
    {                                                                                                       
        return MPU6050_INIT_ERROR;                          							  
    }
	
    if(I2C1_WriteOneData(MPU6050_DEVICE_ADDRESS, GYRO_CONFIG, 0x18) == I2C1_WRITE_ERROR)   // 陀螺仪自检及测量范围   2000deg/s  
    {
        return MPU6050_INIT_ERROR;                                                    
    }
	
    if(I2C1_WriteOneData(MPU6050_DEVICE_ADDRESS, ACCEL_CONFIG, 0x18) == I2C1_WRITE_ERROR)  // 加速度计自检及测量范围   +-16G
    {
        return MPU6050_INIT_ERROR;                                                   
    }
	
	if(I2C1_WriteOneData(MPU6050_DEVICE_ADDRESS, SMPLRT_DIV, 0x00) == I2C1_WRITE_ERROR)      // 最快采样速度
    {
        return MPU6050_INIT_ERROR;
    }

    if(I2C1_WriteOneData(MPU6050_DEVICE_ADDRESS, MPU_CONFIG, MPU6050_LPF_98HZ) == I2C1_WRITE_ERROR)   // 低通滤波    
    {                                                                                     
        return MPU6050_INIT_ERROR;
    }
	
	
	for (i = 0; i < 3; i++)// 初始化加速计和陀螺二阶低通滤波
	{
		lpf2pInit(&gyroLpf[i], 1000, GYRO_LPF_CUTOFF_FREQ);
		lpf2pInit(&accLpf[i],  1000, ACCEL_LPF_CUTOFF_FREQ);
	}
	
	return MPU6050_INIT_OK;
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
void bsp_Mpu6050Init(void)
{
	I2C1_Configuration();
	MPU6050_Delay(10000);
	if(MPU6050_ModeConfiguration() != MPU6050_INIT_OK)
	{
		while(1);
	}
	MPU6050_Delay(10000);
//	MPU6050.Lpf_Factor = Math_Get_LPF_1st_Factor((IMU_LOOP_TIME * 1e-6), ACC_LPF_CUT);	
//	MPU6050_Calibrate();
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
void MPU6050_ReadData(void)
{
	u8 mpu_buf[14] = {0};
	
	I2C1_ReadData(MPU6050_DEVICE_ADDRESS, MPU6050_DATA_START, 14, mpu_buf);

	// 原始数据
	MPU6050.Raw.AccX = (((int16_t)mpu_buf[0]<<8) | mpu_buf[1]) - MPU6050.Offset.AccX;
	MPU6050.Raw.AccY = (((int16_t)mpu_buf[2]<<8) | mpu_buf[3]) - MPU6050.Offset.AccY;
	MPU6050.Raw.AccZ = (((int16_t)mpu_buf[4]<<8) | mpu_buf[5]) - MPU6050.Offset.AccZ;
	
	MPU6050.Raw.GyroX = (((int16_t)mpu_buf[8]<<8)  | mpu_buf[9]) - MPU6050.Offset.GyroX;
	MPU6050.Raw.GyroY = (((int16_t)mpu_buf[10]<<8) | mpu_buf[11]) - MPU6050.Offset.GyroY;
	MPU6050.Raw.GyroZ = (((int16_t)mpu_buf[12]<<8) | mpu_buf[13]) - MPU6050.Offset.GyroZ;
	
	// 单位变换
	MPU6050.Chg.AccX = (float)MPU6050.Raw.AccX;
	MPU6050.Chg.AccY = (float)MPU6050.Raw.AccY;
	MPU6050.Chg.AccZ = (float)MPU6050.Raw.AccZ;
	
	MPU6050.Chg.GyroX = Math_DegToRag((float)MPU6050.Raw.GyroX * MPU6050G_S2000DPS);
	MPU6050.Chg.GyroY = Math_DegToRag((float)MPU6050.Raw.GyroY * MPU6050G_S2000DPS);
	MPU6050.Chg.GyroZ = Math_DegToRag((float)MPU6050.Raw.GyroZ * MPU6050G_S2000DPS);
	
	// 滤波
	MPU6050.Fil.AccX = Math_LPF_1st(MPU6050.Fil.AccX, MPU6050.Chg.AccX, MPU6050.Lpf_Factor);
	MPU6050.Fil.AccY = Math_LPF_1st(MPU6050.Fil.AccY, MPU6050.Chg.AccY, MPU6050.Lpf_Factor);
	MPU6050.Fil.AccZ = Math_LPF_1st(MPU6050.Fil.AccZ, MPU6050.Chg.AccZ, MPU6050.Lpf_Factor);
	
	MPU6050.Fil.GyroX = MPU6050.Chg.GyroX;
	MPU6050.Fil.GyroY = MPU6050.Chg.GyroY;
	MPU6050.Fil.GyroZ = MPU6050.Chg.GyroZ;	
}
void bsp_mpu6050ReadData(uint8_t *buf)
{
	I2C1_ReadData(MPU6050_DEVICE_ADDRESS, MPU6050_DATA_START, 14, buf);
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
uint8_t MPU6050_GetGyro(s16 *gx, s16 *gy, s16 *gz)
{
	uint8_t buf[6]={0}, res=0;
	
	res = I2C1_ReadData(MPU6050_DEVICE_ADDRESS, GYRO_XOUT_H, 6, buf);
	if(res == I2C1_READ_OK)
	{
		*gx = ((uint16_t)buf[0]<<8) | buf[1];
		*gy = ((uint16_t)buf[2]<<8) | buf[3];
		*gz = ((uint16_t)buf[4]<<8) | buf[5];
	}
	return res;
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
uint8_t MPU6050_GetAcc(s16 *ax, s16 *ay, s16 *az)
{
	uint8_t buf[6]={0}, res=0;
	
	res = I2C1_ReadData(MPU6050_DEVICE_ADDRESS, ACCEL_XOUT_H, 6, buf);
	if(res == I2C1_READ_OK)
	{
		*ax = ((uint16_t)buf[0]<<8) | buf[1];
		*ay = ((uint16_t)buf[2]<<8) | buf[3];
		*az = ((uint16_t)buf[4]<<8) | buf[5];
	}
	return res;
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
uint8_t MPU6050_GetTmp(s16 *temp)
{
	uint8_t buf[2]={0}, res=0;
	int16_t raw = 0;
	
	res = I2C1_ReadData(MPU6050_DEVICE_ADDRESS, TEMP_OUT_H, 2, buf);
	if(res == I2C1_READ_OK)
	{
		raw = ((uint16_t)buf[0]<<8) | buf[1];
	}
	*temp = (36.53+((double)raw)/340)/100;  
	return res;
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
#define CALIBRATE_NUM 	10
void MPU6050_Calibrate(void)
{
	u8 i = 0;
	s16 sumax=0, sumay=0, sumaz=0;
	s16 sumgx=0, sumgy=0, sumgz=0;
	u8 mpu_buf[14] = {0};

	for(i=0; i<10; i++)
	{		
		MPU6050_Delay(1000);		
		I2C1_ReadData(MPU6050_DEVICE_ADDRESS, MPU6050_DATA_START, 14, mpu_buf);			
	}
	
	for(i=0; i<CALIBRATE_NUM; i++)
	{		
		MPU6050_Delay(1000);
		
		I2C1_ReadData(MPU6050_DEVICE_ADDRESS, MPU6050_DATA_START, 14, mpu_buf);
		
		// 原始数据
		sumax += (((int16_t)mpu_buf[0]<<8) | mpu_buf[1]);
		sumay += (((int16_t)mpu_buf[2]<<8) | mpu_buf[3]);
		sumaz += (((int16_t)mpu_buf[4]<<8) | mpu_buf[5]);
		
		sumgx += (((int16_t)(mpu_buf[8]<<8) | mpu_buf[9]));
		sumgy += (((int16_t)(mpu_buf[10]<<8) | mpu_buf[11]));
		sumgz += (((int16_t)(mpu_buf[12]<<8) | mpu_buf[13]));			
	}
	sumax /= CALIBRATE_NUM;
	sumay /= CALIBRATE_NUM;
	sumaz /= CALIBRATE_NUM;
	sumgx /= CALIBRATE_NUM;
	sumgx /= CALIBRATE_NUM;
	sumgx /= CALIBRATE_NUM;
	
//	MPU6050.Offset.AccX = sumax;
//	MPU6050.Offset.AccY = sumay;
//	MPU6050.Offset.AccZ = sumaz;
	
	MPU6050.Offset.GyroX = sumgx;
	MPU6050.Offset.GyroY = sumgy;
	MPU6050.Offset.GyroZ = sumgz;
}


/****************** 北京龙鼎源科技 http://www.bdrl.com.cn/ ********************/

