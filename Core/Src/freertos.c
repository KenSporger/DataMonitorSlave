/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "gpio.h"
#include "tim.h"
#include "rtc.h"
#include "w25qxx.h"
#include "usart.h"
#include "stdio.h"
#include "sd_hal_mpu6050.h"
#include "i2c.h"
#include "hc05.h"
#include "wifi.h"
#include "wifi_client.h"
#include "ssd1306.h"
#include "DS_18B20.h"
#include "MPU6050.h"
#include "usart.h"
#include "cJSON.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define SET_ADDR 10 * 4096
#define Times_SET_ADDR 9 * 4096
#define Mpu_SET_ADDR 11 * 4096
#define Mpu_his_SET_ADDR 8 * 4096
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
uint8_t HC_BUF[16]={0};//蓝牙数据
int16_t g_x;
int16_t g_y;
int16_t g_z;
int16_t a_x;
int16_t a_y;
int16_t a_z;

uint8_t boot_flag = 0;
uint8_t alarm_flag = 0;
uint8_t sta = 0x01;
float temperature_rt = 25;
float temperature_old = 25;
int temperature_threshold = 35;
int8_t sensitiveness_shock = 5;
float upload_interval = 1.0;
uint8_t alarm_time = 3;
uint8_t func_index = 1;
uint8_t vision_index = 1;
uint8_t display_index = 1;
uint8_t param_index = 1;
uint8_t wifi_index = 1;
uint8_t wifi_upload = 0;
uint16_t interval =1000;
uint8_t key;
uint32_t recv_num = 0;
uint32_t send_num = 0;
uint8_t beep = 1;
uint16_t beep_duty=10;
uint8_t start_flag = 0;		//1 开始计时 0结束计时，
extern int g_beep_time;

float FAX_table[80];
int FAX_idx=0;

float FAY_table[80];
int FAY_idx=0;

float FAZ_table[80];
int FAZ_idx=0;

float Temp_table[80];
int Temp_idx=0;

int shockValue = 0;


uint8_t server_sta;			//服务器状态
uint8_t wifi_sta;			//wifi状态
//flag
uint8_t esp8266_ready_flag = 0;
uint8_t data_readfinish_flag = 0;
/* USER CODE END Variables */
/* Definitions for TaskLed */
osThreadId_t TaskLedHandle;
const osThreadAttr_t TaskLed_attributes = {
  .name = "TaskLed",
  .priority = (osPriority_t) osPriorityHigh,
  .stack_size = 256 * 4
};
/* Definitions for TaskBoot */
osThreadId_t TaskBootHandle;
const osThreadAttr_t TaskBoot_attributes = {
  .name = "TaskBoot",
  .priority = (osPriority_t) osPriorityHigh,
  .stack_size = 128 * 4
};
/* Definitions for TaskGUI */
osThreadId_t TaskGUIHandle;
const osThreadAttr_t TaskGUI_attributes = {
  .name = "TaskGUI",
  .priority = (osPriority_t) osPriorityHigh,
  .stack_size = 256 * 4
};
/* Definitions for TaskKey */
osThreadId_t TaskKeyHandle;
const osThreadAttr_t TaskKey_attributes = {
  .name = "TaskKey",
  .priority = (osPriority_t) osPriorityHigh,
  .stack_size = 256 * 4
};
/* Definitions for TaskBeep */
osThreadId_t TaskBeepHandle;
const osThreadAttr_t TaskBeep_attributes = {
  .name = "TaskBeep",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 256 * 4
};
/* Definitions for TaskSEG */
osThreadId_t TaskSEGHandle;
const osThreadAttr_t TaskSEG_attributes = {
  .name = "TaskSEG",
  .priority = (osPriority_t) osPriorityHigh,
  .stack_size = 128 * 4
};
/* Definitions for TaskSupervision */
osThreadId_t TaskSupervisionHandle;
const osThreadAttr_t TaskSupervision_attributes = {
  .name = "TaskSupervision",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for TaskHC05 */
osThreadId_t TaskHC05Handle;
const osThreadAttr_t TaskHC05_attributes = {
  .name = "TaskHC05",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 256 * 4
};
/* Definitions for myTaskWiFi */
osThreadId_t myTaskWiFiHandle;
const osThreadAttr_t myTaskWiFi_attributes = {
  .name = "myTaskWiFi",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 256 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void Beep(uint16_t time,uint8_t tune);
void DrawUIInit(void);
void DrawUIMenu(void);
void readData(float datas[], int idx, float data);
void DrawDataLine(float max, float min, float lmt, float data, float datas[], int cdata);
void DrawRTSupervision(uint8_t vision_index);
void DrawRTDataLine(uint8_t vision_index);
void DrawWiFi(void);
void DrawParamSetting(uint8_t param_index);
void DrawFuncIndex(uint8_t func_index);
void DrawDataLine(float max, float min, float lmt, float data, float datas[], int cdata);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTaskLed(void *argument);
void StartTaskBoot(void *argument);
void StartTaskGUI(void *argument);
void StartTaskKey(void *argument);
void StartTaskBeep(void *argument);
void StartTask02(void *argument);
void StartTask03(void *argument);
void StartTaskSEG(void *argument);
void StartTaskSupervision(void *argument);
void StartTaskHC(void *argument);
void StartTaskWiFi(void *argument);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

void MX_FREERTOS_Init(void) 
{
  TaskLedHandle = osThreadNew(StartTaskLed, NULL, &TaskLed_attributes);
	myTaskWiFiHandle = osThreadNew(StartTaskWiFi, NULL, &myTaskWiFi_attributes);
	TaskBootHandle = osThreadNew(StartTaskBoot, NULL, &TaskBoot_attributes);
	TaskGUIHandle = osThreadNew(StartTaskGUI, NULL, &TaskGUI_attributes);
	TaskKeyHandle = osThreadNew(StartTaskKey, NULL, &TaskKey_attributes);
  TaskSEGHandle = osThreadNew(StartTaskSEG, NULL, &TaskSEG_attributes);
	TaskSupervisionHandle = osThreadNew(StartTaskSupervision, NULL, &TaskSupervision_attributes);
	TaskBeepHandle = osThreadNew(StartTaskBeep, NULL, &TaskBeep_attributes);
	vTaskSuspend(TaskSEGHandle);//挂起
	vTaskSuspend(myTaskWiFiHandle);//挂起
}

void StartDefaultTask(void *argument)
{
  
}


void StartTaskSupervision(void *argument)
{
	for(;;)
	{
		vTaskSuspend(TaskKeyHandle);//挂起
		vTaskSuspend(TaskGUIHandle);//挂起
		temperature_old = temperature_rt;
		temperature_rt = ds18b20_read();
		if(temperature_rt > 100)
			temperature_rt = temperature_old;
		MPU_getdata();
		shockValue = (gx*gx + gy*gy +gz*gz)/ 10000;
		osDelay(10);
		//printf("MPU Data:%4.1f %4.1f %4.1f, %5d %5d %5d, %5d %5d %5d\n", fAX, fAY, fAZ, ax, ay, az, gx, gy, gz);
		if(Temp_idx < 80)
			Temp_table[Temp_idx++] = temperature_rt;
		else 
		{
			memcpy((void* )Temp_table,(void* )(Temp_table+1),sizeof(float)*79);
			Temp_table[79] = temperature_rt;
		}
		if(FAX_idx < 80) 
			FAX_table[FAX_idx++] = fAX;
		else 
		{
			memcpy((void* )FAX_table,(void* )(FAX_table+1),sizeof(float)*79);
			FAX_table[79] = fAX;
		}
		if(FAY_idx < 80) 
			FAY_table[FAY_idx++] = fAY;
		else 
		{
			memcpy((void* )FAY_table,(void* )(FAY_table+1),sizeof(float)*79);
			FAY_table[79] = fAY;
		}
		if(FAZ_idx < 80) 
			FAZ_table[FAZ_idx++] = fAZ;
		else 
		{
			memcpy((void* )FAZ_table,(void* )(FAZ_table+1),sizeof(float)*79);
			FAZ_table[79] = fAZ;
		}
		
		if(temperature_rt > temperature_threshold && temperature_rt < 100 && alarm_flag == 0)
		{
			alarm_flag = 1;
			vTaskResume(TaskSEGHandle);//唤醒
		}
		if(sensitiveness_shock && alarm_flag == 0)
		{
			if(shockValue > (10-sensitiveness_shock)*250)
			{
				alarm_flag = 2;
				vTaskResume(TaskGUIHandle);//唤醒
				vTaskResume(TaskSEGHandle);//唤醒
			}
		}
		osDelay(50);
		vTaskResume(TaskKeyHandle);//唤醒
		vTaskResume(TaskGUIHandle);//唤醒
		osDelay(10);
	}	
}

void StartTaskBoot(void *argument)
{
	for(;;)
	{
		vTaskSuspend(TaskGUIHandle);//挂起
		vTaskSuspend(TaskSupervisionHandle);//挂起
		vTaskSuspend(myTaskWiFiHandle);//挂起
		vTaskResume(TaskKeyHandle);//唤醒
		if(boot_flag==0)
			DrawUIInit();
		else
			vTaskResume(TaskGUIHandle);//唤醒
		osDelay(1);
	}
}

void StartTaskLed(void *argument)
{
  for(;;)
  {
		vTaskSuspend(TaskGUIHandle);//挂起
		if(boot_flag==0)
		{
			SetLeds(sta);
			osDelay(200);
			if(sta < 0x80) sta<<=1;
			else sta = 0x01;
		}
		else
		{
			SetLeds(0x00);
			vTaskSuspend(TaskLedHandle);//挂起
		}
		osDelay(1);
	}
}

void StartTaskGUI(void *argument)
{
	for(;;)
	{
		vTaskSuspend(TaskBootHandle);
		SetLeds(0x00);
		vTaskSuspend(TaskLedHandle);
		vTaskResume(TaskSupervisionHandle);//唤醒
		vTaskResume(TaskBeepHandle);//唤醒
		vTaskResume(myTaskWiFiHandle);//唤醒
		if(boot_flag==1)
		{
			DrawUIMenu();
			Beep(50,0);
			Beep(50,7);
			Beep(50,4);
			Beep(50,3);
			boot_flag = 2;
		}
		if(boot_flag==2)
		{
			vTaskResume(TaskSupervisionHandle);//唤醒
			switch(func_index)
			{
				case 1:
					DrawFuncIndex(func_index);
					DrawRTSupervision(vision_index);
					break;
				case 2:
					DrawFuncIndex(func_index);
					DrawRTDataLine(display_index);
					break;
				case 3:
					DrawFuncIndex(func_index);
					DrawWiFi();
					vTaskResume(myTaskWiFiHandle);//唤醒
					break;
				case 4:
					DrawFuncIndex(func_index);
					DrawParamSetting(param_index);
					break;
				default:
					break;
			}
		}
		osDelay(1);
	}
}

void StartTaskKey(void *argument)
{
  for(;;)
  {
		key = KeyScan();
		switch(key)
		{
			case KEY_UP:
				if(boot_flag == 2)
				{
					func_index-=1;
					if(func_index < 1)
							func_index = 4;
				}
				break;
			case KEY_DOWN:
				if(boot_flag == 2)
				{
					func_index+=1;
					if(func_index > 4)
						func_index = 1;
				}
				break;
			case KEY_LEFT:
				if(boot_flag == 2)
				{
					if(func_index == 1)
					{
						vision_index -=1;
						if(vision_index < 1)
							vision_index = 3;
					}
					if(func_index == 2)
					{
						display_index -=1;
						if(display_index < 1)
							display_index = 4;
					}
					if (func_index == 3)
					{
						wifi_upload = 1;
						vTaskResume(myTaskWiFiHandle);//唤醒
					}
					if(func_index == 4)
					{
						switch(param_index)
						{
							case 1:
								if(temperature_threshold > 0)
									temperature_threshold -= 1;
								break;
							case 2:
								sensitiveness_shock -= 1;
								if(sensitiveness_shock < 0)
									sensitiveness_shock = 9;
								break;
							case 3:
								if(alarm_time > 0)
									alarm_time -= 1;
								break;
							case 4:
								if(upload_interval > 0)
									upload_interval -= 0.1f;
							default:
								break;
						}
					}
				}
				break;
			case KEY_RIGHT:
				if(boot_flag == 2)
				{
					if(func_index == 1)
					{
						vision_index +=1;
						if(vision_index > 3)
							vision_index = 1;
					}
					if(func_index == 2)
					{
						display_index +=1;
						if(display_index > 4)
							display_index = 1;
					}
					if (func_index == 3)
					{
						wifi_upload = 0;
						vTaskSuspend(myTaskWiFiHandle);
					}
					if(func_index == 4)
					{
						switch(param_index)
						{
							case 1:
								if(temperature_threshold < 90)
									temperature_threshold += 1;
								break;
							case 2:
								sensitiveness_shock += 1;
								if(sensitiveness_shock > 9)
									sensitiveness_shock = 0;
								break;
							case 3:
								if(alarm_time < 60)
									alarm_time += 1;
								break;
							case 4:
								if(upload_interval < 9.9f)
									upload_interval += 0.1f;
							default:
								break;
						}
					}
				}
				break;
			case KEY_SHIFT:
				if(boot_flag==0)
				{
					boot_flag = 1;
				}
				if(func_index == 4)
				{
					param_index+=1;
					if(param_index > 4)
						param_index = 1;
				}
				if(func_index == 3)
				{
					if(wifi_index == 1)
						wifi_index = 2;
					else if(wifi_index == 2)
					{
						send_num = 0;
						recv_num = 0;
						wifi_index = 3;
					}
					else if(wifi_index == 3)
						wifi_index = 1;
				}
				break;
			case KEY_RETURN:
				SetLeds(0x80);
				if(boot_flag==2)
				{
					boot_flag = 0;
					vTaskSuspend(TaskGUIHandle);//挂起
					vTaskSuspend(TaskSupervisionHandle);//挂起
					vTaskSuspend(TaskBeepHandle);//挂起
					vTaskSuspend(myTaskWiFiHandle);//挂起
					GUI_Clear();
					GUI_Update();
					vTaskResume(TaskBootHandle);//唤醒
					vTaskResume(TaskLedHandle);//唤醒
				}
				break;
			case KEY_ALL:
				alarm_flag = 0;
				SetLeds(0x80);
				break;
			default:
				break;
		}
    osDelay(20);
  }
}

void StartTaskSEG(void *argument)
{
	uint32_t tick, tick_sys, tick_end = 0, temp = 0, sec = 0;
  for(;;)
  {
		vTaskSuspend(TaskGUIHandle);//挂起
		vTaskResume(TaskKeyHandle);//唤醒
		vTaskResume(TaskSupervisionHandle);
		tick_sys = HAL_GetTick() / 10;
		if(temp == 0)
		{
			temp = 1;
			tick_end = tick_sys + alarm_time*100;
		}
		tick = tick_end - tick_sys;
		if((temperature_rt > temperature_threshold && temperature_rt < 100 && alarm_flag == 2)
			|| (sensitiveness_shock && shockValue > (10-sensitiveness_shock)*250 && alarm_flag == 1))
		{
			alarm_flag = 3;
		}
		if(tick_sys >= tick_end || alarm_flag == 0)
		{
			Beep(0,2);
			alarm_flag = 0;
			temp = 0;
			temperature_threshold += 1;
//			sensitiveness_shock = 0;
			osDelay(50);
			HAL_GPIO_WritePin(A3_GPIO_Port,A3_Pin,GPIO_PIN_RESET);
			vTaskResume(TaskGUIHandle);//唤醒
			vTaskSuspend(TaskSEGHandle);//挂起
		}
		else
		{
			sec = tick / 100;
			sec %= 60;
			if(alarm_flag >= 1)
			{
				//显示分秒
				Write595(0, (sec / 10) % 10, 0);
				osDelay(1);
				Write595(1, sec % 10, 1);
				osDelay(1);
				Write595(2, (tick / 10) % 10, 0);
				osDelay(1);
				Write595(3, tick % 10, 0);
				osDelay(1);
			}
			if(alarm_flag == 1)
				if(tick / 10 % 10 == 1 || tick / 10 % 10 == 3 || tick / 10 % 10 == 5)
					Beep(100,tick / 10 % 10);
			if(alarm_flag == 2)
				if(tick / 10 % 10 == 0 || tick / 10 % 10 == 4 || tick / 10 % 10 == 7)
					Beep(100,tick / 10 % 10);
			if(alarm_flag == 3)
				if(tick / 10 % 10 == 0 || tick / 10 % 10 == 1 || tick / 10 % 10 == 7)
					Beep(100,tick / 10 % 10);
		}
    osDelay(1);
  }
}

void StartTaskBeep(void *argument)
{
	HAL_TIM_Base_Start_IT(&htim4);
	uint32_t tick = osKernelGetTickCount() + 1000;
	
  for(;;)
  {
		if (osKernelGetTickCount() >= tick)
		{
			tick = osKernelGetTickCount() + 1000;
		}
    osDelay(1);
  }
}
void StartTaskHC(void *argument)
{
  /* USER CODE BEGIN StartTaskHC */
  /* Infinite loop */
	HC05_Init();
  for(;;)
  {
		if(start_flag == 1)
		{

				HAL_UART_Transmit(&huart2, HC_BUF, 15, 100);
		}
		
		//HAL_UART_Transmit(&huart2, HC_BUF, 15, 100);
    osDelay(50);
  }
  /* USER CODE END StartTaskHC */
}

extern __IO uint8_t TCP_RX_TEMP;
void StartTaskWiFi(void *argument)
{
  /* USER CODE BEGIN StartTaskWiFi */
  /* Infinite loop */
	char frame[100];
	char* posL, *posR;
	uint32_t tick = osKernelGetTickCount();
  for(;;)
  {
			//printf("task:wifi\n");
			if (esp8266_ready_flag == 0){
				esp8266_ready_flag = esp8266_init();
				HAL_UART_Receive_IT(&huart6, (uint8_t *)&TCP_RX_TEMP, 1); // 开启接受中断
			}
			else{
				if (wifi_upload && osKernelGetTickCount() > tick){ 
					tcp_data_printf("{\"type\":0,\"temp\":%.1f,\"ax\":%d,\"ay\":%d,\"az\":%d,\"gx\":%d,\"gy\":%d,\"gz\":%d,\"roll\":%.1f,\"pitch\":%.1f,\"yaw\":%.1f,\"warning\":%d}\n",
													temperature_rt,ax, ay, az, gx, gy, gz, fAX, fAY, fAZ, alarm_flag);
					send_num += strlen("{\"type\":0,\"temp\":%.1f,\"ax\":%d,\"ay\":%d,\"az\":%d,\"gx\":%d,\"gy\":%d,\"gz\":%d,\"roll\":%.1f,\"pitch\":%.1f,\"yaw\":%.1f,\"warning\":%d}\n");
					SetLeds(0xf0);
					tick =  osKernelGetTickCount() + upload_interval*1000;					
				}
				
				// 处理上位机发送的数据
				if ((posL = find_str_from_mes_buf((char*)"{"))!=NULL && (posR = find_str_from_mes_buf((char*)"}"))!=NULL)
				{
					memset(frame, '\0', 100);
					strncpy(frame, posL, (char)(posR-posL+1));
					clear_mes_buf();
					cJSON* server_data_obj = cJSON_Parse(frame);
					if (server_data_obj != NULL)
					{
						switch( cJSON_GetObjectItemCaseSensitive(server_data_obj, "type")->valueint) // type帧类型
						{
							case 2:{ // 回传参数
								tcp_data_printf("{\"type\":1,\"max_temp\":%d,\"shake_level\":%d,\"warning_time\":%u,\"upload_time\":%.1f}\n",
													temperature_threshold,sensitiveness_shock, alarm_time, upload_interval);
								break;
							}
							case 3:{ // 开始传输/停止传输
								wifi_upload = cJSON_GetObjectItemCaseSensitive(server_data_obj, "wifi_upload")->valueint;
								recv_num += strlen("wifi_upload%d\n");
								break;
							}
							case 4:{ // 设置参数
								temperature_threshold = cJSON_GetObjectItemCaseSensitive(server_data_obj, "max_temp")->valueint;
								sensitiveness_shock =  cJSON_GetObjectItemCaseSensitive(server_data_obj, "shake_level")->valueint;
								alarm_time = cJSON_GetObjectItemCaseSensitive(server_data_obj, "warning_time")->valueint;
								upload_interval = cJSON_GetObjectItemCaseSensitive(server_data_obj, "upload_time")->valuedouble;	
								recv_num += strlen("max_temp%d,shake_level%d,warning_time:%u,upload_time:%d\n");
								break;
							}
							default:
								break;
						}
						SetLeds(0x0f);
						cJSON_Delete(server_data_obj);						
					}
					
				}
				
			}
			
		osDelay(40);
  }
  /* USER CODE END StartTaskWiFi */
}
void DrawUIInit(void)
{
	uint8_t cnt = 10;
	GUI_Clear();
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_DispStringHCenterAt("专业实践",64,0);
	GUI_DispStringHCenterAt("综合设计Ⅱ",64,20);
	GUI_DispStringHCenterAt("防火防盗监测器",64,40);
	GUI_Update();
	while(cnt--)
	{
		if(boot_flag>0)
			return;
		osDelay(200);
	}
	GUI_Clear();
	GUI_DispStringAt("成员1 狄宇奕",0,0);
	GUI_DispStringAt("18041113",56,16);
	GUI_DispStringAt("成员2 卢秀强",0,32);
	GUI_DispStringAt("18041124",56,48);
	GUI_Update();
	cnt = 15;
	while(cnt--)
	{
		if(boot_flag>0)
			return;
		osDelay(200);
	}
	GUI_Clear();
	GUI_DrawBitmap(&bmDYY,0,0);
	GUI_DrawBitmap(&bmIMG_20210417_100002,64,0);
	GUI_Update();
	cnt = 15;
	while(cnt--)
	{
		if(boot_flag>0)
			return;
		osDelay(200);
	}
	boot_flag = 1;
}

void DrawUIMenu(void)
{
	GUI_Clear();
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_DispStringAt("实时监测",0,0);
	GUI_DispStringAt("数据曲线",0,12);
	GUI_DispStringAt("无线通信",0,24);
	GUI_DispStringAt("参数设置",0,36);
	GUI_DrawLine(48,0,48,48);
	GUI_DrawLine(0,50,127,50);
	GUI_DispStringAt("K1︽  K2《  K3》 K4︾",0,52);
}

void DrawRTSupervision(uint8_t vision_index)
{
	GUI_ClearRect(49,0,127,48);
	switch(vision_index)
	{
		case 1:
			GUI_DispStringAt("当前温度:",49,0);
			GUI_Write_float2(temperature_rt,80,12);
			GUI_DispStringAt("℃",108,12);
			GUI_DispStringAt("震动警报:",49,24);
			if(alarm_flag == 2)
				GUI_DispStringAt("警报",96,36);
			else
				GUI_DispStringAt("无",96,36);
			break;
		case 2:
			GUI_DispStringAt("ax:",52,0);
			GUI_Write_Num5(ax,72,0);
			if(ax > 0)
				GUI_FillRect(88,12,88+ax*55/32768,15);
			else if(ax < 0)
				GUI_DrawRect(88+ax*55/32768,12,88,15);
			GUI_DispStringAt("ay:",52,16);
			GUI_Write_Num5(ay,72,16);
			if(ay > 0)
				GUI_FillRect(88,28,88+ay*55/32768,31);
			else if(ay < 0)
				GUI_DrawRect(88+ay*55/32768,28,88,31);
			GUI_DispStringAt("az:",52,32);
			GUI_Write_Num5(az,72,32);
			if(az > 0)
				GUI_FillRect(88,44,88+az*55/32768,47);
			else if(az < 0)
				GUI_DrawRect(88+az*55/32768,44,88,47);
			break;
		case 3:
			GUI_DispStringAt("gx:",52,0);
			GUI_Write_Num5(gx,72,0);
			if(gx > 0)
				GUI_FillRect(88,12,88+gx*55/32768,15);
			else if(gx < 0)
				GUI_DrawRect(88+gx*55/32768,12,88,15);
			GUI_DispStringAt("gy:",52,16);
			GUI_Write_Num5(gy,72,16);
			if(gy > 0)
				GUI_FillRect(88,28,88+gy*55/32768,31);
			else if(gy < 0)
				GUI_DrawRect(88+gy*55/32768,28,88,31);
			GUI_DispStringAt("gz:",52,32);
			GUI_Write_Num5(gz,72,32);
			if(gz > 0)
				GUI_FillRect(88,44,88+gz*55/32768,47);
			else if(gz < 0)
				GUI_DrawRect(88+gz*55/32768,44,88,47);
			break;
		default:
			break;
	}
	GUI_Update();
}

void readData(float datas[], int idx, float data)
{
	if(idx < 80)
		datas[idx++] = data;
	else
	{
		memcpy((void*)datas,(void*)(datas + 1),sizeof(float) * 79);
		datas[79] = data;
	}
}

void DrawDataLine(float max, float min, float lmt, float data, float datas[], int cdata)
{
	int sw = 128 - 50;
	int sh = 64 - 12 - 12;
	int ox = 50;
	int oy = 12 + sh;
	float dh = sh / (max - min);
	if(lmt != 0)
	{
		for(int i=0;i<80; i+=6)
		GUI_DrawHLine(oy - (lmt - min) * dh, ox + i, ox + i + 3);
	}
	else
		GUI_DrawHLine(oy + min * dh, ox, ox + sw);
	for(int i = 1;i<cdata && i<80; ++i)
		GUI_DrawLine(ox + i - 1, oy - (datas[i-1] - min) * dh, ox + i, oy - (datas[i] - min) * dh);
}

void DrawRTDataLine(uint8_t display_index)
{
	GUI_ClearRect(49,0,127,48);
	switch(display_index)
	{
		case 1:
			GUI_DispStringAt("温度:",49,0);
			GUI_Write_float2(temperature_rt,80,0);
			GUI_DispStringAt("℃",106,0);
			DrawDataLine(35, 25, temperature_threshold, temperature_rt, Temp_table, Temp_idx);
			break;
		case 2:
			GUI_DispStringAt("横滚角：",49,0);
			GUI_Write_float3(fAX,90,0);
			GUI_DispStringAt("°",122,0);
			DrawDataLine(180, -180, 0, fAX, FAX_table, FAX_idx);
			break;
		case 3:
			GUI_DispStringAt("俯仰角：",49,0);
			GUI_Write_float3(fAY,90,0);
			GUI_DispStringAt("°",122,0);
			DrawDataLine(90, -90, 0, fAY, FAY_table, FAY_idx);
			break;
		case 4:
			GUI_DispStringAt("航向角：",49,0);
			GUI_Write_float3(fAZ,90,0);
			GUI_DispStringAt("°",122,0);
			DrawDataLine(180, -180, 0, fAZ, FAZ_table, FAZ_idx);
			break;
		default:
			break;
	}
	GUI_Update();
}

void DrawWiFi()
{
		GUI_ClearRect(49,0,127,48);
		switch(wifi_index)
		{
			case 1:
				GUI_DispStringAt("SSID:ESP8266",49,0);
				GUI_DispStringAt("Link:",49,12);
				GUI_DispStringLenAt((const char*)wifi_ip,8,79,12);
				GUI_DispStringAt((const char*)(wifi_ip+8),48,24);
				GUI_DispStringAt(":",48+(strlen((const char*)wifi_ip)-8)*6, 24);
				GUI_DispStringAt((const char*)wifi_port,54+(strlen((const char*)wifi_ip)-8)*6, 24);
				if(server_sta == SERVER_ON_LINE)
						GUI_DispStringAt("OK",49,36);
				else 
						GUI_DispStringAt("error",49,36);
				if(wifi_upload)
						GUI_DispStringAt("上传中",84,36);
				else 
						GUI_DispStringAt("已关闭",84,36);		
				break;
			case 2:
				GUI_DispStringAt("Send Bytes:",49,0);
				GUI_Write_Num5(send_num,90,12);
				GUI_DispStringAt("Recv Bytes:",49,24);
				GUI_Write_Num5(recv_num,90,36);
				break;
			case 3:
				GUI_DispStringAt("Send Bytes:",49,0);
				GUI_Write_Num5(send_num,90,12);
				GUI_DispStringAt("Recv Bytes:",49,24);
				GUI_Write_Num5(recv_num,90,36);
				break;
		}
		GUI_Update();
}

void DrawParamSetting(uint8_t param_index)
{
	GUI_ClearRect(49,0,127,48);
	GUI_DispStringAt("温度上限:  ℃",49,0);
	GUI_DispStringAt("震动灵敏度:",49,12);
	GUI_DispStringAt("报警时长:  S",49,24);
	GUI_DispStringAt("上传间隔:   S",49,36);
	GUI_Write_Num2(temperature_threshold,102,0);
	GUI_Write_Num2(sensitiveness_shock,114,12);
	GUI_Write_Num2(alarm_time,102,24);
	GUI_Write_float1(upload_interval,102,36);
	switch(param_index)
	{
		case 1:
			GUI_InvertRect(102,0,113,11);
			break;
		case 2:
			GUI_InvertRect(114,12,125,23);
			break;
		case 3:
			GUI_InvertRect(102,24,113,35);
			break;
		case 4:
			GUI_InvertRect(102,36,118,47);
		default:
			break;
	}
	GUI_Update();
}

void DrawFuncIndex(uint8_t func_index)
{
	switch(func_index)
	{
		case 1:
			GUI_DispStringAt("实时监测",0,0);
			GUI_DispStringAt("数据曲线",0,12);
			GUI_DispStringAt("参数设置",0,36);
			GUI_InvertRect(0,0,47,11);
			break;
		case 2:
			GUI_DispStringAt("实时监测",0,0);
			GUI_DispStringAt("数据曲线",0,12);
			GUI_DispStringAt("无线通信",0,24);
			GUI_InvertRect(0,12,47,23);
			break;
		case 3:
			GUI_DispStringAt("数据曲线",0,12);
			GUI_DispStringAt("无线通信",0,24);
			GUI_DispStringAt("参数设置",0,36);
			GUI_InvertRect(0,24,47,35);
			break;
		case 4:
			GUI_DispStringAt("实时监测",0,0);
			GUI_DispStringAt("无线通信",0,24);
			GUI_DispStringAt("参数设置",0,36);
			GUI_InvertRect(0,32,47,47);
			break;
		default:
			break;
	}
}
void Beep(uint16_t time,uint8_t tune)
{
	static const float TAB[7] = {261.62f,293.67f,329.63f,349.23f,391.99f,440.00f,493.88f};
	if(tune >0 && tune < 7)
	{
		g_beep_time = time;

		int arr = 1000000 /(TAB[tune -1] * 2);
		__HAL_TIM_SetAutoreload(&htim4,arr);
	}
}
