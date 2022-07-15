#include "sys.h"

extern int hour,min;
extern float TEMP,pH_value,TDS_value;
extern int time;
int auto1,temp1,reduce_water,feed1,add_water,auto_flag,feed_flag,flag,switch1,switch2;
int t1,t2;

/*
************************************************************
各种开关函数  B3/4//6/7/8/12/13  A11 
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

//补水
void ADD_ON(void)
{
	GPIO_WriteBit(ADD_PORT, ADD_Pin , Bit_SET);
}
void ADD_OFF(void)
{
	GPIO_WriteBit(ADD_PORT, ADD_Pin, Bit_RESET);
}



//抽水
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


//喂食
void FEED(void)
{
	if(feed_flag==1)              //确保一天只投喂一次
	{
		TIM_SetCompare1(TIM3,10);   //5----右下   15----中    25----左上
		DelayXms(500);
		TIM_SetCompare1(TIM3,15);
		feed_flag=0;
	}
}

////自动模式下的喂食
//void AUTO_FEED(void)
//{
//		TIM_SetCompare1(TIM3,10);   //5----右下   15----中    25----左上
//		DelayXms(500);
//		TIM_SetCompare1(TIM3,15);
//}


//自动模式
//包括定点喂食，水温低于20℃时加热，pH低于.....换水
void check(void)
{
	if(auto_flag==1)
	{
		//投喂
	  if(hour==time)
	  {
			flag++;
			if(flag==1)
			{
				feed_flag++;
				FEED();
			}
	  }
	  
		
		//控温
	  if(TEMP<20)
	  {
	  	TEMP_ON();
	  }
	  else
	  {
	  	TEMP_OFF();
	  }
		
		
		//换水
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



//读取电平函数
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
*	函数名称：	Hardware_Init
*
*	函数功能：	硬件初始化
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		初始化单片机功能以及外接设备
************************************************************
*/
void Hardware_Init(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断控制器分组设置

	Delay_Init();									//systick初始化
	
	My_TIM3_Init(199,7199);//周期为20ms,一定要放在串口初始化前面！！！！不然会冲突
	
	TIM2_Int_Init(9999,7199);							//10Khz的计数频率，计数到10000为1s 
	
	Usart1_Init(115200);							//串口1，打印信息用
	
	Usart3_Init(115200);							//串口3，驱动ESP8266用
	
	Switch_Init();										//开关初始化
	
	DS18B20_Init();                   //温度计初始化
	
	ADCx_Init();                      //TDS的ADC初始化
	
	pH_ADCx_Init();                   //pH的ADC初始化
	
	delay_init();	    	 							//延时函数初始化	 
	
	usart2_init(115200);	 							//串口初始化为115200

	ESP82662_Init(); 								//esp82662进行初始化，连接wifi          这里有两处地方用到了串口1打印数据，只能选择一处，否则会栈溢出

	while(get_beijing_time());									//获取时间

  ESP8266_Init();					//初始化ESP8266
	
	UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");

}
