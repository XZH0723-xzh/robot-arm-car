#ifndef __BUZZER_H
#define __BUZZER_H

#include "stm32f10x.h"

// 音符频率定义（简谱1=C调，低音/中音/高音）
#define NOTE_L1  262   // 低音1
#define NOTE_L2  294
#define NOTE_L3  330
#define NOTE_L4  349
#define NOTE_L5  392
#define NOTE_L6  440
#define NOTE_L7  494

#define NOTE_1   523   // 中音1
#define NOTE_2   587
#define NOTE_3   659
#define NOTE_4   698
#define NOTE_5   784
#define NOTE_6   880
#define NOTE_7   988

#define NOTE_H1  1047  // 高音1
#define NOTE_H2  1175
#define NOTE_H3  1319
#define NOTE_H4  1397
#define NOTE_H5  1568
#define NOTE_H6  1760
#define NOTE_H7  1976

#define REST     0     // 休止符

// 洒水车专用节拍定义
#define QUA_SPRINKLER  380   // 四分音符
#define HALF_SPRINKLER 760   // 二分音符
#define EIGHT_SPRINKLER 190  // 八分音符

void Buzzer_Init(void);          // 蜂鸣器PWM初始化 (PA8)
void Buzzer_PlayTone(uint16_t freq); // 播放指定频率
void Buzzer_Stop(void);          // 停止发声
void Buzzer_Play_Sprinkler(void); // 播放洒水车音乐

#endif
