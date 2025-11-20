/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f1xx_hal.h"

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
#define AIN22_Pin GPIO_PIN_2
#define AIN22_GPIO_Port GPIOA
#define AIN21_Pin GPIO_PIN_3
#define AIN21_GPIO_Port GPIOA
#define BIN21_Pin GPIO_PIN_6
#define BIN21_GPIO_Port GPIOA
#define BIN22_Pin GPIO_PIN_7
#define BIN22_GPIO_Port GPIOA
#define ATT_Pin GPIO_PIN_12
#define ATT_GPIO_Port GPIOB
#define CLK_Pin GPIO_PIN_13
#define CLK_GPIO_Port GPIOB
#define CMD_Pin GPIO_PIN_14
#define CMD_GPIO_Port GPIOB
#define DAT_Pin GPIO_PIN_15
#define DAT_GPIO_Port GPIOB
#define SHOOT_Pin GPIO_PIN_12
#define SHOOT_GPIO_Port GPIOA
#define AIN11_Pin GPIO_PIN_15
#define AIN11_GPIO_Port GPIOA
#define AIN12_Pin GPIO_PIN_3
#define AIN12_GPIO_Port GPIOB
#define BIN11_Pin GPIO_PIN_8
#define BIN11_GPIO_Port GPIOB
#define BIN12_Pin GPIO_PIN_9
#define BIN12_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
