#include "stm32f10x.h"                  // Device header

// LED初始化 PC13 （STM32F103核心板默认：低电平亮）
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    GPIO_SetBits(GPIOC, GPIO_Pin_13);  // 上电 → 直接熄灭！
}

// 收到正确CAN数据 → 点亮LED
void LED_On(void)
{
    GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}
void LED_Off(void)
{
    GPIO_SetBits(GPIOC, GPIO_Pin_13);
}
