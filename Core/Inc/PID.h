//
// Created by ddh on 2024/2/5.
//

#ifndef PROJECT_PID_H
#define PROJECT_PID_H
typedef struct
{
    double SetPoint;
    double CurValue;
    double Output;
    double kp, ki, kd;
    double PreError, CurError;
    double Integral;
    double outMin, outMax;
} PID;

void PID_Init(PID *pid, double Kp, double Ki, double Kd, double SetPoint, double outMin, double outMax);
void updatePID(PID *pid, double CurValue);

#endif //PROJECT_PID_H
