/**
 ******************************************************************************
 * @file           : dvc_motor.cpp
 * @brief          : 电机模块
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 GMaster
 * All rights reserved.
 *
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "dvc_motor.hpp"
#include "alg_general.hpp"
#include "tim.h"
#include <math.h>
#include <algorithm>

/* Typedef -------------------------------------------------------------------*/

/* Define --------------------------------------------------------------------*/

/* Macro ---------------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/* User code -----------------------------------------------------------------*/

/**
 * @brief 构造函数
 * @param htim 定时器句柄指针
 * @param channel PWM通道
 * @param in1_port IN1引脚端口
 * @param in1_pin IN1引脚号
 * @param in2_port IN2引脚端口
 * @param in2_pin IN2引脚号
 */
Motor::Motor(TIM_HandleTypeDef *htim, uint32_t channel,
             GPIO_TypeDef *in1_port, uint16_t in1_pin,
             GPIO_TypeDef *in2_port, uint16_t in2_pin)
    : m_htim(htim),
      m_channel(channel),
      m_in1_port(in1_port),
      m_in1_pin(in1_pin),
      m_in2_port(in2_port),
      m_in2_pin(in2_pin),
      m_speed(0),
      m_mode(MotorMode::STOP)
{
}

/**
 * @brief 初始化电机
 */
void Motor::init(void)
{
    // 启动PWM输出
    HAL_TIM_PWM_Start(m_htim, m_channel);
    
    // 初始化GPIO为停止状态
    HAL_GPIO_WritePin(m_in1_port, m_in1_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(m_in2_port, m_in2_pin, GPIO_PIN_RESET);
    
    // 设置初始PWM为0
    __HAL_TIM_SET_COMPARE(m_htim, m_channel, 0);
    
    m_speed = 0;
    m_mode = MotorMode::STOP;
}

/**
 * @brief 设置电机模式
 * @param mode 电机模式
 * 
 * TB6612FNG真值表：
 * IN1 | IN2 | 功能
 * ----|-----|------
 *  0  |  0  | 停止（短路制动）
 *  0  |  1  | 反转
 *  1  |  0  | 正转
 *  1  |  1  | 短路制动
 */
void Motor::setMode(MotorMode mode)
{
    m_mode = mode;
    
    switch (mode)
    {
    case MotorMode::FORWARD:
        // 正转: IN1=1, IN2=0
        HAL_GPIO_WritePin(m_in1_port, m_in1_pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(m_in2_port, m_in2_pin, GPIO_PIN_RESET);
        break;
        
    case MotorMode::BACKWARD:
        // 反转: IN1=0, IN2=1
        HAL_GPIO_WritePin(m_in1_port, m_in1_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(m_in2_port, m_in2_pin, GPIO_PIN_SET);
        break;
        
    case MotorMode::BRAKE:
        // 短路制动: IN1=1, IN2=1
        HAL_GPIO_WritePin(m_in1_port, m_in1_pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(m_in2_port, m_in2_pin, GPIO_PIN_SET);
        break;
        
    case MotorMode::STOP:
    default:
        // 停止: IN1=0, IN2=0
        HAL_GPIO_WritePin(m_in1_port, m_in1_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(m_in2_port, m_in2_pin, GPIO_PIN_RESET);
        break;
    }
}

/**
 * @brief 设置电机速度（开环PWM控制）
 * @param speed 速度值 (-1000 ~ 1000)
 *              正值：正转，负值：反转，0：停止
 */
void Motor::setSpeed(int16_t speed)
{
    // 限幅到 [-1000, 1000]
    m_speed = std::clamp(speed, (int16_t)-1000, (int16_t)1000);
    
    // 判断方向并设置模式
    if (m_speed > 0)
    {
        // 正转
        setMode(MotorMode::FORWARD);
        __HAL_TIM_SET_COMPARE(m_htim, m_channel, m_speed);
    }
    else if (m_speed < 0)
    {
        // 反转
        setMode(MotorMode::BACKWARD);
        __HAL_TIM_SET_COMPARE(m_htim, m_channel, -m_speed);
    }
    else
    {
        // 停止
        stop();
    }
}

/**
 * @brief 停止电机
 */
void Motor::stop(void)
{
    m_speed = 0;
    setMode(MotorMode::STOP);
    __HAL_TIM_SET_COMPARE(m_htim, m_channel, 0);
}

/**
 * @brief 刹车（短路制动）
 */
void Motor::brake(void)
{
    m_speed = 0;
    setMode(MotorMode::BRAKE);
    __HAL_TIM_SET_COMPARE(m_htim, m_channel, 0);
}

