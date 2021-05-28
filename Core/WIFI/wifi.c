#include "wifi.h"
#include <stdio.h>
#include <stdarg.h>
#include "usart.h"

/*****esp8266��ʼ������*****/

//wifi�ʺź�����
 const uint8_t wifi_name[20]	="lxqbt";
 const uint8_t wifi_pass[20]	="p.e.com458624";
 
//�����˶��������ڵ������ip��
 const uint8_t wifi_ip[32]	="192.168.43.180";
 const uint8_t wifi_port[6]	="10456";

/*
AT+RST					��λ
AT						����Ӳ���Ƿ�����
ATE0					�رջ���
AT+CWMODE=1				����Ϊ�ͻ���
AT+CIPSTATUS			�ж�״̬������2��ʾ����������WIFI������3��ʾ���������ӷ�����
AT+CWAUTOCONN=1			��Ϊ�Զ�����WIFIģʽ
AT+CIPSTART="TCP","192.168.43.1",8085	���ӷ�����
AT+CIPMODE=1			��Ϊ͸��ģʽ
AT+CIPSEND				��ʼ͸��
+++
*/

extern uint8_t server_sta;
extern uint8_t wifi_sta;

__IO uint8_t TCP_RX_TEMP;			//1�ֽڽ��ջ���
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)		//�����жϻص�����
{
	if (UartHandle->Instance == USART6)
	{
			TCP_RX_TEMP = USART6->DR;				//��������
			add_mes_buf(TCP_RX_TEMP);	//�����ݴ�������ģʽ�µ�buf
	}
	__HAL_UNLOCK(UartHandle);
	HAL_UART_Receive_IT(&huart6, (uint8_t *)&TCP_RX_TEMP, 1);
}

uint8_t esp8266_init(void)
{
	uint8_t conn_server_n = 0;		//��¼����server�Ĵ���
	uint8_t conn_wifi_n = 0;		//��¼����WIFI����
	HAL_UART_Receive_IT(&huart6, (uint8_t *)&TCP_RX_TEMP, 1);
	set_config_mode();				//���ó�����ģʽ
	
	while(1)
	{
		//����ATָ��
		if(test_at())
		{
			#if ESP8266_LOG
			printf("AT���Գɹ�\r\n");
			#endif
			//�ɹ�
			break;	//����ѭ��
		}
		else
		{
			#if ESP8266_LOG
			printf("AT����ʧ��\r\n");
			#endif
			//ATʧ�������������1.��ǰ��͸��ģʽ 2.ģ����
			close_tran();	//�ر�͸��ģʽ
			printf("esp8266 offline!");
//			break;
		}
	}
	// ���Զ�����
	if (close_auto_connect_ap())
	{
		#if ESP8266_LOG
			printf("��ֹ�Զ�����·�����ɹ�\r\n");
		#endif	
	}
	else
	{
		#if ESP8266_LOG
		printf("��ֹ�Զ�����·����ʧ��\r\n");
		#endif	
	}
	//�ػ���
	if(close_huixian())
	{
		#if ESP8266_LOG
		printf("�ر�esp8266���� ����\r\n");
		#endif
	}
	else
	{
		#if ESP8266_LOG
		printf("�ر�esp8266���� �쳣\r\n");
		#endif
	}
	//���óɿͻ���
	if(set_client())
	{
		#if ESP8266_LOG
		printf("���óɿͻ��� ����\r\n");
		#endif
	}
	else
	{
		#if ESP8266_LOG
		printf("���óɿͻ��� �쳣\r\n");
		#endif
	}
	//�ж�wifi�����Ƿ�����	is_connect_ap
	while(is_connect_ap() == 0)
	{
		conn_wifi_n++;
		if(conn_wifi_n > WIFI_CONN_TIMES)
		{
			#if ESP8266_LOG
			printf("����WIFI��ʱ\r\n");
			#endif
			wifi_sta = WIFI_OFF_LINE;	//����wifi״̬
			return 0 ;
		}
		if(connect_ap())
		{
			//auto_connect_ap();			//�����Զ�����WIFI
			#if ESP8266_LOG
			printf("����wifi ����\r\n");
			#endif
			break;
		}
		else
		{
			#if ESP8266_LOG
			printf("����wifi �쳣\r\n");
			#endif
		}

	}
	wifi_sta = WIFI_ON_LINE;	//����wifi״̬
	#if ESP8266_LOG
	printf("����wifi ����\r\n");
	#endif
	#if UDP_MODE
	while(is_connect_ap()!=2)
	#else
	while(is_connect_ap()!=3)
	#endif
	{
		#if ESP8266_LOG
		printf("�������ӷ�����...\r\n");
		#endif
		conn_server_n++;
		if(conn_server_n > SERVER_CONN_TIMES)
		{
			#if ESP8266_LOG
			printf("���������ӳ�ʱ\r\n");
			#endif
			server_sta = SERVER_OFF_LINE;	//����server״̬
			return 0 ;
		}
		if(connect_server())	//������ӳɹ������Զ��˳�
		{
			break;
		}
	}
	#if ESP8266_LOG
	printf("���ӷ����� ����\r\n");
	#endif
	
	//����͸��ģʽ1
	if(set_mode1())
	{
		#if ESP8266_LOG
		printf("����͸��ģʽ1 ����\r\n");
		#endif
	}
	else
	{
		#if ESP8266_LOG
		printf("����͸��ģʽ1 �쳣\r\n");
		#endif
	}		
	//��ʼ͸��
	if(start_tran())
	{
		#if ESP8266_LOG
		printf("��ʼ͸�� ����\r\n");
		#endif
		//��ʼ��������
		set_tran_mode();
	}
	else
	{
		#if ESP8266_LOG
		printf("��ʼ͸�� �쳣\r\n");
		#endif
		return 0;
	}
	#if ESP8266_LOG
	printf("����������\r\n");
	#endif
	server_sta = SERVER_ON_LINE;	//����SERVER״̬
	return 1;
}

////��λģ��
//void RST_ESP8266(void)
//{
//	HAL_GPIO_TogglePin(EN_GPRS_GPIO_Port,EN_GPRS_Pin);	//esp8266�ϵ�
//	osDelay(500);
//	HAL_GPIO_TogglePin(EN_GPRS_GPIO_Port,EN_GPRS_Pin);	//esp8266�ϵ�
//	osDelay(2000);
//}

//����ATָ�ȷ�����Ӻ�оƬ����
uint8_t test_at()
{
	return 	send_cmd_wait((uint8_t*)"AT\r\n",4,100);
}

//�رջ���
uint8_t close_huixian()
{
	return send_cmd_wait((uint8_t*)"ATE0\r\n",6,50);
}

//���óɿͻ���
uint8_t set_client()
{
	
	return send_cmd_wait((uint8_t*)"AT+CWMODE=1\r\n",13,50);
}

//�ж�����״̬
uint8_t is_connect_ap()
{
	uart_send_wifi((uint8_t*)"AT+CIPSTATUS\r\n",14);
	osDelay(50);
	
	if(find_str_from_mes_buf(AP2))	//������wifi
	{
		clear_mes_buf();	//��ջ���
		return 2;
	}
	if(find_str_from_mes_buf(AP3))	//������wifi�����ӷ���������
	{
		clear_mes_buf();	//��ջ���
		return 3;
	}
	if(find_str_from_mes_buf(AP4))	//������wifi��������������
	{
		clear_mes_buf();	//��ջ���
		return 4;
	}
	clear_mes_buf();	//��ջ���
	return 0;
}

//�����Զ�����·����
uint8_t auto_connect_ap()
{
	return send_cmd_wait((uint8_t*)"AT+CWAUTOCONN=1\r\n",17,50);
}

// ��ֹ�Զ�����·����
uint8_t close_auto_connect_ap()
{
	return send_cmd_wait((uint8_t*)"AT+CWAUTOCONN=0\r\n",17,50);
}

//���ӵ�wifi
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

//���ӵ�������
uint8_t connect_server()
{
	osDelay(3000);
	tcp_data_printf("AT+CIPSTART=\"UDP\",\"192.168.43.71\",8090,500\r\n");
	
	osDelay(2000);	//�ȴ�2s
	
	if(find_str_from_mes_buf("CONNECT"))	//���ҽ��ն������Ƿ���ڡ�CONNECT��
	{
		clear_mes_buf();
		return 1;
	}

	return 0;
}
#else
//���ӵ�������
uint8_t connect_server()
{
	tcp_data_printf("AT+CIPSTART=\"TCP\",\"%s\",%s\r\n",wifi_ip,wifi_port);
	
	osDelay(2000);	//�ȴ�2s
	
	if(find_str_from_mes_buf("CONNECT"))	//���ҽ��ն������Ƿ���ڡ�CONNECT��
	{
		clear_mes_buf();
		return 1;
	}
	return 0;
}
#endif

//�Ͽ�������
uint8_t disconnect_server()
{
	//AT+CIPCLOSE
	return send_cmd_wait((uint8_t*)"AT+CIPCLOSE\r\n",13,100);
}

//����͸��ģʽ
uint8_t set_mode1()
{
	
	return send_cmd_wait((uint8_t*)"AT+CIPMODE=1\r\n",14,50);
}

//��ʼ͸��
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

//�ر�͸��
uint8_t close_tran()
{
	uart_send_wifi((uint8_t*)"+++",3);
	osDelay(500);
	uart_send_wifi((uint8_t*)"\r\n",2);	//ʵ�ʲ���ʱ������+++�Ժ󣬻���Ҫһ��ָ���\r\n������ģ��
	osDelay(100);
	if(test_at())
		return 1;		//�رճɹ�
	return 0;			//�ر�ʧ��
}

//����Ϊ͸��ģʽ
void set_tran_mode()
{
	wifi_work_mode=_WIFI_TRAN_MODE;
	
}

//���ó�����ģʽ
void set_config_mode()
{
	wifi_work_mode=_WIFI_CONFIG_MODE;
	clear_mes_buf();	//��ս��ջ���
}

//��ȡ����ģʽ
uint8_t get_work_mode()
{
	return wifi_work_mode;
	
}

//��������͵ȴ�ʱ��
uint8_t send_cmd_wait(uint8_t *str,uint8_t len,uint32_t d_ms)
{
	HAL_UART_Transmit(&huart6, str, len, d_ms);
//	uart_send_wifi(str,len);	//DMA��������
	osDelay(d_ms);
	if(find_str_from_mes_buf(OK))	//���Ҵ������ݻ����Ƿ�OK
	{	
		clear_mes_buf();
		return 1;
	}
	return 0;
}

