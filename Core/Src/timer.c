//
// Created by ddh on 2024/2/4.
//

#include "timer.h"

volatile uint32_t TimerCnt; //��ʱ���жϵĴ�������Ϊȫ�ֱ�����

extern TIM_HandleTypeDef htim2; //��ʱ�����

void timer_Init() //��ʱ����ʼ��
{
    HAL_TIM_Base_Start_IT(&htim2); //ʹ�ܶ�ʱ���ж�
    HAL_TIM_Base_Start(&htim2);  //������ʱ��
}

uint32_t Get_SystemTimer(void)//��ȡϵͳʱ��ĺ���
{
    // return system time in us
    // htim2.Init.Period: 65565-1 Counter Period
    return htim2.Instance->CNT + TimerCnt * htim2.Init.Period;
    //ϵͳʱ��=��ʱ����ǰ����ֵ+65535*��ʱ���жϴ���
}

double Get_SystemSecond(void){
    return Get_SystemTimer()/1000000.0;
}



