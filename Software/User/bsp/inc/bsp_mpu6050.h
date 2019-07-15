/**
  ******************************************************************************
  * @file    bsp_mpu6050.h
  * @author  zwx
  * @version V1.8.0
  * @date    2019-01-29
  * @brief   
  ******************************************************************************
  */  

#ifndef __BSP_MPU6050_H__
#define __BSP_MPU6050_H__

#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Debug switch section ------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
	//定义MPU6050内部地址
	#define MPU6050_DEVICE_ADDRESS    0xD0
	#define	WHO_AM_I		          0x75	//ReadOnly   MPU6050_ID = 0x68
	#define MPU6050_ID                0x68

	#define	PWR_MGMT_1		          0x6B	//电源管理 典型值 0x00 正常启用
	#define	MPU_CONFIG			      0x1A	//低通滤波频率 典型值 0x00 

	#define	SMPLRT_DIV		          0x19	//陀螺仪采样率 典型值 0X07 125Hz

	#define	GYRO_CONFIG		          0x1B	//陀螺仪自检及测量范围                 典型值 0x18 不自检 2000deg/s
	#define	ACCEL_CONFIG	          0x1C	//加速度计自检及测量范围及高通滤波频率 典型值 0x01 不自检 2G 5Hz
	#define INT_PIN_CFG               0x37 
	#define INT_ENABLE                0x38
	#define INT_STATUS                0x3A  //只读
	#define USER_CTRL                 0x6A

	#define	ACCEL_XOUT_H	          0x3B
	#define	ACCEL_XOUT_L	          0x3C
	#define	ACCEL_YOUT_H	          0x3D
	#define	ACCEL_YOUT_L	          0x3E
	#define	ACCEL_ZOUT_H	          0x3F
	#define	ACCEL_ZOUT_L	          0x40
	#define	TEMP_OUT_H		          0x41
	#define	TEMP_OUT_L		          0x42
	#define	GYRO_XOUT_H		          0x43
	#define	GYRO_XOUT_L		          0x44	
	#define	GYRO_YOUT_H		          0x45
	#define	GYRO_YOUT_L		          0x46
	#define	GYRO_ZOUT_H		          0x47
	#define	GYRO_ZOUT_L		          0x48
	#define MPU6050_DATA_START        ACCEL_XOUT_H   //由于数据存放地址是连续的，所以一并读出

	#define MPU6050_INIT_ERROR		  (0u)
	#define MPU6050_INIT_OK           (1u)
	
	#define MPU6050_LPF_256HZ       0
	#define MPU6050_LPF_188HZ       1
	#define MPU6050_LPF_98HZ        2
	#define MPU6050_LPF_42HZ        3
	#define MPU6050_LPF_20HZ        4
	#define MPU6050_LPF_10HZ        5
	#define MPU6050_LPF_5HZ         6
	
	#define MPU6050G_S250DPS            ((float)0.0076335f)  // 0.0087500 dps/LSB
	#define MPU6050G_S500DPS            ((float)0.0152671f)  // 0.0175000 dps/LSB
	#define MPU6050G_S2000DPS           ((float)0.0609756f)  // 0.0700000 dps/LSB
	
	#define MPU6500_G_PER_LSB_2      	(float)((2 * 2) / 65536.0)
	#define MPU6500_G_PER_LSB_4     	(float)((2 * 4) / 65536.0)
	#define MPU6500_G_PER_LSB_8         (float)((2 * 8) / 65536.0)
	#define MPU6500_G_PER_LSB_16        (float)((2 * 16) / 65536.0)

	
/* Exported struct -----------------------------------------------------------*/
// 校验数据
typedef struct
{
	s16 AccX;
	s16 AccY;
	s16 AccZ;
	s16 GyroX;
	s16 GyroY;
	s16 GyroZ;
}Offset_str;

// 原始数据
typedef struct
{
	s16 AccX;
	s16 AccY;
	s16 AccZ;
	s16 GyroX;
	s16 GyroY;
	s16 GyroZ;
}Raw_str;

// 数据单位变换后
typedef struct
{
	float AccX;
	float AccY;
	float AccZ;
	float GyroX;
	float GyroY;
	float GyroZ;
}Chg_str;

// 滤波后的数据
typedef struct
{
	float AccX;
	float AccY;
	float AccZ;
	float GyroX;
	float GyroY;
	float GyroZ;
}Fil_str;

typedef struct
{
	float Pitch;
	float Roll;
	float Yaw;
}Angle_Str;
 
typedef struct
{
	Raw_str Raw;
	Chg_str Chg; 
	Fil_str Fil;
	Angle_Str  Ang;
	Offset_str Offset;
	float Lpf_Factor;
}MPU6050_str;

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern MPU6050_str MPU6050;
/* Exported functions ------------------------------------------------------- */
extern void bsp_Mpu6050Init(void);
extern void MPU6050_ReadData(void);
extern uint8_t MPU6050_GetGyro(s16 *gx, s16 *gy, s16 *gz);
extern uint8_t MPU6050_GetAcc(s16 *ax, s16 *ay, s16 *az);
extern uint8_t MPU6050_GetTmp(s16 *temp);
extern void MPU6050_Calibrate(void);

extern void bsp_mpu6050ReadData(uint8_t *buf);

#ifdef __cplusplus
}
#endif

#endif

/****************** 北京龙鼎源科技 http://www.bdrl.com.cn/ ********************/

