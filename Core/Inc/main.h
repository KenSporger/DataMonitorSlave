/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SW2_Pin GPIO_PIN_2
#define SW2_GPIO_Port GPIOE
#define SW3_Pin GPIO_PIN_3
#define SW3_GPIO_Port GPIOE
#define SW4_Pin GPIO_PIN_4
#define SW4_GPIO_Port GPIOE
#define SW5_Pin GPIO_PIN_5
#define SW5_GPIO_Port GPIOE
#define SW6_Pin GPIO_PIN_6
#define SW6_GPIO_Port GPIOE
#define CS_Pin GPIO_PIN_4
#define CS_GPIO_Port GPIOC
#define L1_Pin GPIO_PIN_8
#define L1_GPIO_Port GPIOE
#define L2_Pin GPIO_PIN_9
#define L2_GPIO_Port GPIOE
#define L3_Pin GPIO_PIN_10
#define L3_GPIO_Port GPIOE
#define L4_Pin GPIO_PIN_11
#define L4_GPIO_Port GPIOE
#define L5_Pin GPIO_PIN_12
#define L5_GPIO_Port GPIOE
#define L6_Pin GPIO_PIN_13
#define L6_GPIO_Port GPIOE
#define L7_Pin GPIO_PIN_14
#define L7_GPIO_Port GPIOE
#define L8_Pin GPIO_PIN_15
#define L8_GPIO_Port GPIOE
#define MPU_GND_Pin GPIO_PIN_10
#define MPU_GND_GPIO_Port GPIOB
#define MPU_SDA_Pin GPIO_PIN_13
#define MPU_SDA_GPIO_Port GPIOB
#define MPU_SCL_Pin GPIO_PIN_15
#define MPU_SCL_GPIO_Port GPIOB
#define SER_Pin GPIO_PIN_8
#define SER_GPIO_Port GPIOC
#define DISEN_Pin GPIO_PIN_9
#define DISEN_GPIO_Port GPIOC
#define DISLK_Pin GPIO_PIN_8
#define DISLK_GPIO_Port GPIOA
#define SCK_Pin GPIO_PIN_11
#define SCK_GPIO_Port GPIOA
#define A3_Pin GPIO_PIN_12
#define A3_GPIO_Port GPIOA
#define A0_Pin GPIO_PIN_15
#define A0_GPIO_Port GPIOA
#define A1_Pin GPIO_PIN_10
#define A1_GPIO_Port GPIOC
#define A2_Pin GPIO_PIN_11
#define A2_GPIO_Port GPIOC
#define HC05_KEY_Pin GPIO_PIN_2
#define HC05_KEY_GPIO_Port GPIOD
#define BEEP_Pin GPIO_PIN_4
#define BEEP_GPIO_Port GPIOB
#define DQ_Pin GPIO_PIN_0
#define DQ_GPIO_Port GPIOE
#define SW1_Pin GPIO_PIN_1
#define SW1_GPIO_Port GPIOE
#define OLED_SCL_Pin GPIO_PIN_6
#define OLED_SDA_Pin GPIO_PIN_7
#define OLED_SCL_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
