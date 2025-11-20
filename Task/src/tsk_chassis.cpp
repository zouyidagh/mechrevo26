/**
 ******************************************************************************
 * @file           : tsk_chassis.cpp
 * @brief          : 底盘控制任务
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 GMaster
 * All rights reserved.
 *
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "cmsis_os.h"
#include "crt_chassis.hpp"
#include "dvc_motor.hpp"
#include "main.h"
#include "tim.h"
#include "gpio.h"

/* Define --------------------------------------------------------------------*/
/******************************************************************************
 *                            硬件接口定义
 ******************************************************************************/
// 遥控器SPI2 CS引脚（使用PB12作为片选）
#define RC_CS_PORT SPI2_CS_GPIO_Port
#define RC_CS_PIN  SPI2_CS_Pin

/* Variables -----------------------------------------------------------------*/
/******************************************************************************
 *                            电机实例化
 ******************************************************************************/
// 左前轮电机: TIM1_CH3, AIN21, AIN22
static Motor motorLF(&htim1, TIM_CHANNEL_3, AIN21_GPIO_Port, AIN21_Pin, AIN22_GPIO_Port, AIN22_Pin);

// 右前轮电机: TIM1_CH4, BIN21, BIN22
static Motor motorRF(&htim1, TIM_CHANNEL_4, BIN21_GPIO_Port, BIN21_Pin, BIN22_GPIO_Port, BIN22_Pin);

// 左后轮电机: TIM2_CH2, AIN11, AIN12
static Motor motorLR(&htim2, TIM_CHANNEL_2, AIN11_GPIO_Port, AIN11_Pin, AIN12_GPIO_Port, AIN12_Pin);

// 右后轮电机: TIM2_CH1, BIN11, BIN12
static Motor motorRR(&htim2, TIM_CHANNEL_1, BIN11_GPIO_Port, BIN11_Pin, BIN12_GPIO_Port, BIN12_Pin);


/******************************************************************************
 *                            底盘实例化
 ******************************************************************************/
// 麦克纳姆轮底盘
static Chassis chassis(&motorLF, &motorRF, &motorLR, &motorRR);

/* Function prototypes -------------------------------------------------------*/

/* User code -----------------------------------------------------------------*/

/**
 * @brief 底盘控制任务
 * @param argument 任务参数（未使用）
 * 
 * 任务功能：
 * 1. 初始化底盘、电机、遥控器
 * 2. 以1ms周期运行底盘控制循环
 * 3. 读取遥控器数据并控制底盘运动
 */
extern "C" void chassis_task(void *argument)
{
    TickType_t taskLastWakeTime = xTaskGetTickCount(); // 获取任务开始时间
    
    // 初始化底盘（会同时初始化所有电机）
    chassis.init();
    
    // 主循环
    while(1)
    {
        // 调用底盘控制循环（读取遥控器并控制电机）
        chassis.chassisControlLoop();
        
        // 确保任务以定周期1ms运行
        vTaskDelayUntil(&taskLastWakeTime, pdMS_TO_TICKS(1));
    }
}
