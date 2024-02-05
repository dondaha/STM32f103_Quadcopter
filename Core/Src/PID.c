//
// Created by ddh on 2024/2/5.
//
#include "PID.h"

void PID_Init(PID *pid, double Kp, double Ki, double Kd, double SetPoint, double outMin, double outMax){
    pid->kp = Kp;
    pid->ki = Ki;
    pid->kd = Kd;
    pid->SetPoint = SetPoint;
    pid->outMin = outMin;
    pid->outMax = outMax;
    pid->PreError = 0;
    pid->Integral = 0;
}

void updatePID(PID *pid, double CurValue){
    pid->CurError = pid->SetPoint - CurValue;
    pid->Integral += pid->CurError;
    pid->Output = pid->kp * pid->CurError + pid->ki * pid->Integral + pid->kd * (pid->CurError - pid->PreError);
    pid->PreError = pid->CurError;
    if (pid->Output > pid->outMax) pid->Output = pid->outMax;
    if (pid->Output < pid->outMin) pid->Output = pid->outMin;
}