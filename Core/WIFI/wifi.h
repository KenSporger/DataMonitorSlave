#ifndef _WIFI_H_
#define _WIFI_H_

#include "main.h"
#include "wifi_config.h"
#include "cmsis_os2.h" 
#define ESP8266_LOG 	1
#define UDP_MODE 		0

#define FANHUI_V		"OK"		//��ָͨ���ֵ

typedef enum
{
	SERVER_OFF_LINE = 1,
	SERVER_ON_LINE,
}server_sta_e;

typedef enum
{
	
	WIFI_OFF_LINE = 1,
	WIFI_ON_LINE,
}wifi_sta_e;

extern const uint8_t wifi_name[20];		//·��������
extern const uint8_t wifi_pass[20];		//·��������

extern const uint8_t wifi_ip[32];			//������ip
extern const uint8_t wifi_port[6];		//�������˿�

static uint8_t wifi_work_mode=0;		//wifiģ��Ĺ���ģʽ��1Ϊ����ģʽ��2Ϊ͸��ģʽ


#define _WIFI_TRAN_MODE 		2
#define _WIFI_CONFIG_MODE 		1
#define SERVER_CONN_TIMES       3		//���ӷ���������������
#define WIFI_CONN_TIMES      	1		//����WIFI����������
	
/*ģ���ʼ��
	
	1.����AT�����֤оƬ��������
	2.�رջ���
	3.���óɿͷ���ģʽ
	4.���ӵ�·����
	5.���ӵ�������
	6.����͸��ģʽ1
	7.��ʼ͸��
	
	8.�ر�͸��


*/
uint8_t esp8266_init(void);

void RST_ESP8266(void);

//����ATָ�ȷ�����Ӻ�оƬ����
uint8_t test_at(void);

//�رջ���
uint8_t close_huixian(void);

//���óɿͻ���
uint8_t set_client(void);

//�ж��Ƿ�����·����
uint8_t is_connect_ap(void);

//���ӵ�·����
uint8_t connect_ap(void);

//�����Զ�����·����
uint8_t auto_connect_ap(void);

//��ֹ�Զ�����·����
uint8_t close_auto_connect_ap(void);

//���ӵ�������
uint8_t connect_server(void);

//�Ͽ�������
uint8_t disconnect_server(void);

//����͸��ģʽ
uint8_t set_mode1(void);

//��ʼ͸��
uint8_t start_tran(void);

//�ر�͸��
uint8_t close_tran(void);

//����Ϊ͸��ģʽ
void set_tran_mode(void);

//���ó�����ģʽ
void set_config_mode(void);

//��ȡ����ģʽ
uint8_t get_work_mode(void);


//��������͵ȴ�ʱ��
uint8_t send_cmd_wait(uint8_t *str,uint8_t len,uint32_t d_ms);



#endif

