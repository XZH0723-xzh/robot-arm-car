#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "Servo.h"
#include "Key.h"
#include "MyCAN.h"
#include "LED.h"
#include "HC.h"

uint8_t KeyNum;
int8_t  Angle=0;
int8_t CurrentSpeed=0;
float front_dis;
uint8_t AutoAvoidMode = 0;  // 新增：自动避障模式标志位

CanRxMsg RxMsg;

int main(void)
{
    OLED_Init();
    Motor_Init();
    Key_Init();
    MyCAN_Init();
    LED_Init();
    HC_SR04_Init();

    LED_Off();
    OLED_ShowString(1,1,"System Ready");
	OLED_ShowNum(2,1,front_dis, 4);  // 实时显示z距离

    while(1)
    {
        // CAN接收处理
        if(CAN_GetFlagStatus(CAN1, CAN_FLAG_FMP0) == SET)
        {
            CAN_Receive(CAN1, CAN_FIFO0, &RxMsg);
            if(RxMsg.StdId == 0x200)
            {
                LED_On();
                Delay_ms(30);
                LED_Off();
                
                uint8_t cmd  = RxMsg.Data[0];
                uint8_t data = RxMsg.Data[1];
                
                if(cmd == 0x01) // 前进
                {
                    AutoAvoidMode = 0;  // 退出自动避障
                    Motor_SetSpeed(data);
                    Motor_SetForwardSpeed(data);
                    OLED_ShowString(4,1,"MANUAL FORW");
                }
                else if(cmd == 0x02) // 右转
                {
                    AutoAvoidMode = 0;
                    Motor_TurnRight(data);
                }
                else if(cmd == 0x03) // 左转
                {
                    AutoAvoidMode = 0;
                    Motor_TurnLeft(data);
                } 
                else if(cmd == 0x04) // 后退
                {
                    AutoAvoidMode = 0;
                    Motor_Back(data);
                }                
            }
        }
        Delay_ms(50);  // 周期延时，避免过于频繁检测
    }
}
