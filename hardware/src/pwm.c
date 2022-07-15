#include "pwm.h"
void My_TIM3_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;//定时器的通道
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//分频因子
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_Period=arr;//自动重装载值
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;//预分频系数
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCNPolarity=TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
		
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	TIM_OC1Init(TIM3,&TIM_OCInitStruct);//通道1

	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);        //使能预装载寄存器
  TIM_Cmd(TIM3,ENABLE);        //使能定时器
	
}
