//
// Created by ddh on 2024/2/3.
//
#include "controller.h"
#include "MPU6050.h"
#include "PID.h"
#include "stdio.h"
extern TIM_HandleTypeDef htim1;
extern MPU6050_t MPU6050;

Controller controller={0,0,0,0,0};

void motor_control(int value){
    // value : 0-100 integer
    if (value>100) value=100;
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, value);
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, value);
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_3, value);
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, value);
}

void set_motor(uint32_t channel, double value){
    // value : 0-100 integer
    if (value>100) value=100;
    if (value<0) value=0;
    __HAL_TIM_SetCompare(&htim1, channel, (int)value);
}

void debug_control(){
    while (1) {
        MPU6050_Read_All(&hi2c1, &MPU6050);
        // 输出gx gy gz
        printf("gx: %f, gy: %f, gz: %f  ", MPU6050.Gx, MPU6050.Gy, MPU6050.Gz);
        // 输出controller的各项值
        printf("throttle: %d, roll: %d, pitch: %d, yaw: %d, is_flying: %d\r\n", controller.throttle, controller.roll,
               controller.pitch, controller.yaw, controller.is_flying);
        HAL_Delay(50);
    }
}

void user_control(){
    MPU6050_adjust(&hi2c1, &MPU6050);
    PID roll_x, roll_y, yaw_z;
    PID_Init(&roll_x, 0, 0, 0, 0, -20, 20);
    PID_Init(&roll_y, 0, 0, 0, 0, -20, 20);
    PID_Init(&yaw_z, 10, 0, 0, 0, -20, 20);
    while (controller.is_flying==1){
        roll_x.SetPoint = controller.roll;
        roll_y.SetPoint = controller.pitch;
        yaw_z.SetPoint = controller.yaw;
        controller.throttle = controller.throttle>10?controller.throttle:10;
        MPU6050_Read_All(&hi2c1, &MPU6050);
        updatePID(&roll_x, MPU6050.KalmanAngleX);
        updatePID(&roll_y, MPU6050.KalmanAngleY);
        updatePID(&yaw_z, MPU6050.Gz);
        set_motor(M1, controller.throttle - roll_x.Output - roll_y.Output - yaw_z.Output);
        set_motor(M2, controller.throttle - roll_x.Output + roll_y.Output + yaw_z.Output);
        set_motor(M3, controller.throttle + roll_x.Output + roll_y.Output - yaw_z.Output);
        set_motor(M4, controller.throttle + roll_x.Output - roll_y.Output + yaw_z.Output);
    }
    // close all the motors
    motor_control(0);
}

void parse_command(const uint8_t *RxBuffer){
    // check
    if ((RxBuffer[0]+RxBuffer[1]+RxBuffer[2]+RxBuffer[3]+RxBuffer[4]+RxBuffer[5])%256 != RxBuffer[6]){
        return;
    }
    // parse
    controller.throttle = RxBuffer[1]; // 总油门
    controller.roll = RxBuffer[2]-128; // 横滚
    controller.pitch = RxBuffer[3]-128; // 俯仰
    controller.yaw = RxBuffer[4]-128; // 偏航角速度
    controller.is_flying = RxBuffer[5]; // 是否飞行
}