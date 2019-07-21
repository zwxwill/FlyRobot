/**
  ******************************************************************************
  * @file    srv_sensor.c
  * @author  zwx
  * @version V1.0.0
  * @date    2019-07-14
  * @brief   sensor service layer
  ******************************************************************************
  */  

#include "srv_sensor.h"
#include "bsp_mpu6050.h"
#include "bsp_i2c.h"
#include <math.h>
#include "bsp.h"
#include "lib_filter.h"

/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/


/* Private struct  -----------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
MPU6050_Str Mpu6050;  /* MPU6050 Variable */
static BiasObj_Str BiasObj;  /* Bias object */
static float       AccScale;
extern lpf2pData gyroLpf[3];
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  processAccGyroMeasurements
  * @param  None
  * @retval None
  */
void SensorAddBiasValue(void)
{
	BiasObj.bufHead->x = Mpu6050.GyroRaw.x;
	BiasObj.bufHead->y = Mpu6050.GyroRaw.y;
	BiasObj.bufHead->z = Mpu6050.GyroRaw.z;
	BiasObj.bufHead++;

	if(BiasObj.bufHead >= &BiasObj.buffer[SENSORS_NBR_OF_BIAS_SAMPLES])
	{
		BiasObj.bufHead = BiasObj.buffer;
		BiasObj.isBufferFilled = TRUE;
	}
}

/**
  * @brief  SensorCalcMeanAndVariance
  * @param  None
  * @retval None
  */
void SensorCalcMeanAndVariance(Axis3f* varOut, Axis3f* meanOut)
{
	uint32_t i        = 0;
	int64_t  sum[3]   = {0};
	int64_t  sumsq[3] = {0};

	for (i = 0; i < SENSORS_NBR_OF_BIAS_SAMPLES; i++)
	{
		sum[0]   += BiasObj.buffer[i].x;
		sum[1]   += BiasObj.buffer[i].y;
		sum[2]   += BiasObj.buffer[i].z;
		sumsq[0] += BiasObj.buffer[i].x * BiasObj.buffer[i].x;
		sumsq[1] += BiasObj.buffer[i].y * BiasObj.buffer[i].y;
		sumsq[2] += BiasObj.buffer[i].z * BiasObj.buffer[i].z;
	}	

	varOut->x = (sumsq[0] - ((int64_t)sum[0] * sum[0]) / SENSORS_NBR_OF_BIAS_SAMPLES);
	varOut->y = (sumsq[1] - ((int64_t)sum[1] * sum[1]) / SENSORS_NBR_OF_BIAS_SAMPLES);
	varOut->z = (sumsq[2] - ((int64_t)sum[2] * sum[2]) / SENSORS_NBR_OF_BIAS_SAMPLES);

	meanOut->x = (float)sum[0] / SENSORS_NBR_OF_BIAS_SAMPLES;
	meanOut->y = (float)sum[1] / SENSORS_NBR_OF_BIAS_SAMPLES;
	meanOut->z = (float)sum[2] / SENSORS_NBR_OF_BIAS_SAMPLES;
}


/**
  * @brief  SensorFindBiasValue
  * @param  None
  * @retval None
  */
void SensorFindBiasValue(void)
{
	Axis3f mean;     
	Axis3f variance;  
	
	if(BiasObj.isBufferFilled)
	{
		SensorCalcMeanAndVariance(&variance, &mean);

		if((variance.x < GYRO_VARIANCE_THRESHOLD) && (variance.y < GYRO_VARIANCE_THRESHOLD) && (variance.z < GYRO_VARIANCE_THRESHOLD))
		{
			BiasObj.bias.x = mean.x;
			BiasObj.bias.y = mean.y;
			BiasObj.bias.z = mean.z;
			BiasObj.isBiasValueFound = TRUE;
		}
		else
		{
			BiasObj.isBufferFilled = FALSE;
		}
	}
}


/**
  * @brief  processAccGyroMeasurements
  * @param  None
  * @retval None
  */
void ProcessGyroBias(void)
{
	SensorAddBiasValue();
	if(!BiasObj.isBiasValueFound)
	{
		SensorFindBiasValue();
	}
	Mpu6050.GyroBias.x = BiasObj.bias.x;
	Mpu6050.GyroBias.y = BiasObj.bias.y;
	Mpu6050.GyroBias.z = BiasObj.bias.z;
}


/**
  * @brief  ProcessAccScale
  * @param  None
  * @retval None
  */
void ProcessAccScale(int16_t ax, int16_t ay, int16_t az)
{
	static uint8_t accBiasFound = FALSE;
	static uint32_t accScaleSumCount = 0;
	static float accScaleSum = 0;

	if(!accBiasFound)
	{
		accScaleSum += sqrtf(powf(ax * SENSORS_G_PER_LSB_CFG, 2) + powf(ay * SENSORS_G_PER_LSB_CFG, 2) + powf(az * SENSORS_G_PER_LSB_CFG, 2));
		accScaleSumCount++;

		if(accScaleSumCount == SENSORS_ACC_SCALE_SAMPLES)
		{
			AccScale = accScaleSum / accScaleSumCount;
			accBiasFound = TRUE;
		}
	}
}

/*¶þ½×µÍÍ¨ÂË²¨*/
static void applyAxis3fLpf(lpf2pData *data, Axis3f* in)
{
	u8 i = 0;
	
	for (i = 0; i < 3; i++) 
	{
		in->axis[i] = lpf2pApply(&data[i], in->axis[i]);
	}
}

/**
  * @brief  processAccGyroMeasurements
  * @param  None
  * @retval None
  */
void ProcessAccGyroMeasurements(void)
{
	u8 mpu_buf[14] = {0};
	int16_t ax, ay, az, gx, gy, gz;
	
	I2C1_ReadData(MPU6050_DEVICE_ADDRESS, MPU6050_DATA_START, 14, mpu_buf);
	
	ax = (((int16_t)mpu_buf[0]<<8)  | mpu_buf[1]);
	ay = (((int16_t)mpu_buf[2]<<8)  | mpu_buf[3]);
	az = (((int16_t)mpu_buf[4]<<8)  | mpu_buf[5]);
	gx = (((int16_t)mpu_buf[8]<<8)  | mpu_buf[9]);
	gy = (((int16_t)mpu_buf[10]<<8) | mpu_buf[11]);
	gz = (((int16_t)mpu_buf[12]<<8) | mpu_buf[13]);	
	
	Mpu6050.AccRaw.x  = ax;
	Mpu6050.AccRaw.y  = ay;
	Mpu6050.AccRaw.z  = az;
	
	Mpu6050.GyroRaw.x = gx - Mpu6050.GyroBias.x;
	Mpu6050.GyroRaw.y = gy - Mpu6050.GyroBias.y;
	Mpu6050.GyroRaw.z = gz - Mpu6050.GyroBias.z;
	
	ProcessGyroBias();
	if(BiasObj.isBiasValueFound)
	{
		ProcessAccScale(ax, ay, az);
	}
	Mpu6050.Gyro.x = Mpu6050.GyroRaw.x ;	
	Mpu6050.Gyro.y = Mpu6050.GyroRaw.y;
	Mpu6050.Gyro.z = Mpu6050.GyroRaw.z;
	
	applyAxis3fLpf(gyroLpf, &Mpu6050.Gyro);	

//	Mpu6050.Gyro.x = -(gx - Mpu6050.GyroBias.x) ;	
//	Mpu6050.Gyro.y =  (gy - Mpu6050.GyroBias.y);
//	Mpu6050.Gyro.z =  (gz - Mpu6050.GyroBias.z) ;

//	Mpu6050.Acc.x = ax;
//	Mpu6050.Acc.y = ay;
//	Mpu6050.Acc.z = az;
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
static void CollectAccGyroData(void)
{
	bsp_mpu6050ReadData(Mpu6050.Buf);
}


/**
  * @brief  CollectAndProcessSensorData
  * @param  None
  * @retval None
  */
void CollectAndProcessSensorData(void)
{
	CollectAccGyroData();
	ProcessAccGyroMeasurements();
}

/**
  * @brief  SensorInit
  * @param  None
  * @retval None
  */
void SensorInit(void)
{
	BiasObj.isBiasValueFound = FALSE;
	BiasObj.bufHead = BiasObj.buffer;
}


/*********************************************************************************/
