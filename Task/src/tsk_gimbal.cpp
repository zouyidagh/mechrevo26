/**
 ******************************************************************************
 * @file           : tsk_gimbal.cpp
 * @brief          : 云台控制任务
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
#include "crt_gimbal.hpp"
#include "main.h"
#include "tim.h"
#include "gpio.h"

/* Define --------------------------------------------------------------------*/
/******************************************************************************
 *                            硬件接口定义
 ******************************************************************************/
// TIM3定时器通道定义
// Yaw舵机: TIM3_CH1
// Pitch舵机: TIM3_CH2

// 发射马达GPIO（在main.h中定义为SHOOT）
#define SHOOT_MOTOR_PORT SHOOT_GPIO_Port
#define SHOOT_MOTOR_PIN  SHOOT_Pin

/* Variables -----------------------------------------------------------------*/
/******************************************************************************
 *                            云台实例化
 ******************************************************************************/
// 云台控制对象
// 参数: (Yaw定时器, Yaw通道, Pitch定时器, Pitch通道, 发射马达GPIO端口, 发射马达GPIO引脚)
static Gimbal gimbal(&htim3, TIM_CHANNEL_1,    // Yaw舵机
                     &htim3, TIM_CHANNEL_2,    // Pitch舵机
                     SHOOT_MOTOR_PORT, SHOOT_MOTOR_PIN);  // 发射马达

/* Function prototypes -------------------------------------------------------*/

/* User code -----------------------------------------------------------------*/

/**
 * @brief 云台控制任务
 * @param argument 任务参数（未使用）
 * 
 * 任务功能：
 * 1. 初始化云台（启动PWM，复位到中位，关闭发射马达）
 * 2. 以2ms周期运行云台控制循环
 * 3. 读取遥控器右摇杆数据并控制云台舵机
 * 4. 读取RIGHT_1按键并控制发射马达
 * 
 * 遥控器映射：
 * - 右摇杆X轴：控制Yaw角度（左右转动）
 * - 右摇杆Y轴：控制Pitch角度（上下转动）
 * - RIGHT_1按键：控制发射马达开关
 */
extern "C" void gimbal_task(void *argument)
{
    TickType_t taskLastWakeTime = xTaskGetTickCount(); // 获取任务开始时间
    
    // 初始化云台
    gimbal.init();
    
    // 主循环
    while(1)
    {
        // 调用云台控制循环（读取遥控器并控制舵机和马达）
        gimbal.gimbalControlLoop();
        
        // 确保任务以定周期2ms运行
        vTaskDelayUntil(&taskLastWakeTime, pdMS_TO_TICKS(2));
    }
}
