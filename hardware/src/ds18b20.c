#include "ds18b20.h"
#include "Delay.h"	
#include "led.h"
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
  


//��λDS18B20
void DS18B20_Rst(void)	   
{                 
	DS18B20_IO_OUT(); 	//SET PG11 OUTPUT
    DS18B20_DQ_OUT=0; 	//����DQ
    DelayUs(750);    	//����750us
    DS18B20_DQ_OUT=1; 	//DQ=1 
	DelayUs(15);     	//15US
}
//�ȴ�DS18B20�Ļ�Ӧ
//����1:δ��⵽DS18B20�Ĵ���
//����0:����
u8 DS18B20_Check(void) 	   
{   
	u8 retry=0;
	DS18B20_IO_IN();	//SET PG11 INPUT	 
    while (DS18B20_DQ_IN&&retry<200)
	{
		retry++;
		DelayUs(1);
	};	 
	if(retry>=200)return 1;
	else retry=0;
    while (!DS18B20_DQ_IN&&retry<240)
	{
		retry++;
		DelayUs(1);
	};
	if(retry>=240)return 1;	    
	return 0;
}
//��DS18B20��ȡһ��λ
//����ֵ��1/0
u8 DS18B20_Read_Bit(void) 	 
{
    u8 data;
	DS18B20_IO_OUT();	//SET PG11 OUTPUT
    DS18B20_DQ_OUT=0; 
	DelayUs(2);
    DS18B20_DQ_OUT=1; 
	DS18B20_IO_IN();	//SET PG11 INPUT
	DelayUs(12);
	if(DS18B20_DQ_IN)data=1;
    else data=0;	 
    DelayUs(50);           
    return data;
}
//��DS18B20��ȡһ���ֽ�
//����ֵ������������
u8 DS18B20_Read_Byte(void)     
{        
    u8 i,j,dat;
    dat=0;
	for (i=1;i<=8;i++) 
	{
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
    return dat;
}
//дһ���ֽڵ�DS18B20
//dat��Ҫд����ֽ�
void DS18B20_Write_Byte(u8 dat)     
 {             
    u8 j;
    u8 testb;
	DS18B20_IO_OUT();	//SET PG11 OUTPUT;
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if (testb) 
        {
            DS18B20_DQ_OUT=0;	// Write 1
            DelayUs(2);                            
            DS18B20_DQ_OUT=1;
            DelayUs(60);             
        }
        else 
        {
            DS18B20_DQ_OUT=0;	// Write 0
            DelayUs(60);             
            DS18B20_DQ_OUT=1;
            DelayUs(2);                          
        }
    }
}
//��ʼ�¶�ת��
void DS18B20_Start(void) 
{   						               
    DS18B20_Rst();	   
	DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);	// skip rom
    DS18B20_Write_Byte(0x44);	// convert
} 

//��ʼ��DS18B20��IO�� DQ ͬʱ���DS�Ĵ���
//����1:������
//����0:����    	 
u8 DS18B20_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PORTG��ʱ�� 
	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				//PORTG.11 �������
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOB,GPIO_Pin_9);    //���1

	DS18B20_Rst();

	return DS18B20_Check();
}  
//��ds18b20�õ��¶�ֵ
//���ȣ�0.1C
//����ֵ���¶�ֵ ��-550~1250�� 
short DS18B20_Get_Temp(void)
{
    u8 temp;
    u8 TL,TH;
	short tem;
    DS18B20_Start ();  			// ds1820 start convert
    DS18B20_Rst();
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);	// skip rom
    DS18B20_Write_Byte(0xbe);	// convert	    
    TL=DS18B20_Read_Byte(); 	// LSB   
    TH=DS18B20_Read_Byte(); 	// MSB  
	    	  
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;					//�¶�Ϊ��  
    }else temp=1;				//�¶�Ϊ��	  	  
    tem=TH; 					//��ø߰�λ
    tem<<=8;    
    tem+=TL;					//��õװ�λ
    tem=(float)tem*0.625;		//ת��     
	if(temp)return tem; 		//�����¶�ֵ
	else return -tem;    
}






///****************************************Copyright (c)****************************************************
//**                                        
//**                                 
//**
//**--------------File Info---------------------------------------------------------------------------------
//** File name:			
//** Last modified Date:          
//** Last Version:		   
//** Descriptions:							
//**--------------------------------------------------------------------------------------------------------
//** Created by:			FiYu
//** Created date:		2015-4-12
//** Version:			    1.0
//** Descriptions:		DHT11��ʪ�ȴ�����ʵ��				
//**--------------------------------------------------------------------------------------------------------
//** Modified by:			FiYu
//** Modified date:		
//** Version:				
//** Descriptions:		
//** Rechecked by:			
//**********************************************************************************************************/
//#include "ds18b20.h"
//#include "stm32f10x.h"

// /**************************************************************************************
// * ��  �� : ����DS18B20�õ���I/O��
// * ��  �� : ��
// * ����ֵ : ��
// **************************************************************************************/
//static void DS18B20_GPIO_Config(void)
//{		
//	GPIO_InitTypeDef GPIO_InitStructure;

//	RCC_APB2PeriphClockCmd(DS18B20_CLK, ENABLE); 
//															   
//  GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;	
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������  
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
//  GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
//	
//	GPIO_SetBits(DS18B20_PORT, DS18B20_PIN);	 //DS18B20�������ų�ʼ������Ϊ�ߵ�ƽ���
//}

// /**************************************************************************************
// * ��  �� : ����ʹDS18B20-DATA���ű�Ϊ����ģʽ
// * ��  �� : ��
// * ����ֵ : ��
// **************************************************************************************/
//static void DS18B20_Mode_IPU(void)
//{
// 	  GPIO_InitTypeDef GPIO_InitStructure;
//	
//	  GPIO_InitStructure.GPIO_Pin = DS18B20_PIN; 
//	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	  //��������
//	  GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
//}

// /**************************************************************************************
// * ��  �� : ����ʹDS18B20-DATA���ű�Ϊ���ģʽ
// * ��  �� : ��
// * ����ֵ : ��
// **************************************************************************************/
//static void DS18B20_Mode_Out_PP(void)
//{
// 	GPIO_InitTypeDef GPIO_InitStructure;
//															   
//  GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;	

//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //�������   
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
//}

// /**************************************************************************************
// * ��  �� : �������ӻ����͸�λ����
// * ��  �� : ��
// * ����ֵ : ��
// **************************************************************************************/
//static void DS18B20_Rst(void)
//{
//	DS18B20_Mode_Out_PP();     //��������Ϊ������� 
//	
//	DS18B20_DATA_OUT(LOW);     //�������ٲ���480us�ĵ͵�ƽ��λ�ź�
//	delay_us(750);
//	DS18B20_DATA_OUT(HIGH);   //�����ڲ�����λ�źź��轫��������
//	delay_us(15);   //�ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������
//}

// /**************************************************************************************
// * ��  �� : ���ӻ����������صĴ�������
// * ��  �� : ��
// * ����ֵ : 0���ɹ�   1��ʧ��  
// **************************************************************************************/
//static u8 DS18B20_Presence(void)
//{
//	u8 pulse_time = 0;
//	
//	DS18B20_Mode_IPU();    //��������Ϊ��������
//	
//	/* �ȴ���������ĵ�������������Ϊһ��60~240us�ĵ͵�ƽ�ź� 
//	 * �����������û����������ʱ�����ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������
//	 */
//	while( DS18B20_DATA_IN() && pulse_time<100 )
//	{
//		pulse_time++;
//		delay_us(1);
//	}	

//	if( pulse_time >=100 )  //����100us�󣬴������嶼��û�е���
//		return 1;             //��ȡʧ��
//	else                 //����100us�󣬴������嵽��
//		pulse_time = 0;    //�����ʱ����
//	
//	while( !DS18B20_DATA_IN() && pulse_time<240 )  // �������嵽�����Ҵ��ڵ�ʱ�䲻�ܳ���240us
//	{
//		pulse_time++;
//		delay_us(1);
//	}	
//	if( pulse_time >=240 ) // �������嵽�����Ҵ��ڵ�ʱ�䳬����240us
//		return 1;        //��ȡʧ��
//	else
//		return 0;
//}

// /**************************************************************************************
// * ��  �� : ��DS18B20��ȡһ��bit
// * ��  �� : ��
// * ����ֵ : u8 
// **************************************************************************************/
//static u8 DS18B20_Read_Bit(void)
//{
//	u8 dat;
//	
//	/* ��0�Ͷ�1��ʱ������Ҫ����60us */	
//	DS18B20_Mode_Out_PP();
//	/* ��ʱ�����ʼ���������������� >1us <15us �ĵ͵�ƽ�ź� */
//	DS18B20_DATA_OUT(LOW);
//	delay_us(10);
//	
//	/* ���ó����룬�ͷ����ߣ����ⲿ�������轫�������� */
//	DS18B20_Mode_IPU();
//	
//	if( DS18B20_DATA_IN() == SET )
//		dat = 1;
//	else
//		dat = 0;
//	
//	/* �����ʱ������ο�ʱ��ͼ */
//	delay_us(45);
//	
//	return dat;
//}

// /**************************************************************************************
// * ��  �� : ��DS18B20��һ���ֽڣ���λ����
// * ��  �� : ��
// * ����ֵ : u8  
// **************************************************************************************/
//u8 DS18B20_Read_Byte(void)
//{
//	u8 i, j, dat = 0;	
//	
//	for(i=0; i<8; i++) 
//	{
//		j = DS18B20_Read_Bit();		//��DS18B20��ȡһ��bit
//		dat = (dat) | (j<<i);
//	}
//	
//	return dat;																																																																																
//}

// /**************************************************************************************
// * ��  �� : дһ���ֽڵ�DS18B20����λ����
// * ��  �� : u8
// * ����ֵ : ��  
// **************************************************************************************/
//void DS18B20_Write_Byte(u8 dat)
//{
//	u8 i, testb;
//	DS18B20_Mode_Out_PP();
//	
//	for( i=0; i<8; i++ )
//	{
//		testb = dat&0x01;
//		dat = dat>>1;		
//		/* д0��д1��ʱ������Ҫ����60us */
//		if (testb)
//		{			
//			DS18B20_DATA_OUT(LOW);
//			delay_us(8);   //1us < �����ʱ < 15us
//			
//			DS18B20_DATA_OUT(HIGH);
//			delay_us(58);    //58us+8us>60us
//		}		
//		else
//		{			
//			DS18B20_DATA_OUT(LOW);  
//			/* 60us < Tx 0 < 120us */
//			delay_us(70);
//			
//			DS18B20_DATA_OUT(HIGH);			
//			/* 1us < Trec(�ָ�ʱ��) < �����*/
//			delay_us(2);
//		}
//	}
//}

// /**************************************************************************************
// * ��  �� : ��ʼDS18B20
// * ��  �� : ��
// * ����ֵ : ��  
// **************************************************************************************/
//void DS18B20_Start(void)
//{
//	DS18B20_Rst();	           //�������ӻ����͸�λ����
//	DS18B20_Presence();	       //���ӻ����������صĴ�������
//	DS18B20_Write_Byte(0XCC);		 // ���� ROM 
//	DS18B20_Write_Byte(0X44);		 // ��ʼת�� 
//}

// /**************************************************************************************
// * ��  �� : DS18B20��ʼ������
// * ��  �� : ��
// * ����ֵ : u8
// **************************************************************************************/
//u8 DS18B20_Init(void)
//{
//	DS18B20_GPIO_Config();   
//	DS18B20_Rst();
//	
//	return DS18B20_Presence();
//}

// /**************************************************************************************
// * ��  �� : ��DS18B20��ȡ�¶�ֵ
// * ��  �� : ��  
// * ����ֵ : float 
// **************************************************************************************/
//float DS18B20_Get_Temp(void)
//{
//	u8 tpmsb, tplsb;
//	short s_tem;
//	float f_tem;
//	
//	DS18B20_Rst();	   
//	DS18B20_Presence();	 
//	DS18B20_Write_Byte(0XCC);				/* ���� ROM */
//	DS18B20_Write_Byte(0X44);				/* ��ʼת�� */
//	
//	DS18B20_Rst();
//  DS18B20_Presence();
//	DS18B20_Write_Byte(0XCC);				/* ���� ROM */
//  DS18B20_Write_Byte(0XBE);				/* ���¶�ֵ */
//	
//	tplsb = DS18B20_Read_Byte();		 
//	tpmsb = DS18B20_Read_Byte(); 
//	
//	s_tem = tpmsb<<8;
//	s_tem = s_tem | tplsb;
//	
//	if( s_tem < 0 )		/* ���¶� */
//		f_tem = (~s_tem+1) * 0.0625;	
//	else
//		f_tem = (s_tem * 0.625);
//	  
//																	//��������Ŀ�Ľ�С������һλҲת��Ϊ����ʾ����
//																	//ͬʱ����һ���������������
//	  
//	return f_tem; 	
//}


///*************************************END OF FILE******************************/


