#include "wifi_config.h"
#include "string.h"
#include "stdio.h"
//#include "stm32f10x.h"

/***�ⲿ����Ҫ��������esp8266���ֳ�ʼ����ATָ��***/

//�ڻ�����Ѱ�������ջ���
//������mes�����ջ��壬str������
//���ҵ���������ַ���λ�ã�����null
char* find_str_from_mes_buf(char *str)
{
	char* res = strstr(mes_buf,str);
	return res;
}

//������ݵ�����
void add_mes_buf(char ch)
{
	mes_buf[num++]=ch;

	if(num==MES_LEN)num=0;
}

//��ս��ջ���
void clear_mes_buf(void)
{
	num = 0;
	memset(mes_buf,0,MES_LEN);	//��ս��ջ���
}
