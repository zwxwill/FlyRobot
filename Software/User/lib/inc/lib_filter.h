/**
  ******************************************************************************
  * @file    lib_filter.h
  * @author  zwx
  * @version V1.8.0
  * @date    2019-01-29
  * @brief   
  ******************************************************************************
  */  

#ifndef __LIB_FILTER_H__
#define __LIB_FILTER_H__

#include "stm32f10x.h"

#define IIR_SHIFT         8

int16_t iirLPFilterSingle(int32_t in, int32_t attenuation,  int32_t* filt);

typedef struct 
{
	float a1;
	float a2;
	float b0;
	float b1;
	float b2;
	float delay_element_1;
	float delay_element_2;
} lpf2pData;


#ifdef __cplusplus
extern "C" {
#endif

/* Exported define -----------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
/* Exported struct  ----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void lpf2pInit(lpf2pData* lpfData, float sample_freq, float cutoff_freq);
void lpf2pSetCutoffFreq(lpf2pData* lpfData, float sample_freq, float cutoff_freq);
float lpf2pApply(lpf2pData* lpfData, float sample);
float lpf2pReset(lpf2pData* lpfData, float sample);

#ifdef __cplusplus
}
#endif

#endif

/****************** 北京龙鼎源科技 http://www.bdrl.com.cn/ ********************/
