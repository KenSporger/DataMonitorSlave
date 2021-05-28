#ifndef _WIFI_CONFIG
#define _WIFI_CONFIG

#define MES_LEN 200


//command
#define OK	(char*)"OK"
	
#define AP2	(char*)"2\r\n"		//已连接wifi
#define AP3	(char*)"3\r\n"		//已连接wifi，连接服务器正常
#define AP4	(char*)"4\r\n"		//已连接wifi，但服务器离线
	
#define OFF (char*)"off"
#define ON (char*)"on"
#define STATE (char*)"state"


static	int num;
static	char mes_buf[MES_LEN];		//串口队列长度


//寻找子并清空接收缓冲
char* find_str_from_mes_buf(char *str);

//添加数据到队列
void add_mes_buf(char ch);

//清空缓冲
void clear_mes_buf(void);

#endif
