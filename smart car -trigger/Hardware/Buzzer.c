#include "Buzzer.h"
#include "Delay.h"

// 硬件定义：PA8 对应 TIM1_CH1
#define BUZZER_TIM        TIM1
#define BUZZER_RCC        RCC_APB2Periph_TIM1
#define BUZZER_GPIO       GPIOA
#define BUZZER_PIN        GPIO_Pin_8
#define BUZZER_GPIO_RCC   RCC_APB2Periph_GPIOA

/**
  * @brief  蜂鸣器PWM初始化 (PA8, TIM1_CH1)
  */
void Buzzer_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // 1. 使能时钟 (TIM1 和 GPIOA 都在 APB2)
    RCC_APB2PeriphClockCmd(BUZZER_RCC | BUZZER_GPIO_RCC, ENABLE);

    // 2. 配置 PA8 为复用推挽输出
    GPIO_InitStructure.GPIO_Pin = BUZZER_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BUZZER_GPIO, &GPIO_InitStructure);

    // 3. 配置 TIM1 时基 (72MHz系统时钟)
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1; // 计数频率 1MHz
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(BUZZER_TIM, &TIM_TimeBaseStructure);

    // 4. 配置 TIM1 通道 1 (CH1)
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(BUZZER_TIM, &TIM_OCInitStructure);

    // 【关键】高级定时器 TIM1 必须使能主输出
    TIM_CtrlPWMOutputs(BUZZER_TIM, ENABLE);

    // 5. 使能 TIM1
    TIM_Cmd(BUZZER_TIM, ENABLE);
}

/**
  * @brief  播放指定频率的声音
  * @param  freq: 频率(Hz)，传入0则停止发声
  */
void Buzzer_PlayTone(uint16_t freq)
{
    if(freq == 0)
    {
        TIM_SetCompare1(BUZZER_TIM, 0);
        return;
    }

    uint16_t arr = 1000000 / freq;
    TIM_SetAutoreload(BUZZER_TIM, arr);
    TIM_SetCompare1(BUZZER_TIM, arr / 2); // 50%占空比
}

/**
  * @brief  停止蜂鸣器发声
  */
void Buzzer_Stop(void)
{
    Buzzer_PlayTone(0);
}

/**
  * @brief  播放洒水车经典音乐《祝你生日快乐》
  */
void Buzzer_Play_Sprinkler(void)
{
    // 音符数组
    uint16_t notes[] = {
        NOTE_5, NOTE_5, NOTE_6, NOTE_5, NOTE_H1, NOTE_7,
        NOTE_5, NOTE_5, NOTE_6, NOTE_5, NOTE_H2, NOTE_H1,
        NOTE_5, NOTE_5, NOTE_H5, NOTE_H3, NOTE_H1, NOTE_7, NOTE_6,
        NOTE_H4, NOTE_H4, NOTE_H3, NOTE_H1, NOTE_H2, NOTE_H1,
        REST
    };

    // 节拍数组
    uint16_t beats[] = {
        EIGHT_SPRINKLER, EIGHT_SPRINKLER, QUA_SPRINKLER, QUA_SPRINKLER, QUA_SPRINKLER, HALF_SPRINKLER,
        EIGHT_SPRINKLER, EIGHT_SPRINKLER, QUA_SPRINKLER, QUA_SPRINKLER, QUA_SPRINKLER, HALF_SPRINKLER,
        EIGHT_SPRINKLER, EIGHT_SPRINKLER, QUA_SPRINKLER, QUA_SPRINKLER, QUA_SPRINKLER, QUA_SPRINKLER, HALF_SPRINKLER,
        EIGHT_SPRINKLER, EIGHT_SPRINKLER, QUA_SPRINKLER, QUA_SPRINKLER, QUA_SPRINKLER, HALF_SPRINKLER,
        QUA_SPRINKLER
    };

    // 播放循环
    for(int i = 0; i < sizeof(notes)/sizeof(notes[0]); i++)
    {
        Buzzer_PlayTone(notes[i]);
        Delay_ms(beats[i]);
    }
    
    Buzzer_Stop();
}
