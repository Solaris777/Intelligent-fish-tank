#ifndef _ESP82662_H_
#define _ESP82662_H_


//�Ӷ���1�ṹ��--Location
typedef struct
{
	char id[32];
	char name[32];
	char country[32];
	char path[64];
	char timezone[32];
	char timezone_offset[32];
}Location;
 
//�Ӷ���2�ṹ��--Now
typedef struct
{
	char text[32];
	char code[32];
	char temperature[32];
}Now;

typedef struct
{
	Location location;		//�Ӷ���1
	Now now;				//�Ӷ���2
	char last_update[64];	//�Ӷ���3
}Results;

typedef struct   //�ṹ�塣
{
    vu16  year;
    vu8   month;
    vu8   date;
    vu8   hour;
    vu8   min;
    vu8   sec;	 
}nt_calendar_obj;	 

//�û�����ATָ��
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



#define REV_OK		0	//������ɱ�־
#define REV_WAIT	1	//����δ��ɱ�־

void ESP82662_Clear(void);
_Bool ESP82662_WaitRecive(void);
u8* esp82662_check_cmd(u8 *str);
u8 esp82662_quit_trans(void);
void ESP82662_Init(void);
u8 get_beijing_time(void);


#endif
