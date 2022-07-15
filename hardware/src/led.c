#include "sys.h"

extern int hour,min;
extern float TEMP,pH_value,TDS_value;
extern int time;
int auto1,temp1,reduce_water,feed1,add_water,auto_flag,feed_flag,flag,switch1,switch2;
int t1,t2;

/*
************************************************************
���ֿ��غ���  B3/4//6/7/8/12/13  A11 
************************************************************
*/
void Switch_Init(void)
{
	GPIO_InitTypeDef gpio_initstruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); 
	gpio_initstruct.GPIO_Mode = GPIO_Mode_Out_PP;									
	gpio_initstruct.GPIO_Pin = ADD_Pin|REDUCE_Pin|HOT_Pin|SWITCH1_Pin|SWITCH2_Pin;					
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;								
	GPIO_Init(ADD_PORT,  &gpio_initstruct);		

}

//��ˮ
void ADD_ON(void)
{
	GPIO_WriteBit(ADD_PORT, ADD_Pin , Bit_SET);
}
void ADD_OFF(void)
{
	GPIO_WriteBit(ADD_PORT, ADD_Pin, Bit_RESET);
}



//��ˮ
void REDUCE_ON(void)
{
	GPIO_WriteBit(REDUCE_PORT, REDUCE_Pin, Bit_SET);
}
void REDUCE_OFF(void)
{
	GPIO_WriteBit(REDUCE_PORT, REDUCE_Pin, Bit_RESET);
}




void TEMP_ON(void)
{
	GPIO_WriteBit(HOT_PORT, HOT_Pin, Bit_SET);
}
void TEMP_OFF(void)
{
	GPIO_WriteBit(HOT_PORT, HOT_Pin, Bit_RESET);
}



void SWITCH1_ON(void)
{
	GPIO_WriteBit(SWITCH1_PORT , SWITCH1_Pin, Bit_SET);
}


void SWITCH1_OFF(void)
{
	GPIO_WriteBit(SWITCH1_PORT , SWITCH1_Pin, Bit_RESET);
}



void SWITCH2_ON(void)
{
	GPIO_WriteBit(SWITCH2_PORT , SWITCH2_Pin, Bit_SET);
}


void SWITCH2_OFF(void)
{
	GPIO_WriteBit(SWITCH2_PORT , SWITCH2_Pin, Bit_RESET);
}


void AUTO_ON(void)
{
	auto_flag=1;
}

void AUTO_OFF(void)
{
	auto_flag=0;
}

int AUTO_FLAG(void)
{
	return auto_flag;
}


//ιʳ
void FEED(void)
{
	if(feed_flag==1)              //ȷ��һ��ֻͶιһ��
	{
		TIM_SetCompare1(TIM3,10);   //5----����   15----��    25----����
		DelayXms(500);
		TIM_SetCompare1(TIM3,15);
		feed_flag=0;
	}
}

////�Զ�ģʽ�µ�ιʳ
//void AUTO_FEED(void)
//{
//		TIM_SetCompare1(TIM3,10);   //5----����   15----��    25----����
//		DelayXms(500);
//		TIM_SetCompare1(TIM3,15);
//}


//�Զ�ģʽ
//��������ιʳ��ˮ�µ���20��ʱ���ȣ�pH����.....��ˮ
void check(void)
{
	if(auto_flag==1)
	{
		//Ͷι
	  if(hour==time)
	  {
			flag++;
			if(flag==1)
			{
				feed_flag++;
				FEED();
			}
	  }
	  
		
		//����
	  if(TEMP<20)
	  {
	  	TEMP_ON();
	  }
	  else
	  {
	  	TEMP_OFF();
	  }
		
		
		//��ˮ
		if(pH_value<6.5||pH_value>8.5||TDS_value>700)
		{
		  t1=min;
			t2=t1+3;
			if(t1!=t2)
			{
				REDUCE_ON();
			}
			else
			{
				REDUCE_OFF();
			}
		}
	}
	
	else
	{
		
	}
	
	
}



//��ȡ��ƽ����
void Read_level(void)
{
	reduce_water=GPIO_ReadOutputDataBit(REDUCE_PORT,REDUCE_Pin);
	temp1=GPIO_ReadOutputDataBit (HOT_PORT,HOT_Pin);
	add_water=GPIO_ReadOutputDataBit (ADD_PORT,ADD_Pin);
	auto1=AUTO_FLAG();
	switch1=GPIO_ReadOutputDataBit (SWITCH1_PORT,SWITCH2_Pin);
	switch2=GPIO_ReadOutputDataBit (SWITCH2_PORT,SWITCH2_Pin);
}




/*
************************************************************
*	�������ƣ�	Hardware_Init
*
*	�������ܣ�	Ӳ����ʼ��
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		��ʼ����Ƭ�������Լ�����豸
************************************************************
*/
void Hardware_Init(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�жϿ�������������

	Delay_Init();									//systick��ʼ��
	
	My_TIM3_Init(199,7199);//����Ϊ20ms,һ��Ҫ���ڴ��ڳ�ʼ��ǰ�棡��������Ȼ���ͻ
	
	TIM2_Int_Init(9999,7199);							//10Khz�ļ���Ƶ�ʣ�������10000Ϊ1s 
	
	Usart1_Init(115200);							//����1����ӡ��Ϣ��
	
	Usart3_Init(115200);							//����3������ESP8266��
	
	Switch_Init();										//���س�ʼ��
	
	DS18B20_Init();                   //�¶ȼƳ�ʼ��
	
	ADCx_Init();                      //TDS��ADC��ʼ��
	
	pH_ADCx_Init();                   //pH��ADC��ʼ��
	
	delay_init();	    	 							//��ʱ������ʼ��	 
	
	usart2_init(115200);	 							//���ڳ�ʼ��Ϊ115200

	ESP82662_Init(); 								//esp82662���г�ʼ��������wifi          �����������ط��õ��˴���1��ӡ���ݣ�ֻ��ѡ��һ���������ջ���

	while(get_beijing_time());									//��ȡʱ��

  ESP8266_Init();					//��ʼ��ESP8266
	
	UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");

}
