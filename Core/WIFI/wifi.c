#include "wifi.h"
#include <stdio.h>
#include <stdarg.h>
#include "usart.h"

/*****esp8266初始化部分*****/

//wifi帐号和密码
 const uint8_t wifi_name[20]	="lxqbt";
 const uint8_t wifi_pass[20]	="p.e.com458624";
 
//无线运动传感器节点服务器ip：
 const uint8_t wifi_ip[32]	="192.168.43.180";
 const uint8_t wifi_port[6]	="10456";

/*
AT+RST					复位
AT						测试硬件是否正常
ATE0					关闭回显
AT+CWMODE=1				设置为客户端
AT+CIPSTATUS			判断状态：返回2表示已正常连接WIFI；返回3表示已正常连接服务器
AT+CWAUTOCONN=1			设为自动连接WIFI模式
AT+CIPSTART="TCP","192.168.43.1",8085	连接服务器
AT+CIPMODE=1			设为透传模式
AT+CIPSEND				开始透传
+++
*/

extern uint8_t server_sta;
extern uint8_t wifi_sta;

__IO uint8_t TCP_RX_TEMP;			//1字节接收缓冲
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)		//接收中断回调函数
{
	if (UartHandle->Instance == USART6)
	{
			TCP_RX_TEMP = USART6->DR;				//读出数据
			add_mes_buf(TCP_RX_TEMP);	//将数据存入配置模式下的buf
	}
	__HAL_UNLOCK(UartHandle);
	HAL_UART_Receive_IT(&huart6, (uint8_t *)&TCP_RX_TEMP, 1);
}

uint8_t esp8266_init(void)
{
	uint8_t conn_server_n = 0;		//记录连接server的次数
	uint8_t conn_wifi_n = 0;		//记录连接WIFI次数
	HAL_UART_Receive_IT(&huart6, (uint8_t *)&TCP_RX_TEMP, 1);
	set_config_mode();				//设置成配置模式
	
	while(1)
	{
		//测试AT指令
		if(test_at())
		{
			#if ESP8266_LOG
			printf("AT测试成功\r\n");
			#endif
			//成功
			break;	//跳出循环
		}
		else
		{
			#if ESP8266_LOG
			printf("AT测试失败\r\n");
			#endif
			//AT失败有两种情况：1.当前在透传模式 2.模块损坏
			close_tran();	//关闭透传模式
			printf("esp8266 offline!");
//			break;
		}
	}
	// 关自动连接
	if (close_auto_connect_ap())
	{
		#if ESP8266_LOG
			printf("禁止自动连接路由器成功\r\n");
		#endif	
	}
	else
	{
		#if ESP8266_LOG
		printf("禁止自动连接路由器失败\r\n");
		#endif	
	}
	//关回显
	if(close_huixian())
	{
		#if ESP8266_LOG
		printf("关闭esp8266回显 正常\r\n");
		#endif
	}
	else
	{
		#if ESP8266_LOG
		printf("关闭esp8266回显 异常\r\n");
		#endif
	}
	//设置成客户端
	if(set_client())
	{
		#if ESP8266_LOG
		printf("设置成客户端 正常\r\n");
		#endif
	}
	else
	{
		#if ESP8266_LOG
		printf("设置成客户端 异常\r\n");
		#endif
	}
	//判断wifi连接是否正常	is_connect_ap
	while(is_connect_ap() == 0)
	{
		conn_wifi_n++;
		if(conn_wifi_n > WIFI_CONN_TIMES)
		{
			#if ESP8266_LOG
			printf("连接WIFI超时\r\n");
			#endif
			wifi_sta = WIFI_OFF_LINE;	//更新wifi状态
			return 0 ;
		}
		if(connect_ap())
		{
			//auto_connect_ap();			//设置自动连接WIFI
			#if ESP8266_LOG
			printf("连接wifi 正常\r\n");
			#endif
			break;
		}
		else
		{
			#if ESP8266_LOG
			printf("连接wifi 异常\r\n");
			#endif
		}

	}
	wifi_sta = WIFI_ON_LINE;	//更新wifi状态
	#if ESP8266_LOG
	printf("连接wifi 正常\r\n");
	#endif
	#if UDP_MODE
	while(is_connect_ap()!=2)
	#else
	while(is_connect_ap()!=3)
	#endif
	{
		#if ESP8266_LOG
		printf("尝试连接服务器...\r\n");
		#endif
		conn_server_n++;
		if(conn_server_n > SERVER_CONN_TIMES)
		{
			#if ESP8266_LOG
			printf("服务器连接超时\r\n");
			#endif
			server_sta = SERVER_OFF_LINE;	//更新server状态
			return 0 ;
		}
		if(connect_server())	//如果连接成功，则自动退出
		{
			break;
		}
	}
	#if ESP8266_LOG
	printf("连接服务器 正常\r\n");
	#endif
	
	//设置透传模式1
	if(set_mode1())
	{
		#if ESP8266_LOG
		printf("设置透传模式1 正常\r\n");
		#endif
	}
	else
	{
		#if ESP8266_LOG
		printf("设置透传模式1 异常\r\n");
		#endif
	}		
	//开始透传
	if(start_tran())
	{
		#if ESP8266_LOG
		printf("开始透传 正常\r\n");
		#endif
		//开始接收数据
		set_tran_mode();
	}
	else
	{
		#if ESP8266_LOG
		printf("开始透传 异常\r\n");
		#endif
		return 0;
	}
	#if ESP8266_LOG
	printf("服务器在线\r\n");
	#endif
	server_sta = SERVER_ON_LINE;	//更新SERVER状态
	return 1;
}

////复位模块
//void RST_ESP8266(void)
//{
//	HAL_GPIO_TogglePin(EN_GPRS_GPIO_Port,EN_GPRS_Pin);	//esp8266断电
//	osDelay(500);
//	HAL_GPIO_TogglePin(EN_GPRS_GPIO_Port,EN_GPRS_Pin);	//esp8266上电
//	osDelay(2000);
//}

//测试AT指令，确保连接和芯片正常
uint8_t test_at()
{
	return 	send_cmd_wait((uint8_t*)"AT\r\n",4,100);
}

//关闭回显
uint8_t close_huixian()
{
	return send_cmd_wait((uint8_t*)"ATE0\r\n",6,50);
}

//设置成客户端
uint8_t set_client()
{
	
	return send_cmd_wait((uint8_t*)"AT+CWMODE=1\r\n",13,50);
}

//判断连接状态
uint8_t is_connect_ap()
{
	uart_send_wifi((uint8_t*)"AT+CIPSTATUS\r\n",14);
	osDelay(50);
	
	if(find_str_from_mes_buf(AP2))	//已连接wifi
	{
		clear_mes_buf();	//清空缓冲
		return 2;
	}
	if(find_str_from_mes_buf(AP3))	//已连接wifi，连接服务器正常
	{
		clear_mes_buf();	//清空缓冲
		return 3;
	}
	if(find_str_from_mes_buf(AP4))	//已连接wifi，但服务器离线
	{
		clear_mes_buf();	//清空缓冲
		return 4;
	}
	clear_mes_buf();	//清空缓冲
	return 0;
}

//设置自动连接路由器
uint8_t auto_connect_ap()
{
	return send_cmd_wait((uint8_t*)"AT+CWAUTOCONN=1\r\n",17,50);
}

// 禁止自动连接路由器
uint8_t close_auto_connect_ap()
{
	return send_cmd_wait((uint8_t*)"AT+CWAUTOCONN=0\r\n",17,50);
}

//连接到wifi
uint8_t connect_ap()
{
	tcp_data_printf("AT+CWJAP=\"%s\",\"%s\"\r\n",wifi_name,wifi_pass);
	
	osDelay(8000);
	
	if(find_str_from_mes_buf(OK))
	{
		clear_mes_buf();
		return 1;
	}
	
	return 0;
}
#if UDP_MODE

//连接到服务器
uint8_t connect_server()
{
	osDelay(3000);
	tcp_data_printf("AT+CIPSTART=\"UDP\",\"192.168.43.71\",8090,500\r\n");
	
	osDelay(2000);	//等待2s
	
	if(find_str_from_mes_buf("CONNECT"))	//查找接收队列中是否存在“CONNECT”
	{
		clear_mes_buf();
		return 1;
	}

	return 0;
}
#else
//连接到服务器
uint8_t connect_server()
{
	tcp_data_printf("AT+CIPSTART=\"TCP\",\"%s\",%s\r\n",wifi_ip,wifi_port);
	
	osDelay(2000);	//等待2s
	
	if(find_str_from_mes_buf("CONNECT"))	//查找接收队列中是否存在“CONNECT”
	{
		clear_mes_buf();
		return 1;
	}
	return 0;
}
#endif

//断开服务器
uint8_t disconnect_server()
{
	//AT+CIPCLOSE
	return send_cmd_wait((uint8_t*)"AT+CIPCLOSE\r\n",13,100);
}

//设置透传模式
uint8_t set_mode1()
{
	
	return send_cmd_wait((uint8_t*)"AT+CIPMODE=1\r\n",14,50);
}

//开始透传
uint8_t start_tran()
{
	
	uart_send_wifi((uint8_t*)"AT+CIPSEND\r\n",12);
	
	osDelay(50);
	
	if(find_str_from_mes_buf(">"))
	{
		clear_mes_buf();
		return 1;
	}

	return 0;
	
}

//关闭透传
uint8_t close_tran()
{
	uart_send_wifi((uint8_t*)"+++",3);
	osDelay(500);
	uart_send_wifi((uint8_t*)"\r\n",2);	//实际测试时，发完+++以后，还需要一个指令（带\r\n）激活模块
	osDelay(100);
	if(test_at())
		return 1;		//关闭成功
	return 0;			//关闭失败
}

//设置为透传模式
void set_tran_mode()
{
	wifi_work_mode=_WIFI_TRAN_MODE;
	
}

//设置成配置模式
void set_config_mode()
{
	wifi_work_mode=_WIFI_CONFIG_MODE;
	clear_mes_buf();	//清空接收缓冲
}

//获取工作模式
uint8_t get_work_mode()
{
	return wifi_work_mode;
	
}

//发送命令和等待时间
uint8_t send_cmd_wait(uint8_t *str,uint8_t len,uint32_t d_ms)
{
	HAL_UART_Transmit(&huart6, str, len, d_ms);
//	uart_send_wifi(str,len);	//DMA发送数据
	osDelay(d_ms);
	if(find_str_from_mes_buf(OK))	//查找串口数据缓存是否OK
	{	
		clear_mes_buf();
		return 1;
	}
	return 0;
}

