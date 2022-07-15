#include "sys.h"
//时间
//全局变量放main外面
extern Results results[];
extern nt_calendar_obj nwt;								//定义结构体变量
int hour,min,sec;										//时间变量



int main(void)
{
			 
	unsigned short timeCount = 0;	//发送间隔变量
	unsigned short pingCount = 0;	//发送间隔变量
	unsigned char *dataPtr = NULL;
	
	Hardware_Init();				//初始化外围硬件
	
	hour = nwt.hour;
	min = nwt.min;
	sec = nwt.sec;
		
	while(OneNet_DevLink())			//接入OneNET
		DelayXms(500);
	
	while(1)
	{		
		//采集数据
		//TDS与温度
		TEMP_Value_Conversion();
		TDS_Value_Conversion();	
		
		//onenet
    //发送心跳包，保证设备持续连接onenet
		if(++pingCount >= 6000)									 
		{
			if(OneNet_Ping()==0)
			UsartPrintf(USART_DEBUG, "ping success\r\n");
			pingCount = 0;
		}
		
		if(++timeCount >=800)									//100对应1s
		{
			UsartPrintf(USART_DEBUG, "OneNet_SendData\r\n");
			OneNet_SendData();									//发送数据
			
			timeCount = 0;
			ESP8266_Clear();
		}
		DelayXms(20);                         //发送命令更加流畅
		Read_level();                         //读取继电器电平状态
		check();                              //自动模式检测
		dataPtr = ESP8266_GetIPD(0);
		if(dataPtr != NULL)
		OneNet_RevPro(dataPtr);                //接收数据检测
//		DelayXms(10);
		               		
		//时间
		if(sec >= 60)
		{
			sec = 0;
			min ++;
		}

		if(min >= 60)
		{
			min = 0;
			hour ++;
		}

		if(hour >= 24)
		{
			hour = 0;
		}
	}
}
