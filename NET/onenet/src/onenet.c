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
//	�������ƣ�	OneNet_DevLink
//
//	�������ܣ�	��onenet��������
//
//	��ڲ�����	��
//
//	���ز�����	1-�ɹ�	0-ʧ��
//
//	˵����		��onenetƽ̨��������
//==========================================================
_Bool OneNet_DevLink(void)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};					//Э���

	unsigned char *dataPtr;
	
	_Bool status = 1;
	
	UsartPrintf(USART_DEBUG, "OneNet_DevLink\r\n"
							"PROID: %s,	AUIF: %s,	DEVID:%s\r\n"
                        , PROID, AUTH_INFO, DEVID);
	
	if(MQTT_PacketConnect(PROID, AUTH_INFO, DEVID, 256, 0, MQTT_QOS_LEVEL0, NULL, NULL, 0, &mqttPacket) == 0)
	{
		ESP8266_SendData(mqttPacket._data, mqttPacket._len);			//�ϴ�ƽ̨
		
		dataPtr = ESP8266_GetIPD(250);									//�ȴ�ƽ̨��Ӧ
		if(dataPtr != NULL)
		{
			if(MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_CONNACK)
			{
				switch(MQTT_UnPacketConnectAck(dataPtr))
				{
					case 0:UsartPrintf(USART_DEBUG, "Tips:	���ӳɹ�\r\n");status = 0;break;
					
					case 1:UsartPrintf(USART_DEBUG, "WARN:	����ʧ�ܣ�Э�����\r\n");break;
					case 2:UsartPrintf(USART_DEBUG, "WARN:	����ʧ�ܣ��Ƿ���clientid\r\n");break;
					case 3:UsartPrintf(USART_DEBUG, "WARN:	����ʧ�ܣ�������ʧ��\r\n");break;
					case 4:UsartPrintf(USART_DEBUG, "WARN:	����ʧ�ܣ��û������������\r\n");break;
					case 5:UsartPrintf(USART_DEBUG, "WARN:	����ʧ�ܣ��Ƿ�����(����token�Ƿ�)\r\n");break;
					
					default:UsartPrintf(USART_DEBUG, "ERR:	����ʧ�ܣ�δ֪����\r\n");break;
				}
			}
		}
		
		MQTT_DeleteBuffer(&mqttPacket);								//ɾ��
	}
	else
		UsartPrintf(USART_DEBUG, "WARN:	MQTT_PacketConnect Failed\r\n");
	
	return status;
	
}


//==========================================================
//	�������ƣ�	OneNet_Ping
//
//	�������ܣ�	��onenet��������
//
//	��ڲ�����	��
//
//	���ز�����	1-ʧ��	0-�ɹ�
//
//	˵����		��onenetƽ̨��������
//==========================================================
_Bool OneNet_Ping(void)
{
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};					//Э���

	unsigned char *dataPtr;
	
	_Bool status = 1;
	
	MQTT_DeleteBuffer(&mqttPacket);								//ɾ��
	if(MQTT_PacketPing(&mqttPacket)== 0)
	{
		ESP8266_SendData(mqttPacket._data, mqttPacket._len);				//�ϴ�ƽ̨
//		dataPtr=0;
		dataPtr = ESP8266_GetIPD(500);									//�ȴ�ƽ̨��Ӧ250
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
		MQTT_DeleteBuffer(&mqttPacket);								//ɾ��		
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
	
	//�������ݲ���(��д)
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
		sprintf(text, "\"TDS\":%.1f,",TDS_value);        //TDS          ������ź���Ҫ��ֻ�����һ�����Բ���
    strcat(buf, text);
	
		memset(text, 0, sizeof(text));
		sprintf(text, "\"pH\":%.1f,",pH_value);     //pH
		UsartPrintf(USART_DEBUG, "pH:%.1f",pH_value);
		
    strcat(buf, text);
	
	/*�ⲿ��Ϊ���տ��������ݣ���֤����״̬��ͳһ��onenet��Ҫ�ر�������������*/
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
//	�������ƣ�	OneNet_SendData
//
//	�������ܣ�	�ϴ����ݵ�ƽ̨
//
//	��ڲ�����	type���������ݵĸ�ʽ
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void OneNet_SendData(void)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};												//Э���
	
	char buf[250];
	
	short body_len = 0, i = 0;
	
	UsartPrintf(USART_DEBUG, "Tips:	OneNet_SendData-MQTT\r\n");
	
	memset(buf, 0, sizeof(buf));
	
	body_len = OneNet_FillBuf(buf);																	//��ȡ��ǰ��Ҫ���͵����������ܳ���
	
	if(body_len)
	{
		if(MQTT_PacketSaveData(DEVID, body_len, NULL, 3, &mqttPacket) == 0)							//���
		{
			for(; i < body_len; i++)
				mqttPacket._data[mqttPacket._len++] = buf[i];
			
			ESP8266_SendData(mqttPacket._data, mqttPacket._len);									//�ϴ����ݵ�ƽ̨
			UsartPrintf(USART_DEBUG, "Send %d Bytes\r\n", mqttPacket._len);
			
			MQTT_DeleteBuffer(&mqttPacket);															//ɾ��
		}
		else
		{
			UsartPrintf(USART_DEBUG, "WARN:	EDP_NewBuffer Failed\r\n");
		}
	}
	
}

//==========================================================
//	�������ƣ�	OneNet_RevPro
//
//	�������ܣ�	ƽ̨�������ݼ��
//
//	��ڲ�����	dataPtr��ƽ̨���ص�����
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void OneNet_RevPro(unsigned char *cmd)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};								//Э���
	
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
		case MQTT_PKT_CMD:															//�����·�
			
			result = MQTT_UnPacketCmd(cmd, &cmdid_topic, &req_payload, &req_len);	//���topic����Ϣ��
			if(result == 0)
			{
				UsartPrintf(USART_DEBUG, "cmdid: %s, req: %s, req_len: %d\r\n", cmdid_topic, req_payload, req_len);
				
				if(MQTT_PacketCmdResp(cmdid_topic, req_payload, &mqttPacket) == 0)	//����ظ����
				{
					UsartPrintf(USART_DEBUG, "Tips:	Send CmdResp\r\n");
					
					ESP8266_SendData(mqttPacket._data, mqttPacket._len);			//�ظ�����
					MQTT_DeleteBuffer(&mqttPacket);									//ɾ��
				}
			}
		
		break;
			
		case MQTT_PKT_PUBACK:														//����Publish��Ϣ��ƽ̨�ظ���Ack
		
			if(MQTT_UnPacketPublishAck(cmd) == 0)
				UsartPrintf(USART_DEBUG, "Tips:	MQTT Publish Send OK\r\n");
			
		break;
		
		default:
			result = -1;
		break;
	}
	
	ESP8266_Clear();									//��ջ���
	
	if(result == -1)
		return;
	
//	dataPtr = strchr(req_payload, '}');					//����'}'
	
	
	
//	dataptr = strchr(req_payload, '-');	        //����'-'������wifi
//	
//	
//	
//	//����wifi����
//	if(dataptr != NULL && result != -1)
//	{
//		while(*req_payload!='\0')
//    strBuf[num++] = *req_payload++;            //�����յ����ַ�����������
//		s1 = strtok(strBuf,seps);
//		s2 = s1;                                   //�ó�����
//		s1 = strtok(NULL,seps);
//		s3 = s1;                                   //�ó�����
//		s1 = strtok(NULL,seps);
//		s4 = s1;                                   //���ಿ��
//		
//		//����ATָ��
//		UsartPrintf(USART_DEBUG, "AT+CWJAP\r\n");
//	  while(ESP8266_SendCmd("AT+CWJAP=\"RedmiK40\",\"123456hh\"\r\n", "OK"))
//		DelayXms(500);
//	}
	
	
	
	
	
	//��������֣����ݸ���WiFi��
	dataPtr = strchr(req_payload, '{');          //���ڼ��ݹ��ܣ�Ҫ��ontnet�����ÿ��ؿ�ֵΪ{1}��{0}!!!
	while(*dataPtr!='\0')
	strBuf[num++] = *dataPtr++;                  //�����յ����ַ�����������
	s1 = strtok(strBuf,seps);
	s2 = ++s1;                                   //�ó���һ��������
	s1 = strtok(NULL,seps);
	s3 = s1;                                   //�ó��ڶ�����������

	
	
	
	
	
	
	
	if(dataPtr != NULL && result != -1)					//����ҵ���
	{
//		
//		dataPtr++;                                
//		
//		if(*dataPtr >= '0' && *dataPtr <= '9')		//�ж��Ƿ����·��������������
//		{
//			numBuf[num++] = *dataPtr++;
//		}
//		
//		num = atoi((const char *)numBuf);				//תΪ��ֵ��ʽ
		
		
		//�����Ŀ���
		if(strstr((char *)req_payload, "add_water"))		//����ƽ̨���͵�����������"led"
		{
			num = atoi((const char *)s3);				//תΪ��ֵ��ʽ
			if(num == 1)								//����Ŀ����������Ϊ1������
			{
				ADD_ON();
			}
			else if(num == 0)							//�����������Ϊ0�������
			{
				ADD_OFF();
			}
		}
		
		//���������
		else if(strstr((char *)req_payload, "reduce_water"))    
		{
			num = atoi((const char *)s3);				//תΪ��ֵ��ʽ
			if(num == 1)			//�����������Ϊ0������
			{
				REDUCE_ON();    
			}
			else if(num == 0)	//�����������Ϊ1�������
			{
				REDUCE_OFF();
			}
		}
		 	
		//����������
			else if(strstr((char *)req_payload, "temp"))
		{
			num = atoi((const char *)s3);				//תΪ��ֵ��ʽ
			if(num == 1)						            //�ر�˵��������PB4����Ϊ�������ţ�һֱΪ�ߵ�ƽ���ʽ��̵�����Ϊ�͵�ƽ��������ȡ�෴��ƽ�������෴����		
			{
				TEMP_ON();                       
			}
			else if(num == 0)							
			{
				TEMP_OFF();
			}
		}
		
		//����һ����
			else if(strstr((char *)req_payload, "switch1"))
		{
			num = atoi((const char *)s3);				//תΪ��ֵ��ʽ
			if(num == 1)					
			{
				SWITCH1_ON();
			}
			else if(num == 0)				
			{
				SWITCH1_OFF();
			}
		}
		
		//���ض�����
			else if(strstr((char *)req_payload, "switch2"))
		{
			num = atoi((const char *)s3);				//תΪ��ֵ��ʽ
			if(num == 1)						
			{
				SWITCH2_ON();
			}
			else if(num == 0)						
			{
				SWITCH2_OFF();
			}
		}
		
		//ιʳ����
			else if(strstr((char *)req_payload, "feed"))
		{
			num = atoi((const char *)s3);				//תΪ��ֵ��ʽ
			if(num == 1)						
			{
				feed_flag=1;
				FEED();
			}
			else if(num == 0)					
			{
			}
		}

		//�Զ�����
			else if(strstr((char *)req_payload, "auto"))
		{
			num = atoi((const char *)s3);				//תΪ��ֵ��ʽ
			if(num == 1)						
			{
				AUTO_ON();
			}
			else if(num == 0)						
			{
				AUTO_OFF();
			}
		}
					
		//����wifi
			else if(strstr((char *)req_payload,"wifi"))
	  {
		  //����ATָ��
	    while(ESP8266_SendCmd(s2, "OK"))
		  DelayXms(500);
		}
		
		//����ιʳʱ��
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


