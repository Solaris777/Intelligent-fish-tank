#ifndef __DS18B20_H
#define __DS18B20_H 
#include "sys.h"   
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//DS18B20��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/12
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////									   
u8 DS18B20_Init(void);//��ʼ��DS18B20
short DS18B20_Get_Temp(void);//��ȡ�¶�
void DS18B20_Start(void);//��ʼ�¶�ת��
void DS18B20_Write_Byte(u8 dat);//д��һ���ֽ�
u8 DS18B20_Read_Byte(void);//����һ���ֽ�
u8 DS18B20_Read_Bit(void);//����һ��λ
u8 DS18B20_Check(void);//����Ƿ����DS18B20
void DS18B20_Rst(void);//��λDS18B20    
#endif






//#ifndef __DS18B20_H
//#define	__DS18B20_H

//#include "stm32f10x.h"
//#include "sys.h"

//#define HIGH  1
//#define LOW   0

//#define DS18B20_CLK     RCC_APB2Periph_GPIOB
//#define DS18B20_PIN     GPIO_Pin_9                  
//#define DS18B20_PORT		GPIOB 

////���κ꣬��������������һ��ʹ��,����ߵ�ƽ��͵�ƽ
//#define DS18B20_DATA_OUT(a)	if (a)	\
//					GPIO_SetBits(GPIOB,GPIO_Pin_9);\
//					else		\
//					GPIO_ResetBits(GPIOB,GPIO_Pin_9)
// //��ȡ���ŵĵ�ƽ
//#define  DS18B20_DATA_IN()	   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)

//typedef struct
//{
//	u8  humi_int;		//ʪ�ȵ���������
//	u8  humi_deci;	 	//ʪ�ȵ�С������
//	u8  temp_int;	 	//�¶ȵ���������
//	u8  temp_deci;	 	//�¶ȵ�С������
//	u8  check_sum;	 	//У���
//		                 
//}DS18B20_Data_TypeDef;

//u8 DS18B20_Init(void);
//float DS18B20_Get_Temp(void);
//#endif /* __DS18B20_H */









