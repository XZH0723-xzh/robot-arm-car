#include "stm32f10x.h"
#include "Delay.h"
#include "MyCAN.h"
#include "OLED.h"
#include "Bluetooth.h"
#include "Buzzer.h"
#include "Servo.h"
#include "string.h"
#include "stdlib.h"  


/* 电机 */
CanTxMsg TxMsg_MotorCtrl = {
	.StdId = 0x200,
	.ExtId = 0x00000000,
	.IDE = CAN_Id_Standard,
	.RTR = CAN_RTR_Data,
	.DLC = 4,      
	.Data = {0}
};
    uint8_t KeyNum;
	uint16_t Speed = 0;
	uint8_t Bluetooth_Data;
	uint16_t BackSpeed = 0;
	uint16_t  Angle=90;

	void Motor_SendSpeed(int8_t speed)
{
    TxMsg_MotorCtrl.Data[0] = 0x01;       // 前进指令
    TxMsg_MotorCtrl.Data[1] = speed;
    MyCAN_Transmit(&TxMsg_MotorCtrl);
}

// 发送右转指令
void Motor_SendTurnRight(void)
{
    TxMsg_MotorCtrl.Data[0] = 0x02;       // 右转指令
    TxMsg_MotorCtrl.Data[1] = 100;         // 转向速度
    MyCAN_Transmit(&TxMsg_MotorCtrl);
}

// 发送左转指令
void Motor_SendTurnLeft(void)
{
    TxMsg_MotorCtrl.Data[0] = 0x03;       // 左转指令
    TxMsg_MotorCtrl.Data[1] = 100;         // 转向速度
    MyCAN_Transmit(&TxMsg_MotorCtrl);
}

void Motor_Back(int8_t Backspeed)
{
    TxMsg_MotorCtrl.Data[0] = 0x04;       // 后退指令
    TxMsg_MotorCtrl.Data[1] = BackSpeed;         // 后退速度
    MyCAN_Transmit(&TxMsg_MotorCtrl);
}

void Avoid_Obstacle(void)
{
    TxMsg_MotorCtrl.Data[0] = 0x05;       // 后退指令
    TxMsg_MotorCtrl.Data[1] = 1;         // 后退速度
    MyCAN_Transmit(&TxMsg_MotorCtrl);
}
int main(void)
{
    /* 所有模块初始化 */
    OLED_Init();   
    Servo_Init();   
    Bluetooth_Init();  
	MyCAN_Init ();

    /* 舵机初始角度（完全按照你提供的代码设置） */
    float Angle1 = 80;   // 舵机1初始角度
    float Angle2 = 130;  // 舵机2初始角度
    float Angle3 = 180;   // 舵机3初始角度
    float Angle4 = 60;   // 舵机4初始角度

    /* 上电立即设置舵机到初始角度 */
    Servo_SetAngle1(Angle1);
    Servo_SetAngle2(Angle2);
    Servo_SetAngle3(Angle3);
    Servo_SetAngle4(Angle4);

    /* OLED显示静态标题 */
    OLED_ShowString(1, 1, "Car+Arm Control");

    while (1)
    {
        if (Bluetooth_RxFlag == 1)
        {

            char *Tag = strtok(Bluetooth_RxPacket, ",");

            if (Tag != NULL)
            {
                /* -------------------------- 处理按键数据包（控制电机） -------------------------- */
                if (strcmp(Tag, "key") == 0)
                {
                    char *KeyNum = strtok(NULL, ",");
                    char *Action = strtok(NULL, ",");

                    if (KeyNum != NULL && Action != NULL && strcmp(Action, "down") == 0)
                    {
                        if (strcmp(KeyNum, "1") == 0)  // 按键1：前进
                        {
                            Speed +=40; 
                            if(Speed>100) Speed=100;
                            Motor_SendSpeed(Speed);
                            OLED_ShowNum(2,8,Speed,3);
                            OLED_ShowString(4,1,"FORWARD");
                        }
                        else if (strcmp(KeyNum, "2") == 0)  // 按键2：后退
							
                        {
								BackSpeed +=40;
                                if(BackSpeed>100) BackSpeed=100;
                                Motor_Back(BackSpeed);
                                OLED_ShowNum(2,8,BackSpeed,3);
                                OLED_ShowString(4,1,"BACK ");
                        }
                        else if (strcmp(KeyNum, "3") == 0)  // 按键3：原地右转
                        {
								Motor_SendTurnRight();
                                OLED_ShowString(4,1,"RIGHT");
                        }
                        else if (strcmp(KeyNum, "4") == 0)  // 按键4：左转
                        {
								Motor_SendTurnLeft();
                                OLED_ShowString(4,1,"LEFT");                                
                        }
                        else if (strcmp(KeyNum, "8") == 0)  // 按键8：停止
                        {
								Speed = 0;
                                BackSpeed = 0;
                                Motor_SendSpeed(0);
                                OLED_ShowNum(2,8,0,3);
                                OLED_ShowString(4,1,"STOP");
                        }
						else if (strcmp(KeyNum, "6") == 0)  // 按键0：停止
                        {
								Buzzer_Play_Sprinkler();
                        }
						else if (strcmp(KeyNum, "7") == 0)  // 按键0：停止
                        {
								Buzzer_Stop();
                        }
                    }
                }
                /* -------------------------- 处理滑杆数据包（控制舵机） -------------------------- */
                else if (strcmp(Tag, "slider") == 0)
                {
                    char *SliderNum = strtok(NULL, ",");  // 滑杆编号：1-4
                    char *SliderValue = strtok(NULL, ","); // 滑杆值：0-180

                    if (SliderNum != NULL && SliderValue != NULL)
                    {
                        /* 将字符串形式的滑杆值转换为浮点数 */
                        float Value = atof(SliderValue);

                        /* 根据滑杆编号更新对应舵机的角度 */
                        if (strcmp(SliderNum, "1") == 0)
                        {
                            Angle1 += Value;														
                            Servo_SetAngle1(Angle1);
                            OLED_ShowString(3, 1, "Arm1: ");
                            OLED_ShowNum(3, 6, (uint32_t)Angle1, 3);
                        }
                        else if (strcmp(SliderNum, "2") == 0)
                        {
                            Angle2 -= Value;														
                            Servo_SetAngle2(Angle2);
                            OLED_ShowString(3, 1, "Arm2: ");
                            OLED_ShowNum(3, 6, (uint32_t)Angle2, 3);
                        }
                        else if (strcmp(SliderNum, "3") == 0)
                        {
                            Angle3 += Value;														
                            Servo_SetAngle3(Angle3);
                            OLED_ShowString(3, 1, "Arm3: ");
                            OLED_ShowNum(3, 6, (uint32_t)Angle3, 3);
                        }
                        else if (strcmp(SliderNum, "4") == 0)
                        {
                            Angle4 -= Value;														
                            Servo_SetAngle4(Angle4);
                            OLED_ShowString(3, 1, "Arm4: ");
                            OLED_ShowNum(3, 6, (uint32_t)Angle4, 3);
                        }
                    }
										Angle1 = 38;   // 舵机1初始角度
										Angle2 = 180;  // 舵机2初始角度
										Angle3 = 60;   // 舵机3初始角度
										Angle4 = 60;   // 舵机4初始角度
                }
            }

            /* 清除接收标志，准备接收下一个数据包 */
            Bluetooth_RxFlag = 0;
        }
    }
}
