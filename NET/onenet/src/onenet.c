#include "sys.h"


#define PROID		"499630"

#define AUTH_INFO	"hush"

#define DEVID		"915528348"


extern unsigned char esp8266_buf[128];
extern int auto1,temp1,reduce_water,feed1,add_water,hour,switch1,switch2,feed_flag,min,sec;
extern float TDS_value; 
float TEMP,pH_value;
int time;

//==========================================================
//	函数名称：	OneNet_DevLink
//
//	函数功能：	与onenet创建连接
//
//	入口参数：	无
//
//	返回参数：	1-成功	0-失败
//
//	说明：		与onenet平台建立连接
//==========================================================
_Bool OneNet_DevLink(void)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};					//协议包

	unsigned char *dataPtr;
	
	_Bool status = 1;
	
	UsartPrintf(USART_DEBUG, "OneNet_DevLink\r\n"
							"PROID: %s,	AUIF: %s,	DEVID:%s\r\n"
                        , PROID, AUTH_INFO, DEVID);
	
	if(MQTT_PacketConnect(PROID, AUTH_INFO, DEVID, 256, 0, MQTT_QOS_LEVEL0, NULL, NULL, 0, &mqttPacket) == 0)
	{
		ESP8266_SendData(mqttPacket._data, mqttPacket._len);			//上传平台
		
		dataPtr = ESP8266_GetIPD(250);									//等待平台响应
		if(dataPtr != NULL)
		{
			if(MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_CONNACK)
			{
				switch(MQTT_UnPacketConnectAck(dataPtr))
				{
					case 0:UsartPrintf(USART_DEBUG, "Tips:	连接成功\r\n");status = 0;break;
					
					case 1:UsartPrintf(USART_DEBUG, "WARN:	连接失败：协议错误\r\n");break;
					case 2:UsartPrintf(USART_DEBUG, "WARN:	连接失败：非法的clientid\r\n");break;
					case 3:UsartPrintf(USART_DEBUG, "WARN:	连接失败：服务器失败\r\n");break;
					case 4:UsartPrintf(USART_DEBUG, "WARN:	连接失败：用户名或密码错误\r\n");break;
					case 5:UsartPrintf(USART_DEBUG, "WARN:	连接失败：非法链接(比如token非法)\r\n");break;
					
					default:UsartPrintf(USART_DEBUG, "ERR:	连接失败：未知错误\r\n");break;
				}
			}
		}
		
		MQTT_DeleteBuffer(&mqttPacket);								//删包
	}
	else
		UsartPrintf(USART_DEBUG, "WARN:	MQTT_PacketConnect Failed\r\n");
	
	return status;
	
}


//==========================================================
//	函数名称：	OneNet_Ping
//
//	函数功能：	与onenet创建连接
//
//	入口参数：	无
//
//	返回参数：	1-失败	0-成功
//
//	说明：		与onenet平台建立连接
//==========================================================
_Bool OneNet_Ping(void)
{
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};					//协议包

	unsigned char *dataPtr;
	
	_Bool status = 1;
	
	MQTT_DeleteBuffer(&mqttPacket);								//删包
	if(MQTT_PacketPing(&mqttPacket)== 0)
	{
		ESP8266_SendData(mqttPacket._data, mqttPacket._len);				//上传平台
//		dataPtr=0;
		dataPtr = ESP8266_GetIPD(500);									//等待平台响应250
		if(dataPtr != NULL)
		{
			if(MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_PINGRESP)
			{
				status=0;
			}
		}
		else
		{
			return status;
		}
		MQTT_DeleteBuffer(&mqttPacket);								//删包		
	}
	return status;
}



unsigned char OneNet_FillBuf(char *buf)
{
	
	char text[250];
	
	pH_value=((float)Get_ADC(ADC_Channel_1)*(3.3/4096))*(-5.9647)+22.255;
  TEMP=(DS18B20_Get_Temp())/10+(DS18B20_Get_Temp()%10)/10.0;
	
	memset(text, 0, sizeof(text));
	
		strcpy(buf, "{");
	
	//发送数据部分(大写)
    memset(text, 0, sizeof(text));
		sprintf(text, "\"TEMP\":%.1f,",TEMP);    
    strcat(buf, text);
	
	  memset(text, 0, sizeof(text));
		sprintf(text, "\"time\":%d,",hour);                   
    strcat(buf, text);
		
	
		  memset(text, 0, sizeof(text));
		sprintf(text, "\"min\":%d,",min);                   
    strcat(buf, text);
		
		
	  memset(text, 0, sizeof(text));
		sprintf(text, "\"second\":%d,",sec);                   
    strcat(buf, text);
	
	
		memset(text, 0, sizeof(text));
		sprintf(text, "\"TDS\":%.1f,",TDS_value);        //TDS          这个逗号很重要，只有最后一个可以不用
    strcat(buf, text);
	
		memset(text, 0, sizeof(text));
		sprintf(text, "\"pH\":%.1f,",pH_value);     //pH
		UsartPrintf(USART_DEBUG, "pH:%.1f",pH_value);
		
    strcat(buf, text);
	
	/*这部分为接收控制量数据，保证开关状态的统一，onenet上要关闭数据流过滤器*/
		memset(text, 0, sizeof(text));
		sprintf(text, "\"switch1\":%d,",switch1); 
    strcat(buf, text);
	
		memset(text, 0, sizeof(text));
		sprintf(text, "\"switch2\":%d,",switch2); 
    strcat(buf, text);
		
		memset(text, 0, sizeof(text));
		sprintf(text, "\"temp\":%d,",temp1); 
    strcat(buf, text);
		
		memset(text, 0, sizeof(text));
		sprintf(text, "\"reduce_water\":%d,",reduce_water); 
    strcat(buf, text);
		
		memset(text, 0, sizeof(text));
		sprintf(text, "\"add_water\":%d,",add_water); 
    strcat(buf, text);
		
//				memset(text, 0, sizeof(text));
//		sprintf(text, "\"ud\":%d,",ud1); 
//    strcat(buf, text);
//		
//				memset(text, 0, sizeof(text));
//		sprintf(text, "\"lr\":%d,",lr1); 
//    strcat(buf, text);
		
		memset(text, 0, sizeof(text));
		sprintf(text, "\"auto\":%d",auto1); 
    strcat(buf, text);
	
		strcat(buf, "}");
	
	return strlen(buf);

}

//==========================================================
//	函数名称：	OneNet_SendData
//
//	函数功能：	上传数据到平台
//
//	入口参数：	type：发送数据的格式
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OneNet_SendData(void)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};												//协议包
	
	char buf[250];
	
	short body_len = 0, i = 0;
	
	UsartPrintf(USART_DEBUG, "Tips:	OneNet_SendData-MQTT\r\n");
	
	memset(buf, 0, sizeof(buf));
	
	body_len = OneNet_FillBuf(buf);																	//获取当前需要发送的数据流的总长度
	
	if(body_len)
	{
		if(MQTT_PacketSaveData(DEVID, body_len, NULL, 3, &mqttPacket) == 0)							//封包
		{
			for(; i < body_len; i++)
				mqttPacket._data[mqttPacket._len++] = buf[i];
			
			ESP8266_SendData(mqttPacket._data, mqttPacket._len);									//上传数据到平台
			UsartPrintf(USART_DEBUG, "Send %d Bytes\r\n", mqttPacket._len);
			
			MQTT_DeleteBuffer(&mqttPacket);															//删包
		}
		else
		{
			UsartPrintf(USART_DEBUG, "WARN:	EDP_NewBuffer Failed\r\n");
		}
	}
	
}

//==========================================================
//	函数名称：	OneNet_RevPro
//
//	函数功能：	平台返回数据检测
//
//	入口参数：	dataPtr：平台返回的数据
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OneNet_RevPro(unsigned char *cmd)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};								//协议包
	
	char *req_payload = NULL;
	char *cmdid_topic = NULL;
	
	unsigned short req_len = 0;
	
	unsigned char type = 0;
	
	short result = 0;

	char* dataPtr = NULL;
	int num = 0;
//	char numBuf[10];
	char* s1 = NULL;
	char* s2 = NULL;
	char* s3 = NULL;
	char* s4 = "\"AT+CWJAP=\"";
	char* s5 = "\",\"";
	char* s6 = "\"\r\n\"";
	char strBuf[10];
	const char seps[]="}";
	
	type = MQTT_UnPacketRecv(cmd);
	switch(type)
	{
		case MQTT_PKT_CMD:															//命令下发
			
			result = MQTT_UnPacketCmd(cmd, &cmdid_topic, &req_payload, &req_len);	//解出topic和消息体
			if(result == 0)
			{
				UsartPrintf(USART_DEBUG, "cmdid: %s, req: %s, req_len: %d\r\n", cmdid_topic, req_payload, req_len);
				
				if(MQTT_PacketCmdResp(cmdid_topic, req_payload, &mqttPacket) == 0)	//命令回复组包
				{
					UsartPrintf(USART_DEBUG, "Tips:	Send CmdResp\r\n");
					
					ESP8266_SendData(mqttPacket._data, mqttPacket._len);			//回复命令
					MQTT_DeleteBuffer(&mqttPacket);									//删包
				}
			}
		
		break;
			
		case MQTT_PKT_PUBACK:														//发送Publish消息，平台回复的Ack
		
			if(MQTT_UnPacketPublishAck(cmd) == 0)
				UsartPrintf(USART_DEBUG, "Tips:	MQTT Publish Send OK\r\n");
			
		break;
		
		default:
			result = -1;
		break;
	}
	
	ESP8266_Clear();									//清空缓存
	
	if(result == -1)
		return;
	
//	dataPtr = strchr(req_payload, '}');					//搜索'}'
	
	
	
//	dataptr = strchr(req_payload, '-');	        //搜索'-'，更改wifi
//	
//	
//	
//	//更改wifi部分
//	if(dataptr != NULL && result != -1)
//	{
//		while(*req_payload!='\0')
//    strBuf[num++] = *req_payload++;            //将接收到的字符串存入数组
//		s1 = strtok(strBuf,seps);
//		s2 = s1;                                   //得出名称
//		s1 = strtok(NULL,seps);
//		s3 = s1;                                   //得出密码
//		s1 = strtok(NULL,seps);
//		s4 = s1;                                   //多余部分
//		
//		//输入AT指令
//		UsartPrintf(USART_DEBUG, "AT+CWJAP\r\n");
//	  while(ESP8266_SendCmd("AT+CWJAP=\"RedmiK40\",\"123456hh\"\r\n", "OK"))
//		DelayXms(500);
//	}
	
	
	
	
	
	//控制命令部分（兼容更改WiFi）
	dataPtr = strchr(req_payload, '{');          //由于兼容功能，要在ontnet上设置开关开值为{1}与{0}!!!
	while(*dataPtr!='\0')
	strBuf[num++] = *dataPtr++;                  //将接收到的字符串存入数组
	s1 = strtok(strBuf,seps);
	s2 = ++s1;                                   //得出第一括号内容
	s1 = strtok(NULL,seps);
	s3 = s1;                                   //得出第二个括号内容

	
	
	
	
	
	
	
	if(dataPtr != NULL && result != -1)					//如果找到了
	{
//		
//		dataPtr++;                                
//		
//		if(*dataPtr >= '0' && *dataPtr <= '9')		//判断是否是下发的命令控制数据
//		{
//			numBuf[num++] = *dataPtr++;
//		}
//		
//		num = atoi((const char *)numBuf);				//转为数值形式
		
		
		//开关四控制
		if(strstr((char *)req_payload, "add_water"))		//从云平台发送的数据中搜索"led"
		{
			num = atoi((const char *)s3);				//转为数值形式
			if(num == 1)								//后面的控制数据如果为1，代表开
			{
				ADD_ON();
			}
			else if(num == 0)							//控制数据如果为0，代表关
			{
				ADD_OFF();
			}
		}
		
		//开关五控制
		else if(strstr((char *)req_payload, "reduce_water"))    
		{
			num = atoi((const char *)s3);				//转为数值形式
			if(num == 1)			//控制数据如果为0，代表开
			{
				REDUCE_ON();    
			}
			else if(num == 0)	//控制数据如果为1，代表关
			{
				REDUCE_OFF();
			}
		}
		 	
		//开关三控制
			else if(strstr((char *)req_payload, "temp"))
		{
			num = atoi((const char *)s3);				//转为数值形式
			if(num == 1)						            //特别说明：由于PB4引脚为复用引脚，一直为高电平，故将继电器改为低电平触发，读取相反电平，接收相反数据		
			{
				TEMP_ON();                       
			}
			else if(num == 0)							
			{
				TEMP_OFF();
			}
		}
		
		//开关一控制
			else if(strstr((char *)req_payload, "switch1"))
		{
			num = atoi((const char *)s3);				//转为数值形式
			if(num == 1)					
			{
				SWITCH1_ON();
			}
			else if(num == 0)				
			{
				SWITCH1_OFF();
			}
		}
		
		//开关二控制
			else if(strstr((char *)req_payload, "switch2"))
		{
			num = atoi((const char *)s3);				//转为数值形式
			if(num == 1)						
			{
				SWITCH2_ON();
			}
			else if(num == 0)						
			{
				SWITCH2_OFF();
			}
		}
		
		//喂食控制
			else if(strstr((char *)req_payload, "feed"))
		{
			num = atoi((const char *)s3);				//转为数值形式
			if(num == 1)						
			{
				feed_flag=1;
				FEED();
			}
			else if(num == 0)					
			{
			}
		}

		//自动控制
			else if(strstr((char *)req_payload, "auto"))
		{
			num = atoi((const char *)s3);				//转为数值形式
			if(num == 1)						
			{
				AUTO_ON();
			}
			else if(num == 0)						
			{
				AUTO_OFF();
			}
		}
					
		//更改wifi
			else if(strstr((char *)req_payload,"wifi"))
	  {
		  //输入AT指令
	    while(ESP8266_SendCmd(s2, "OK"))
		  DelayXms(500);
		}
		
		//更改喂食时间
			else if(strstr((char *)req_payload,"time"))
	  {
			num = atoi((const char *)s3);
			time=num;
		}
		
	}

	if(type == MQTT_PKT_CMD || type == MQTT_PKT_PUBLISH)
	{
		MQTT_FreeBuffer(cmdid_topic);
		MQTT_FreeBuffer(req_payload);
	}

}


