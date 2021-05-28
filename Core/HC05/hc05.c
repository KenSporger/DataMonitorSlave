#include "usart.h" 					 
#include "hc05.h" 
#include "string.h"	 
#include "math.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
#include "stdarg.h"	 	 
#include "stdio.h"

uint8_t USART3_TX_BUF[128]; 				
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//ATK-HC05蓝牙模块驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/10/26
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
//********************************************************************************
//无修改信息							  
////////////////////////////////////////////////////////////////////////////////// 	   
//串口3,printf 函数
//确保一次发送数据不超过USART3_MAX_SEND_LEN字节
void u3_printf(char* fmt,...)  
{  
	u16 i;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART3_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART3_TX_BUF);//此次发送数据的长度
	HAL_UART_Transmit(&huart2, USART3_TX_BUF, i, 100);	
}

//初始化ATK-HC05模块
//返回值:0,成功;1,失败.
u8 HC05_Init(void)
{
	u8 retry=10;	  		 
	HAL_GPIO_WritePin(HC05_KEY_GPIO_Port, HC05_KEY_Pin, GPIO_PIN_RESET);
	while(retry--)
	{
		HAL_GPIO_WritePin(HC05_KEY_GPIO_Port, HC05_KEY_Pin, GPIO_PIN_SET);
		osDelay(10);
		u3_printf("AT\r\n");		//发送AT测试指令
		HAL_GPIO_WritePin(HC05_KEY_GPIO_Port, HC05_KEY_Pin, GPIO_PIN_RESET);
		osDelay(300);
		u3_printf("AT+PSWD=1120");
		osDelay(300);
		return 1;	 
	}
	return 0;
}	 			

