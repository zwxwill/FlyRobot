/**
  ******************************************************************************
  * @file    srv_imu.c
  * @author  zwx
  * @version V1.8.0
  * @date    2019-01-29
  * @brief   
  ******************************************************************************
  */  

#include "srv_imu.h"
#include "lib_math.h"
#include "bsp_mpu6050.h"

#include <math.h>

/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private struct  -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define Kp 					2.0f        //���ٶ�Ȩ�أ�Խ��������ٶȲ���ֵ����Խ��
#define Ki 					0.001f      //����������
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static float q0=1.0f, q1=0.0f, q2=0.0f, q3=0.0f;
static float exInt = 0, eyInt = 0, ezInt = 0;
static float halfT = (IMU_LOOP_TIME*1e-6)/2.0f;
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  
  * @param  None
  * @retval None
  */
void Imu_AHRSUpdate(void)
{
	static  
	float norm = 0; // ��һ��
	float ax=MPU6050.Fil.AccX,  ay=MPU6050.Fil.AccY,  az=MPU6050.Fil.AccZ;
	float gx=MPU6050.Fil.GyroX, gy=MPU6050.Fil.GyroY, gz=MPU6050.Fil.GyroZ;
	float vx=0, vy=0, vz=0;
	float ex=0, ey=0, ez=0;
	float tempq0=0, tempq1=0, tempq2=0, tempq3=0;	
	
	// �Ȱ���Щ�õõ���ֵ���
	float q0q0 = q0*q0;
	float q0q1 = q0*q1;
	float q0q2 = q0*q2;
//	float q0q3 = q0*q3;
	float q1q1 = q1*q1; 
//	float q1q2 = q1*q2;
	float q1q3 = q1*q3; 
	float q2q2 = q2*q2;   
	float q2q3 = q2*q3;
	float q3q3 = q3*q3; 
	
	// �Ѽ��ٶȼƵ���ά����ת��Ϊ��λ����
	norm = Math_InvSqrt(ax*ax + ay*ay + az*az);
	ax = ax * norm;
	ay = ay * norm;
	az = az * norm;
	
	// ���ǰ���Ԫ������ɡ��������Ҿ����еĵ����е�����Ԫ�ء�
	vx = 2*(q1q3 - q0q2);
	vy = 2*(q0q1 + q2q3);
	vz = q0q0 - q1q1 - q2q2 + q3q3;	
	
	//���ڰѼ��ٶȵĲ���ʸ���Ͳο�ʸ������������������������ݡ�
	ex = ( ay * vz - az * vy ) ;  
	ey = ( az * vx - ax * vz ) ;  
	ez = ( ax * vy - ay * vx ) ;

	if(ex != 0.0f && ey != 0.0f && ez != 0.0f)
	{
		exInt = exInt + ex * Ki * halfT;
		eyInt = eyInt + ey * Ki * halfT;	
		ezInt = ezInt + ez * Ki * halfT;

		// �ò���������PI����������ƫ
		gx = gx + Kp*ex + exInt;
		gy = gy + Kp*ey + eyInt;
		gz = gz + Kp*ez + ezInt;
	}
	
	// ��Ԫ��΢�ַ���
	tempq0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
	tempq1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
	tempq2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
	tempq3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;  

	// ��Ԫ���淶��
	norm = Math_InvSqrt(tempq0*tempq0 + tempq1*tempq1 + tempq2*tempq2 + tempq3*tempq3);
	q0 = tempq0 * norm;
	q1 = tempq1 * norm;
	q2 = tempq2 * norm;
	q3 = tempq3 * norm;
	
	MPU6050.Ang.Yaw = -Math_RegToDeg(atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2*q2 - 2 * q3 * q3 + 1)); 
	MPU6050.Ang.Pitch = -Math_RegToDeg(asin(-2 * q1 * q3 + 2 * q0 * q2));
	MPU6050.Ang.Roll = Math_RegToDeg(atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1)); 
}

/****************** ��������Դ�Ƽ� http://www.bdrl.com.cn/ ********************/

