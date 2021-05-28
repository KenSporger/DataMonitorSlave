/**
  ******************************************************************************
  * File Name          : gpio.c
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, CS_Pin|SER_Pin|DISEN_Pin|A1_Pin
                          |A2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, L1_Pin|L2_Pin|L3_Pin|L4_Pin
                          |L5_Pin|L6_Pin|L7_Pin|L8_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, DISLK_Pin|SCK_Pin|A3_Pin|A0_Pin, GPIO_PIN_RESET);
	
	/*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, MPU_GND_Pin|MPU_SDA_Pin|MPU_SCL_Pin
													|OLED_SCL_Pin|OLED_SDA_Pin|BEEP_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(HC05_KEY_GPIO_Port, HC05_KEY_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PEPin PEPin PEPin PEPin */
  GPIO_InitStruct.Pin = SW2_Pin|SW3_Pin|SW4_Pin|SW1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PEPin PEPin */
  GPIO_InitStruct.Pin = SW5_Pin|SW6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PCPin PCPin PCPin PCPin
                           PCPin */
  GPIO_InitStruct.Pin = CS_Pin|SER_Pin|DISEN_Pin|A1_Pin
                          |A2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PEPin PEPin PEPin PEPin
                           PEPin PEPin PEPin PEPin */
  GPIO_InitStruct.Pin = L1_Pin|L2_Pin|L3_Pin|L4_Pin
                          |L5_Pin|L6_Pin|L7_Pin|L8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	/*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = MPU_GND_Pin|BEEP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(MPU_GND_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin PBPin */
  GPIO_InitStruct.Pin = MPU_SDA_Pin|MPU_SCL_Pin|OLED_SCL_Pin|OLED_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	/*Configure GPIO pins : PAPin PAPin PAPin PAPin */
  GPIO_InitStruct.Pin = DISLK_Pin|SCK_Pin|A3_Pin|A0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = HC05_KEY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(HC05_KEY_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PD3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */
void SetLeds(uint8_t sta)
{
	HAL_GPIO_WritePin(L1_GPIO_Port,L1_Pin,(sta & 0x01) ? GPIO_PIN_RESET:GPIO_PIN_SET);
	HAL_GPIO_WritePin(L2_GPIO_Port,L2_Pin,(sta & 0x02) ? GPIO_PIN_RESET:GPIO_PIN_SET);
	HAL_GPIO_WritePin(L3_GPIO_Port,L3_Pin,(sta & 0x04) ? GPIO_PIN_RESET:GPIO_PIN_SET);
	HAL_GPIO_WritePin(L4_GPIO_Port,L4_Pin,(sta & 0x08) ? GPIO_PIN_RESET:GPIO_PIN_SET);
	HAL_GPIO_WritePin(L5_GPIO_Port,L5_Pin,(sta & 0x10) ? GPIO_PIN_RESET:GPIO_PIN_SET);
	HAL_GPIO_WritePin(L6_GPIO_Port,L6_Pin,(sta & 0x20) ? GPIO_PIN_RESET:GPIO_PIN_SET);
	HAL_GPIO_WritePin(L7_GPIO_Port,L7_Pin,(sta & 0x40) ? GPIO_PIN_RESET:GPIO_PIN_SET);
	HAL_GPIO_WritePin(L8_GPIO_Port,L8_Pin,(sta & 0x80) ? GPIO_PIN_RESET:GPIO_PIN_SET);
}

extern uint8_t alarm_flag;
uint8_t KeyScan()
{
	uint8_t key = 0;
	uint32_t tick_sys = HAL_GetTick();
	uint32_t tick_end = tick_sys + 3000;
	uint32_t tick = 0;
	if(alarm_flag)
	{		
		while(HAL_GPIO_ReadPin(SW1_GPIO_Port,SW1_Pin) == GPIO_PIN_RESET && HAL_GPIO_ReadPin(SW2_GPIO_Port,SW2_Pin) == GPIO_PIN_RESET
			&& HAL_GPIO_ReadPin(SW3_GPIO_Port,SW3_Pin) == GPIO_PIN_RESET && HAL_GPIO_ReadPin(SW4_GPIO_Port,SW4_Pin) == GPIO_PIN_RESET)
		{
			tick = HAL_GetTick();
			if(tick > tick_end)
			{
				key = KEY_ALL;
				return key;
			}
		}
	}
	else
	{
		if(HAL_GPIO_ReadPin(SW1_GPIO_Port,SW1_Pin) == GPIO_PIN_RESET)
		{
			key = KEY_UP;
			HAL_Delay(100);
			if(HAL_GPIO_ReadPin(SW1_GPIO_Port,SW1_Pin) == GPIO_PIN_RESET)
				return key;
			else
				return 0;
		}
		else if(HAL_GPIO_ReadPin(SW2_GPIO_Port,SW2_Pin) == GPIO_PIN_RESET)
		{
			key = KEY_LEFT;
			HAL_Delay(100);
			if(HAL_GPIO_ReadPin(SW2_GPIO_Port,SW2_Pin) == GPIO_PIN_RESET)
				return key;
			else
				return 0;
		}
		else if(HAL_GPIO_ReadPin(SW3_GPIO_Port,SW3_Pin) == GPIO_PIN_RESET)
		{
			key = KEY_RIGHT;
			HAL_Delay(100);
			if(HAL_GPIO_ReadPin(SW3_GPIO_Port,SW3_Pin) == GPIO_PIN_RESET)
				return key;
			else
				return 0;
		}
		else if(HAL_GPIO_ReadPin(SW4_GPIO_Port,SW4_Pin) == GPIO_PIN_RESET)
		{
			key = KEY_DOWN;
			HAL_Delay(100);
			if(HAL_GPIO_ReadPin(SW4_GPIO_Port,SW4_Pin) == GPIO_PIN_RESET)
				return key;
			else
				return 0;
		}
		else if(HAL_GPIO_ReadPin(SW5_GPIO_Port,SW5_Pin) == GPIO_PIN_SET)
		{
			key = KEY_SHIFT;
			HAL_Delay(100);
			if(HAL_GPIO_ReadPin(SW5_GPIO_Port,SW5_Pin) == GPIO_PIN_SET)
				return key;
			else
				return 0;
		}
		else if(HAL_GPIO_ReadPin(SW6_GPIO_Port,SW6_Pin) == GPIO_PIN_SET)
		{
			key = KEY_RETURN;
			HAL_Delay(100);
			if(HAL_GPIO_ReadPin(SW6_GPIO_Port,SW6_Pin) == GPIO_PIN_SET)
				return key;
			else
				return 0;
		}
	}
	return 0;
}


void Write595(uint8_t sel,uint8_t num,uint8_t bdot)	//????? '0'~'7' , '8'~'F'
{
	static const uint8_t TAB[16] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,
																	0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};

	HAL_GPIO_WritePin(A3_GPIO_Port,A3_Pin,GPIO_PIN_RESET);
	uint8_t dat = TAB[num & 0x0F] |(bdot?0x80:0x00);	

	for(uint8_t i=0;i<8;++i)
	{
		HAL_GPIO_WritePin(SCK_GPIO_Port,SCK_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SER_GPIO_Port,SER_Pin,(dat & 0x80)? GPIO_PIN_SET:GPIO_PIN_RESET);
		dat<<=1;
		HAL_GPIO_WritePin(SCK_GPIO_Port,SCK_Pin,GPIO_PIN_SET);
	}
	HAL_GPIO_WritePin(DISLK_GPIO_Port,DISLK_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DISLK_GPIO_Port,DISLK_Pin,GPIO_PIN_SET);

	HAL_GPIO_WritePin(A0_GPIO_Port,A0_Pin,(sel & 0x01) ? GPIO_PIN_SET:GPIO_PIN_RESET);
	HAL_GPIO_WritePin(A1_GPIO_Port,A1_Pin,(sel & 0x02) ? GPIO_PIN_SET:GPIO_PIN_RESET);
	HAL_GPIO_WritePin(A2_GPIO_Port,A2_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(A3_GPIO_Port,A3_Pin,GPIO_PIN_SET);
}
/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
