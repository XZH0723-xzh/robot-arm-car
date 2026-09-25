#include "stm32f10x.h"                  // Device header
#include "Delay.h"
/* ---------------- 新增：超声波与避障定义 ---------------- */
#define TRIG_PIN    GPIO_Pin_10
#define TRIG_PORT   GPIOB
#define ECHO_PIN    GPIO_Pin_11
#define ECHO_PORT   GPIOB

	void HC_SR04_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // Trig(PA0) 推挽输出
    GPIO_InitStruct.GPIO_Pin = TRIG_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TRIG_PORT, &GPIO_InitStruct);

    // Echo(PA1) 浮空输入
    GPIO_InitStruct.GPIO_Pin = ECHO_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(ECHO_PORT, &GPIO_InitStruct);
}

	float HCSR04_GetDistance(void)
{
    u32 time = 0;
    float distance;

    // 发送10us触发信号
    GPIO_ResetBits(TRIG_PORT, TRIG_PIN);
    Delay_us(2);
    GPIO_SetBits(TRIG_PORT, TRIG_PIN);
    Delay_us(10);
    GPIO_ResetBits(TRIG_PORT, TRIG_PIN);

    // 等待回波
    while(GPIO_ReadInputDataBit(ECHO_PORT, ECHO_PIN) == 0);
    while(GPIO_ReadInputDataBit(ECHO_PORT, ECHO_PIN) == 1)
    {
        time++;
        Delay_us(1);
        if(time > 50000) return -1;
    }

    distance = time * 0.017;
    return distance;
}
