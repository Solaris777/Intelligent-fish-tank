#ifndef _ESP8266_H_
#define _ESP8266_H_



#define ESP8266_WIFI_INFO		"AT+CWJAP=\"HC04012\",\"0401204012\"\r\n"

#define ESP8266_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"183.230.40.39\",6002\r\n"


#define REV_OK		0	//接收完成标志
#define REV_WAIT	1	//接收未完成标志


void ESP8266_Init(void);
_Bool ESP8266_SendCmd(char *cmd, char *res);
_Bool ESP8266_WaitRecive(void);
void ESP8266_Clear(void);
void ESP8266_SendData(unsigned char *data, unsigned short len);
unsigned char *ESP8266_GetIPD(unsigned short timeOut);


#endif
