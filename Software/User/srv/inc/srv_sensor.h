/**
  ******************************************************************************
  * @file    srv_sensor.h
  * @author  zwx
  * @version V1.0.0
  * @date    2019-07-14
  * @brief   sensor service layer
  ******************************************************************************
  */  

#ifndef __SRV_SENSOR_H__
#define __SRV_SENSOR_H__

#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__CC_ARM) 
	#pragma anon_unions
#endif

/* Exported define -----------------------------------------------------------*/
#define MPU6050_BUF_LEN                 14
#define SENSORS_NBR_OF_BIAS_SAMPLES		1024	/* N */
#define GYRO_VARIANCE_THRESHOLD			4000    /* Gyro deviation threshold */
#define SENSORS_ACCEL_FS_CFG            MPU6500_ACCEL_FS_16	
#define SENSORS_G_PER_LSB_CFG           MPU6500_G_PER_LSB_16
#define SENSORS_ACC_SCALE_SAMPLES  		200		/* ACC sample count */
/* Exported typedef ----------------------------------------------------------*/
typedef union 
{
	struct
	{
		int16_t x;
		int16_t y;
		int16_t z;
	};
	int16_t axis[3];
} Axis3i16;

typedef union 
{
	struct 
	{
		int32_t x;
		int32_t y;
		int32_t z;
	};
	int32_t axis[3];
} Axis3i32;

typedef union 
{
	struct 
	{
		int64_t x;
		int64_t y;
		int64_t z;
	};
	int64_t axis[3];
} Axis3i64;

typedef union 
{
	struct 
	{
		float x;
		float y;
		float z;
	};
	float axis[3];
} Axis3f;

/* Exported struct  ----------------------------------------------------------*/
typedef struct Sensor_Mpu6050
{
	uint8_t  Buf[MPU6050_BUF_LEN];
	Axis3i16 AccRaw;
	Axis3i16 GyroRaw;
	Axis3f   GyroBias;
	Axis3f 	 Acc;
	Axis3f 	 Gyro;	
}MPU6050_Str;
	
typedef struct
{
	Axis3f     bias; /* Æ«²î */
	uint8_t    isBiasValueFound;
	uint8_t    isBufferFilled;
	Axis3i16*  bufHead;
	Axis3i16   buffer[SENSORS_NBR_OF_BIAS_SAMPLES];
}BiasObj_Str;
/* Exported variables --------------------------------------------------------*/
extern MPU6050_Str Mpu6050; 

/* Exported functions --------------------------------------------------------*/
extern void ProcessAccGyroMeasurements(void);

#ifdef __cplusplus
}
#endif

#endif

/*********************************************************************************/
