//
// Created by ddh on 2024/2/3.
//

#ifndef PROJECT_CONTROLLER_H
#define PROJECT_CONTROLLER_H
#include "main.h"
#include "usart.h"

#define M1 TIM_CHANNEL_3
#define M2 TIM_CHANNEL_2
#define M3 TIM_CHANNEL_4
#define M4 TIM_CHANNEL_1

typedef struct
{
    // flying data
    int throttle;
    int roll;
    int pitch;
    int yaw;
    // flyer status
    int is_flying;
} Controller;

void motor_control(int value);
void set_motor(uint32_t channel, double value);
void user_control(void);
void parse_command(const uint8_t *RxBuffer);
void debug_control();

#endif //PROJECT_CONTROLLER_H
