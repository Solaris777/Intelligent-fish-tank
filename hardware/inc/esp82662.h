#ifndef _ESP82662_H_
#define _ESP82662_H_


//子对象1结构体--Location
typedef struct
{
	char id[32];
	char name[32];
	char country[32];
	char path[64];
	char timezone[32];
	char timezone_offset[32];
}Location;
 
//子对象2结构体--Now
typedef struct
{
	char text[32];
	char code[32];
	char temperature[32];
}Now;

typedef struct
{
	Location location;		//子对象1
	Now now;				//子对象2
	char last_update[64];	//子对象3
}Results;

typedef struct   //结构体。
{
    vu16  year;
    vu8   month;
    vu8   date;
    vu8   hour;
    vu8   min;
    vu8   sec;	 
}nt_calendar_obj;	 

//用户配置AT指令
extern char restart[];
extern char cwmode[];
extern char cwlap[];
extern char cwjap[];
extern char cifsr[];
extern char cipmux[];
extern char cipstart[];
extern char cipsend[];
extern char cipserver[];
extern char cwlif[];
extern char cipstatus[];
extern char cipsto[];
extern char cipmode[];
extern char test[];



#define REV_OK		0	//接收完成标志
#define REV_WAIT	1	//接收未完成标志

void ESP82662_Clear(void);
_Bool ESP82662_WaitRecive(void);
u8* esp82662_check_cmd(u8 *str);
u8 esp82662_quit_trans(void);
void ESP82662_Init(void);
u8 get_beijing_time(void);


#endif
