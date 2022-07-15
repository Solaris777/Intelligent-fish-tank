#ifndef _LED_H_
#define _LED_H_


void Switch_Init(void);
void ADD_OFF(void);
void ADD_ON(void);
void AUTO_ON(void);
void AUTO_OFF(void);
void REDUCE_ON(void);
void REDUCE_OFF(void);
void TEMP_ON(void);
void TEMP_OFF(void);
void FEED(void);
void SWITCH1_ON(void);
void SWITCH1_OFF(void);
void SWITCH2_ON(void);
void SWITCH2_OFF(void);
void check(void);
void Reset_Init(void);
int AUTO_FLAG(void);
void Hardware_Init(void);
void Read_level(void);
	
extern float TEMP;


//封装很重要，以后写代码都要先封装，便于修改，增加代码可读性

//继电器
#define ADD_PORT          GPIOB
#define ADD_Pin           GPIO_Pin_3
#define REDUCE_PORT       GPIOB
#define REDUCE_Pin        GPIO_Pin_12
#define HOT_PORT          GPIOB
#define HOT_Pin           GPIO_Pin_4
#define SWITCH1_PORT      GPIOB
#define SWITCH1_Pin       GPIO_Pin_7
#define SWITCH2_PORT      GPIOB
#define SWITCH2_Pin       GPIO_Pin_6
#define AUTO_PORT         GPIOB
#define AUTO_Pin          GPIO_Pin_13

//DS18B20
#define	DS18B20_DQ_OUT PBout(9) //数据端口	PA0 
#define	DS18B20_DQ_IN  PBin(9)  //数据端口	PA0 
//IO方向设置
#define DS18B20_IO_IN()  {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=8<<4;}
#define DS18B20_IO_OUT() {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=3<<4;}	


//ESP8266
#define ESP8266_RST_PCLK  RCC_APB2Periph_GPIOB
#define ESP8266_RST_PORT  GPIOB
#define ESP8266_RST_PIN   GPIO_Pin_14  


//舵机
#define MOTOR1_PORT      GPIOA
#define MOTOR1_Pin       GPIO_Pin_6
#define MOTOR2_PORT      GPIOA
#define MOTOR2_Pin       GPIO_Pin_7
#define MOTOR3_PORT      GPIOB
#define MOTOR3_Pin       GPIO_Pin_0

#endif
