#ifndef __PH_ADC_H
#define __PH_ADC_H

// 注意：用作ADC采集的IO必须没有复用，否则采集电压会有影响
/********************ADC1输入通道（引脚）配置**************************/
#define pH_ADC_RCC         RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC2
#define pH_ADC_PORT        GPIOA
#define pH_ADC_GPIO_Pin    GPIO_Pin_1
#define pH_ADCX            ADC2            //与时钟对应

#include "stm32f10x.h"
#include "delay.h"

void pH_ADCx_Init(void);
uint16_t Get_ADC(uint8_t ch);//获取ADC

#endif
