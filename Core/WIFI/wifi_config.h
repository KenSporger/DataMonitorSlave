#ifndef _WIFI_CONFIG
#define _WIFI_CONFIG

#define MES_LEN 200


//command
#define OK	(char*)"OK"
	
#define AP2	(char*)"2\r\n"		//������wifi
#define AP3	(char*)"3\r\n"		//������wifi�����ӷ���������
#define AP4	(char*)"4\r\n"		//������wifi��������������
	
#define OFF (char*)"off"
#define ON (char*)"on"
#define STATE (char*)"state"


static	int num;
static	char mes_buf[MES_LEN];		//���ڶ��г���


//Ѱ���Ӳ���ս��ջ���
char* find_str_from_mes_buf(char *str);

//������ݵ�����
void add_mes_buf(char ch);

//��ջ���
void clear_mes_buf(void);

#endif
