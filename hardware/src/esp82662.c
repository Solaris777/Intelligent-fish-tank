#include "sys.h"


#define ESP82662_WIFI_INFO		"AT+CWJAP=\"HC04012\",\"0401204012\"\r\n"

#define ESP82662_TIME_INFO		"AT+CIPSTART=\"TCP\",\"www.beijing-time.org\",80\r\n"


unsigned short esp82662_cnt = 0, esp82662_cntPre = 0;
 nt_calendar_obj nwt;  //����ṹ�����

//==========================================================
//	�������ƣ�	ESP8266_Clear
//
//	�������ܣ�	��ջ���
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void ESP82662_Clear(void)
{

	memset(USART2_RX_BUF, 0, sizeof(USART2_RX_BUF));
	esp82662_cnt = 0;

}



u8* esp82662_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART2_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)USART2_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}


//==========================================================
//	�������ƣ�	ESP8266_SendCmd
//
//	�������ܣ�	��������
//
//	��ڲ�����	cmd������
//				res����Ҫ���ķ���ָ��
//
//	���ز�����	0-�ɹ�	1-ʧ��
//
//	˵����		
//==========================================================
_Bool ESP82662_SendCmd(u8 *cmd, u8 *ack)
{
	int waittime=100;
	u8 res=0; 
	USART2_RX_STA=0;
	u2_printf("%s\r\n",cmd);	//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(waittime--)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(esp82662_check_cmd(ack))
				{
//					printf("receive:%s\r\n",(u8*)ack);
					break;//�õ���Ч���� 
				}    //����ɾ��һ��else�����ȴ�ʱ���򷵻�1
					USART2_RX_STA=0;
			} 
		}
	}
	return res;
}




u8 esp82662_quit_trans(void)
{
	while((USART2->SR&0X40)==0);	//�ȴ����Ϳ�
	USART2->DR='+';      
	delay_ms(15);					//���ڴ�����֡ʱ��(10ms)
	while((USART2->SR&0X40)==0);	//�ȴ����Ϳ�
	USART2->DR='+';      
	delay_ms(15);					//���ڴ�����֡ʱ��(10ms)
	while((USART2->SR&0X40)==0);	//�ȴ����Ϳ�
	USART2->DR='+';      
	delay_ms(500);					//�ȴ�500ms
	return ESP82662_SendCmd("AT","OK");//�˳�͸���ж�.
}




//==========================================================
//	�������ƣ�	ESP8266_Init
//
//	�������ܣ�	��ʼ��ESP8266
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void ESP82662_Init(void)
{
	ESP82662_Clear();
	
	while(ESP82662_SendCmd("AT\r\n", "OK"));
	
	while(ESP82662_SendCmd("AT+CWMODE=1\r\n", "OK"));
	
	while(ESP82662_SendCmd("AT+CIPMUX=0\r\n", "OK"));
	
	while(ESP82662_SendCmd(ESP82662_WIFI_INFO, "OK"));
	
}







u8 get_beijing_time(void)
{
	u8 *p;
	u8 res;

	
	u8 *resp;
	u8 *p_end;
	p=mymalloc(SRAMIN,40);							//����40�ֽ��ڴ�
	resp=mymalloc(SRAMIN,10);
	p_end=mymalloc(SRAMIN,40);

	res = ESP82662_SendCmd(ESP82662_TIME_INFO, "OK");
	if(res==1)
	{
		myfree(SRAMIN,p);
		return 1;
	}
	delay_ms(300);
	
	while(ESP82662_SendCmd("AT+CIPMODE=1", "OK"));

	while(ESP82662_SendCmd("AT+CIPSEND", "OK"));

	
	u2_printf("GET /time15.asp HTTP/1.1Host:www.beijing-time.org\r\n");

	delay_ms(20);//��ʱ20ms���ص���ָ��ͳɹ���״̬
	USART2_RX_STA=0;	//���㴮��3����
	delay_ms(1000);
	if(USART2_RX_STA&0X8000)		//��ʱ�ٴνӵ�һ�����ݣ�Ϊʱ�������
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
	} 
	
			resp="Date";
			USART2_RX_BUF[USART2_RX_STA & 0x7ff] = 0;
			if(strstr((char*)USART2_RX_BUF,(char*)resp)) 
			{       
				resp="GMT";
				p_end = (u8*)strstr((char*)USART2_RX_BUF,(char*)resp);
				p = p_end - 9; 
				nwt.hour = ((*p - 0x30)*10 + (*(p+1) - 0x30) + 8) % 24;  //GMT0-   v->GMT8

				nwt.min = ((*(p+3) - 0x30)*10 + (*(p+4) - 0x30)) % 60;

				nwt.sec = ((*(p+6) - 0x30)*10 + (*(p+7) - 0x30)) % 60;

				USART2_RX_STA = 0;	

				USART2_RX_STA = 0;																
				myfree(SRAMIN,resp);
				myfree(SRAMIN,p_end);
				
      }               
	esp82662_quit_trans();//�˳�͸��
	ESP82662_SendCmd("AT+CIPCLOSE","OK");         //�ر�����
	myfree(SRAMIN,p);
	return 0;
}


