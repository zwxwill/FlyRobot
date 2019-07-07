/**
  ******************************************************************************
  * @file    srv_timer.c
  * @author  zwx
  * @version V1.1.0
  * @date    2019-01-29
  * @brief
  ******************************************************************************
  */

#include "srv_timer.h"
#include "bsp_ds3231.h"
#include "bsp_tim.h"
#include "bsp.h"

#include <time.h>
#include <stdio.h>


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DS3231_ADJUST_CNT      (10)
#define  timerINTERRUPT_FREQUENCY   1   /* timer frequency: 1s one interrupt */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static volatile int g_iAdjustCnt = 0;
static volatile int g_iAdjustEveryValue = 0;
static volatile int g_iLocalTime = 0;

/* Private functions ---------------------------------------------------------*/
static void Srv_TimeAdjust(unsigned int time);

/**
  * @brief
  * @param  None
  * @retval None
  */
void Srv_SNTPAdjustTime(unsigned int time)
{
    struct tm *t_tm;

    if(time == 0)
    {
        printf("����, ������δ��Ӧ��������״̬�Ƚϲ�\r\n");
    }
    else
    {
        /* �����Ǹ�������ʱ�䣬���Ա���ʱ��Ҫ��8��Сʱ */
        time += 8*60*60;
        Srv_TimeAdjust(time);

        /* ����ȡ��UNIXʱ���ת����������ʱ����ĸ�ʽ */
        t_tm = localtime((unsigned int *)&time);

        /* ��������ʱ���Ǵ�1900�꿪ʼ�ģ���Ҫ���� */
        t_tm->tm_year += 1900;

        /* ��ȡ���·ݷ�Χ��0-11������Ҫ��1 */
        t_tm->tm_mon += 1;

        printf("UNIXʱ�����%d ���ڣ�%02d/%02d/%02d  ʱ�䣺%02d:%02d:%02d\r\n",
               time,
               t_tm->tm_year,
               t_tm->tm_mon,
               t_tm->tm_mday,
               t_tm->tm_hour,
               t_tm->tm_min,
               t_tm->tm_sec);
    }
}

/**
  * @brief
  * @param  None
  * @retval None
  */
static void Srv_TimeAdjust(unsigned int time)
{
    unsigned int localtime = 0;
    struct tm timeTmp = {0};
    int adjustValue = 0;
    T_Calendar tCalendar = {0};

    bsp_Ds3231GetTime(&tCalendar);

    timeTmp.tm_sec  = tCalendar.sec;
    timeTmp.tm_min  = tCalendar.min;
    timeTmp.tm_hour = tCalendar.hour;
    timeTmp.tm_mday = tCalendar.date;
    timeTmp.tm_mon  = tCalendar.month -1;
    timeTmp.tm_year = tCalendar.year + 2000 - 1900;
    timeTmp.tm_wday = tCalendar.week;
    timeTmp.tm_isdst = 0;
    timeTmp.tm_yday = 0;

    /* 1 ת��ΪUTCʱ�� */
    localtime = mktime(&timeTmp);

    /* 2 �Ƚ� */
    adjustValue = time - localtime;

    /* 3 ���� */
    if(adjustValue != 0)
    {
        g_iLocalTime = localtime;
        g_iAdjustCnt = DS3231_ADJUST_CNT;
        g_iAdjustEveryValue = adjustValue / g_iAdjustCnt;

        /* 1 ������ʱ���ж� */
        bsp_TimInit(TIM5, timerINTERRUPT_FREQUENCY, TIM5_IRQ_PRI, 0);
    }
    else
    {
        bsp_TimClose(TIM5);
    }
}

/**
  * @brief
  * @param  None
  * @retval None
  */
void Srv_TimeAdjustInIrq(void)
{
    struct tm *ptimeTmp;
    T_Calendar tCalendar;

    if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);

        if(g_iAdjustCnt > 0)
        {
            g_iLocalTime += 1;
            g_iLocalTime += g_iAdjustEveryValue;

            ptimeTmp = localtime((unsigned int *)&g_iLocalTime);

            tCalendar.sec = ptimeTmp->tm_sec;
            tCalendar.min = ptimeTmp->tm_min;
            tCalendar.hour = ptimeTmp->tm_hour;
            tCalendar.date = ptimeTmp->tm_mday;
            tCalendar.month = ptimeTmp->tm_mon + 1;
            tCalendar.year = ptimeTmp->tm_year + 1900 - 2000;
            tCalendar.week = ptimeTmp->tm_wday;

            bsp_Ds3231SetTimeFromIrq(&tCalendar);

            g_iAdjustCnt--;
        }
    }
}

/**
  * @brief
  * @param  None
  * @retval None
  */
void Srv_PrintRtcTime(void)
{
    T_Calendar tim;

    bsp_Ds3231GetTime(&tim);
    printf("RTCʱ�䣺���ڣ�%04d/%02d/%02d  ʱ�䣺%02d:%02d:%02d\r\n",
           tim.year + 2000,
           tim.month,
           tim.date,
           tim.hour,
           tim.min,
           tim.sec);
}


