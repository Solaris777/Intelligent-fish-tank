#ifndef __PH_ADC_H
#define __PH_ADC_H

// ע�⣺����ADC�ɼ���IO����û�и��ã�����ɼ���ѹ����Ӱ��
/********************ADC1����ͨ�������ţ�����**************************/
#define pH_ADC_RCC         RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC2
#define pH_ADC_PORT        GPIOA
#define pH_ADC_GPIO_Pin    GPIO_Pin_1
#define pH_ADCX            ADC2            //��ʱ�Ӷ�Ӧ

#include "stm32f10x.h"
#include "delay.h"

void pH_ADCx_Init(void);
uint16_t Get_ADC(uint8_t ch);//��ȡADC

#endif
