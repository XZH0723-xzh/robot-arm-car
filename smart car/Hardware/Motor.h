#ifndef __MOTOR_H
#define __MOTOR_H

void Motor_Init(void);
void Motor_SetSpeed(int8_t Speed);
void Motor_SetForwardSpeed(int8_t ForwardSpeed);
void Motor_TurnRight(int8_t Angel);
void Motor_TurnLeft(int8_t Angle);
void Motor_Stop(void);
void Motor_Back(int8_t  BackSpeed);

#endif
