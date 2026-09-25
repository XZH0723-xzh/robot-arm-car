#include "stm32f10x.h"                  // Device header
#include "PWM.h"

/** 
  * 函    数：直流电机初始化
  * 参    数：无
  * 返 回 值：无
  */
void Motor_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//开启GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//开启GPIOA的时钟

	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);						//将PA4,PA5,PA6,PA7引脚初始化为推挽输出	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						//将PA4,PA5,PA6,PA7引脚初始化为推挽输出	

	
	PWM_Init();													//初始化直流电机的底层PWM
}

/**
  * 函    数：直流电机设置速度
  * 参    数：Speed 要设置的速度，范围：-100~100
  * 返 回 值：无
  */
void Motor_SetSpeed(int8_t Speed)
{
	if (Speed >= 0)							//如果设置正转的速度值
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_4);	//PA4置高电平
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);	//PA5置低电平，设置方向为正转
		GPIO_SetBits(GPIOA, GPIO_Pin_6);	//PA6置高电平
		GPIO_ResetBits(GPIOA, GPIO_Pin_7);	//PA7置低电平，设置方向为正转
		PWM_SetCompare3(Speed);				//PWM_A设置为速度值
		PWM_SetCompare2(Speed);				//PWM_b设置为速度值
	}
}

void Motor_SetForwardSpeed(int8_t ForwardSpeed)
{
	if (ForwardSpeed >= 0)							//如果设置正转的速度值
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_4);	//PA4置高电平
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);	//PA5置低电平，设置方向为正转
		GPIO_SetBits(GPIOB, GPIO_Pin_6);	//PA6置高电平
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);	//PA7置低电平，设置方向为正转
		PWM_SetForwardCompare3(ForwardSpeed);				//PWM_A设置为速度值
		PWM_SetForwardCompare4(ForwardSpeed);				//PWM_B设置为速度值
	}
}
			void Motor_TurnRight(int8_t Angle)
{
	if (Angle  >= 0)							//如果设置正转的速度值
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);	//PA4置低电平
		GPIO_SetBits(GPIOA, GPIO_Pin_5);		//PA5置高电平，设置方向为反转
		GPIO_SetBits(GPIOA, GPIO_Pin_6);		//PA6置高电平
		GPIO_ResetBits(GPIOA, GPIO_Pin_7);	//PA7置低电平，设置方向为正转
		PWM_SetCompare3(Angle);						//PWM_A设置为速度值
		PWM_SetCompare2(Angle);						//PWM_B设置为速度值
		
		GPIO_ResetBits(GPIOB, GPIO_Pin_4);	//PB4置低电平
		GPIO_SetBits(GPIOB, GPIO_Pin_5);			//PB5置高电平，设置方向为反转
		GPIO_SetBits(GPIOB, GPIO_Pin_6);			//PB6置高电平
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);	//PB7置低电平，设置方向为正转
		PWM_SetForwardCompare3(Angle);		//PWM_A设置为速度值
		PWM_SetForwardCompare4(Angle);		//PWM_B设置为速度�  ��
		
	}
}
		void Motor_TurnLeft(int8_t Angle)
{
	if (Angle  >= 0)							//如果设置正转的速度值
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_4);	//PA4置高电平
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);	//PA5置低电平，设置方向为正转
		GPIO_SetBits(GPIOA, GPIO_Pin_7);	//PA6置高电平
		GPIO_ResetBits(GPIOA, GPIO_Pin_6);	//PA7置低电平，设置方向为正转
		PWM_SetCompare3(Angle);				//PWM_A设置为速度值
		PWM_SetCompare2(Angle);				//PWM_b设置为速度值
		
		GPIO_SetBits(GPIOB, GPIO_Pin_4);	//PA4置高电平
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);	//PA5置低电平，设置方向为正转
		GPIO_SetBits(GPIOB, GPIO_Pin_7);	//PA6置高电平
		GPIO_ResetBits(GPIOB, GPIO_Pin_6);	//PA7置低电平，设置方向为正转
		PWM_SetForwardCompare3(Angle);				//PWM_A设置为速度值
		PWM_SetForwardCompare4(Angle);				//PWM_B设置为速度值
		
	}
}
		void Motor_Stop(void)
{
		PWM_SetCompare3(0);
		PWM_SetCompare2(0);
		PWM_SetForwardCompare3(0);
		PWM_SetForwardCompare4(0);
}

		void Motor_Back(int8_t  BackSpeed)
		{
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);	//PA4置低电平
		GPIO_SetBits(GPIOA, GPIO_Pin_5);	//PA5置高电平，设置方向为反转
		GPIO_ResetBits(GPIOA, GPIO_Pin_6);	//PA6置低电平
		GPIO_SetBits(GPIOA, GPIO_Pin_7);//PA7置高电平，设置方向为反转
		PWM_SetCompare3(BackSpeed);			//PWM设置为负的速度值，因为此时速度值为负数，而PWM只能给正数
		PWM_SetCompare2(BackSpeed);
			
		GPIO_ResetBits(GPIOB, GPIO_Pin_4);	//PA4置低电平
		GPIO_SetBits(GPIOB, GPIO_Pin_5);	//PA5置高电平，设置方向为反转
		GPIO_ResetBits(GPIOB, GPIO_Pin_6);	//PA6置低电平
		GPIO_SetBits(GPIOB, GPIO_Pin_7);//PA7置高电平，设置方向为反转
		PWM_SetForwardCompare3(BackSpeed);			//PWM设置为负的速度值，因为此时速度值为负数，而PWM只能给正数
		PWM_SetForwardCompare4(BackSpeed);
		}
