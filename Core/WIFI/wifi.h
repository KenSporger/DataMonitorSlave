#ifndef _WIFI_H_
#define _WIFI_H_

#include "main.h"
#include "wifi_config.h"
#include "cmsis_os2.h" 
#define ESP8266_LOG 	1
#define UDP_MODE 		0

#define FANHUI_V		"OK"		//普通指令返回值

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

extern const uint8_t wifi_name[20];		//路由器名字
extern const uint8_t wifi_pass[20];		//路由器密码

extern const uint8_t wifi_ip[32];			//服务器ip
extern const uint8_t wifi_port[6];		//服务器端口

static uint8_t wifi_work_mode=0;		//wifi模块的工作模式，1为配置模式，2为透传模式


#define _WIFI_TRAN_MODE 		2
#define _WIFI_CONFIG_MODE 		1
#define SERVER_CONN_TIMES       3		//连接服务器尝试最大次数
#define WIFI_CONN_TIMES      	1		//连接WIFI尝试最大次数
	
/*模块初始化
	
	1.测试AT命令，保证芯片工作正常
	2.关闭回显
	3.设置成客服端模式
	4.连接到路由器
	5.连接到服务器
	6.设置透传模式1
	7.开始透传
	
	8.关闭透传


*/
uint8_t esp8266_init(void);

void RST_ESP8266(void);

//测试AT指令，确保连接和芯片正常
uint8_t test_at(void);

//关闭回显
uint8_t close_huixian(void);

//设置成客户端
uint8_t set_client(void);

//判断是否连接路由器
uint8_t is_connect_ap(void);

//连接到路由器
uint8_t connect_ap(void);

//设置自动连接路由器
uint8_t auto_connect_ap(void);

//禁止自动连接路由器
uint8_t close_auto_connect_ap(void);

//连接到服务器
uint8_t connect_server(void);

//断开服务器
uint8_t disconnect_server(void);

//设置透传模式
uint8_t set_mode1(void);

//开始透传
uint8_t start_tran(void);

//关闭透传
uint8_t close_tran(void);

//设置为透传模式
void set_tran_mode(void);

//设置成配置模式
void set_config_mode(void);

//获取工作模式
uint8_t get_work_mode(void);


//发送命令和等待时间
uint8_t send_cmd_wait(uint8_t *str,uint8_t len,uint32_t d_ms);



#endif

