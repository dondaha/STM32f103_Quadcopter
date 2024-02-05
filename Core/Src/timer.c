//
// Created by ddh on 2024/2/4.
//

#include "timer.h"

volatile uint32_t TimerCnt; //定时器中断的次数（设为全局变量）

extern TIM_HandleTypeDef htim2; //定时器句柄

void timer_Init() //定时器初始化
{
    HAL_TIM_Base_Start_IT(&htim2); //使能定时器中断
    HAL_TIM_Base_Start(&htim2);  //启动定时器
}

uint32_t Get_SystemTimer(void)//获取系统时间的函数
{
    // return system time in us
    // htim2.Init.Period: 65565-1 Counter Period
    return htim2.Instance->CNT + TimerCnt * htim2.Init.Period;
    //系统时间=定时器当前计数值+65535*定时器中断次数
}

double Get_SystemSecond(void){
    return Get_SystemTimer()/1000000.0;
}



