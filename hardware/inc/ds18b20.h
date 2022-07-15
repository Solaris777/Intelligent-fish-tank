#ifndef __DS18B20_H
#define __DS18B20_H 
#include "sys.h"   
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//DS18B20驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/12
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////									   
u8 DS18B20_Init(void);//初始化DS18B20
short DS18B20_Get_Temp(void);//获取温度
void DS18B20_Start(void);//开始温度转换
void DS18B20_Write_Byte(u8 dat);//写入一个字节
u8 DS18B20_Read_Byte(void);//读出一个字节
u8 DS18B20_Read_Bit(void);//读出一个位
u8 DS18B20_Check(void);//检测是否存在DS18B20
void DS18B20_Rst(void);//复位DS18B20    
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

////带参宏，可以像内联函数一样使用,输出高电平或低电平
//#define DS18B20_DATA_OUT(a)	if (a)	\
//					GPIO_SetBits(GPIOB,GPIO_Pin_9);\
//					else		\
//					GPIO_ResetBits(GPIOB,GPIO_Pin_9)
// //读取引脚的电平
//#define  DS18B20_DATA_IN()	   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)

//typedef struct
//{
//	u8  humi_int;		//湿度的整数部分
//	u8  humi_deci;	 	//湿度的小数部分
//	u8  temp_int;	 	//温度的整数部分
//	u8  temp_deci;	 	//温度的小数部分
//	u8  check_sum;	 	//校验和
//		                 
//}DS18B20_Data_TypeDef;

//u8 DS18B20_Init(void);
//float DS18B20_Get_Temp(void);
//#endif /* __DS18B20_H */









