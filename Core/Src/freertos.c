/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for chassis */
osThreadId_t chassisHandle;
uint32_t chassisBuffer[ 512 ];
osStaticThreadDef_t chassisControlBlock;
const osThreadAttr_t chassis_attributes = {
  .name = "chassis",
  .cb_mem = &chassisControlBlock,
  .cb_size = sizeof(chassisControlBlock),
  .stack_mem = &chassisBuffer[0],
  .stack_size = sizeof(chassisBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for gimbal */
osThreadId_t gimbalHandle;
uint32_t gimbalBuffer[ 128 ];
osStaticThreadDef_t gimbalControlBlock;
const osThreadAttr_t gimbal_attributes = {
  .name = "gimbal",
  .cb_mem = &gimbalControlBlock,
  .cb_size = sizeof(gimbalControlBlock),
  .stack_mem = &gimbalBuffer[0],
  .stack_size = sizeof(gimbalBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ps2 */
osThreadId_t ps2Handle;
uint32_t ps2Buffer[ 128 ];
osStaticThreadDef_t ps2ControlBlock;
const osThreadAttr_t ps2_attributes = {
  .name = "ps2",
  .cb_mem = &ps2ControlBlock,
  .cb_size = sizeof(ps2ControlBlock),
  .stack_mem = &ps2Buffer[0],
  .stack_size = sizeof(ps2Buffer),
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void chassis_task(void *argument);
extern void gimbal_task(void *argument);
void ps2_task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of chassis */
  chassisHandle = osThreadNew(chassis_task, NULL, &chassis_attributes);

  /* creation of gimbal */
  gimbalHandle = osThreadNew(gimbal_task, NULL, &gimbal_attributes);

  /* creation of ps2 */
  ps2Handle = osThreadNew(ps2_task, NULL, &ps2_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_chassis_task */
/**
  * @brief  Function implementing the chassis thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_chassis_task */
__weak void chassis_task(void *argument)
{
  /* USER CODE BEGIN chassis_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END chassis_task */
}

/* USER CODE BEGIN Header_ps2_task */
/**
* @brief Function implementing the ps2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ps2_task */
__weak void ps2_task(void *argument)
{
  /* USER CODE BEGIN ps2_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END ps2_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

