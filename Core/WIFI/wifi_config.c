#include "wifi_config.h"
#include "string.h"
#include "stdio.h"
//#include "stm32f10x.h"

/***这部分主要用来处理esp8266部分初始化的AT指令***/

//在缓冲中寻找命令并清空缓冲
//参数：mes：接收缓冲，str：命令
//查找到命令返回子字符串位置，否则null
char* find_str_from_mes_buf(char *str)
{
	char* res = strstr(mes_buf,str);
	return res;
}

//添加数据到队列
void add_mes_buf(char ch)
{
	mes_buf[num++]=ch;

	if(num==MES_LEN)num=0;
}

//清空接收缓冲
void clear_mes_buf(void)
{
	num = 0;
	memset(mes_buf,0,MES_LEN);	//清空接收缓冲
}
