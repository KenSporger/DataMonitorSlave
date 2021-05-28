#include "wifi_client.h"
#include "usart.h"
#include "string.h"

/***向服务器发送数据的相关函数，主要是协议打包的内容****/

uint8_t tx_data_temp[16];
//发送8位数据
void client_tx_u8_data(uint8_t data_id,uint8_t data)
{
	memset(tx_data_temp,0,6);
	tx_data_temp[0] = 0x5A;		//帧头
	tx_data_temp[1] = data_id;	//数据ID
	tx_data_temp[2] = data;		//数据
	uart_send_wifi(tx_data_temp,3);	//发送数据
}
//傻瓜式发送小数：将整数用一字节发送、小数放大100倍后用一字节发送(保留两位小数)
void client_tx_float_data(uint8_t data_id,float data)
{
	memset(tx_data_temp,0,6);
	tx_data_temp[0] = 0x5A;		//帧头
	tx_data_temp[1] = data_id;	//数据ID
	tx_data_temp[2] = (uint8_t)data;
	tx_data_temp[3] = (data-(float)tx_data_temp[2])*100;

	uart_send_wifi(tx_data_temp,4);	//发送数据
}

void client_tx_int16_data(uint8_t data_id,int16_t data)
{
	memset(tx_data_temp,0,6);
	tx_data_temp[0] = 0x5A;		//帧头
	tx_data_temp[1] = data_id;	//数据ID
	tx_data_temp[2] = (uint8_t )(data >> 8);
	tx_data_temp[3] = (uint8_t )(data & 0xff);
	uart_send_wifi(tx_data_temp,4);	//发送数据
}

void client_tx_buf_data(uint8_t data_id, uint8_t *data_p)
{
		memset(tx_data_temp,0,6);
		tx_data_temp[0] = 0x5A;		//帧头
		tx_data_temp[1] = data_id;	//数据ID
		memcpy(tx_data_temp+2,data_p,12);

		uart_send_wifi(tx_data_temp,14);	//发送数据
}
//发送int型数据，测试失败！
//void client_tx_int32_data(uint8_t data_id,int32_t data)
//{
//	uint8_t i = 0;
//	memset(tx_data_temp,0,6);
//	tx_data_temp[0] = 0x5A;		//帧头
//	tx_data_temp[1] = data_id;	//数据ID
//	//将float转为4字节存放
//	for(i = 0;i<4;i++)
//	{
//		tx_data_temp[2+i] = *((uint8_t *)((&data) + i));	//数据
//	}		
//	USARTx_Send(USART3,tx_data_temp,6);	//发送数据
//}

