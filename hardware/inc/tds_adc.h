#ifndef __ADC_H
#define	__ADC_H


#include "stm32f10x.h"

//TDS
#define    ADCx                          ADC1
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADC_CLK                       RCC_APB2Periph_ADC1

#define    ADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADC_GPIO_CLK                  RCC_APB2Periph_GPIOA  
#define    ADC_PORT                      GPIOA

// PA2-Õ®µ¿2 ∂¿¡¢IO
#define    ADC_PIN                       GPIO_Pin_0
#define    ADC_CHANNEL                   ADC_Channel_1
#define    ADC_DMA_CHANNEL               DMA1_Channel1



void ADCx_Init(void);


#endif /* __ADC_H */

