/**
 ******************************************************************************
 * @file           : dvc_motor.hpp
 * @brief          : header file for dvc_motor.cpp
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 GMaster
 * All rights reserved.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#pragma once

/* Includes ------------------------------------------------------------------*/
#include "gsrl_common.h"
#include "alg_pid.hpp"
#include "tim.h"

/* Exported types ------------------------------------------------------------*/

/**
 * @brief TB6612FNG电机驱动模式枚举
 */
enum class MotorMode : uint8_t
{
    STOP = 0,    // 停止（刹车）
    FORWARD,     // 正转
    BACKWARD,    // 反转
    BRAKE        // 短路制动
};

/**
 * @brief TB6612FNG驱动的370减速电机类
 * 
 * 该类用于控制通过TB6612FNG模块驱动的370减速电机
 * 支持开环PWM速度控制
 */
class Motor
{
private:
    TIM_HandleTypeDef *m_htim;    // 定时器句柄
    uint32_t m_channel;            // PWM通道
    GPIO_TypeDef *m_in1_port;      // IN1引脚端口
    uint16_t m_in1_pin;            // IN1引脚号
    GPIO_TypeDef *m_in2_port;      // IN2引脚端口
    uint16_t m_in2_pin;            // IN2引脚号
    
    int16_t m_speed;               // 当前速度值 (-1000 ~ 1000)
    MotorMode m_mode;              // 当前模式
    
    const uint16_t PWM_MAX = 1000; // PWM最大值（ARR=1000）

    /**
     * @brief 设置电机模式
     * @param mode 电机模式
     */
    void setMode(MotorMode mode);

public:
    /**
     * @brief 构造函数
     * @param htim 定时器句柄指针
     * @param channel PWM通道 (TIM_CHANNEL_1/2/3/4)
     * @param in1_port IN1引脚端口
     * @param in1_pin IN1引脚号
     * @param in2_port IN2引脚端口
     * @param in2_pin IN2引脚号
     */
    Motor(TIM_HandleTypeDef *htim, uint32_t channel,
          GPIO_TypeDef *in1_port, uint16_t in1_pin,
          GPIO_TypeDef *in2_port, uint16_t in2_pin);

    /**
     * @brief 初始化电机
     */
    void init(void);

    /**
     * @brief 设置电机速度（开环PWM控制）
     * @param speed 速度值 (-1000 ~ 1000)
     *              正值：正转，负值：反转，0：停止
     */
    void setSpeed(int16_t speed);

    /**
     * @brief 停止电机
     */
    void stop(void);

    /**
     * @brief 刹车（短路制动）
     */
    void brake(void);

    /**
     * @brief 获取当前速度设定值
     * @return 当前速度 (-1000 ~ 1000)
     */
    int16_t getSpeed(void) const { return m_speed; }

    /**
     * @brief 获取当前模式
     * @return 当前电机模式
     */
    MotorMode getMode(void) const { return m_mode; }
};

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/

/* Defines -------------------------------------------------------------------*/
