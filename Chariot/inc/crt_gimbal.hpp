/**
 ******************************************************************************
 * @file           : crt_gimbal.hpp
 * @brief          : 云台控制
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
#include "main.h"
#include "tim.h"
#include "gpio.h"
#include "ps2.h"
#include "std_typedef.h"

/* Exported types ------------------------------------------------------------*/

/**
 * @brief 云台控制类
 * 
 * 控制云台的两个舵机（Yaw和Pitch）以及一个发射马达
 * 
 * 舵机参数：
 * - 型号：SG90 180度舵机
 * - PWM频率：50Hz (20ms周期)
 * - 脉宽范围：1ms(0度) ~ 2ms(180度)
 * - CCR范围：1000(0度) ~ 2000(180度)
 * 
 * 定时器配置（72MHz主频）：
 * - PSC：72-1，计数频率：1MHz
 * - ARR：20000-1，PWM周期：20ms (50Hz)
 */
class Gimbal
{
private:
    // 定时器和通道
    TIM_HandleTypeDef *m_htimYaw;      // Yaw舵机定时器句柄
    uint32_t m_channelYaw;              // Yaw舵机定时器通道
    
    TIM_HandleTypeDef *m_htimPitch;    // Pitch舵机定时器句柄
    uint32_t m_channelPitch;            // Pitch舵机定时器通道
    
    // 发射马达GPIO
    GPIO_TypeDef *m_shootMotorPort;     // 发射马达GPIO端口
    uint16_t m_shootMotorPin;           // 发射马达GPIO引脚
    
    // 舵机角度（单位：度）
    fp32 m_yawAngle;                    // Yaw舵机当前角度 (0~180)
    fp32 m_pitchAngle;                  // Pitch舵机当前角度 (0~180)
    
    // 舵机角度限制
    constexpr static fp32 m_minAngle = 0.0f;     // 最小角度
    constexpr static fp32 m_maxAngle = 180.0f;   // 最大角度
    
    // 舵机中位
    constexpr static fp32 m_yawCenter = 90.0f;    // Yaw舵机中位角度
    constexpr static fp32 m_pitchCenter = 90.0f;  // Pitch舵机中位角度
    
    // PWM参数（对应1MHz计数频率）
    constexpr static uint32_t m_pulseMin = 500;  // 0度对应的CCR值
    constexpr static uint32_t m_pulseMax = 2500;  // 180度对应的CCR值
    
    // 遥控器灵敏度
    constexpr static fp32 m_sensitivity = 0.1f;   // 摇杆灵敏度（度/步）

public:
    /**
     * @brief 构造函数
     * @param htimYaw Yaw舵机定时器句柄
     * @param channelYaw Yaw舵机定时器通道
     * @param htimPitch Pitch舵机定时器句柄
     * @param channelPitch Pitch舵机定时器通道
     * @param shootMotorPort 发射马达GPIO端口
     * @param shootMotorPin 发射马达GPIO引脚
     */
    Gimbal(TIM_HandleTypeDef *htimYaw, uint32_t channelYaw,
           TIM_HandleTypeDef *htimPitch, uint32_t channelPitch,
           GPIO_TypeDef *shootMotorPort, uint16_t shootMotorPin);

    /**
     * @brief 初始化云台
     */
    void init(void);

    /**
     * @brief 设置Yaw舵机角度
     * @param angle 目标角度（0~180度）
     */
    void setYawAngle(fp32 angle);

    /**
     * @brief 设置Pitch舵机角度
     * @param angle 目标角度（0~180度）
     */
    void setPitchAngle(fp32 angle);

    /**
     * @brief 设置发射马达状态
     * @param enable true:启动，false:停止
     */
    void setShootMotor(bool enable);

    /**
     * @brief 云台复位到中位
     */
    void reset(void);

    /**
     * @brief 云台控制循环（使用遥控器控制）
     * 
     * 右摇杆控制云台朝向：
     * - 右摇杆X轴：控制Yaw角度（左右转动）
     * - 右摇杆Y轴：控制Pitch角度（上下转动）
     * - RIGHT_1按键：控制发射马达
     */
    void gimbalControlLoop(void);

    /**
     * @brief 获取Yaw角度
     */
    fp32 getYawAngle(void) const { return m_yawAngle; }

    /**
     * @brief 获取Pitch角度
     */
    fp32 getPitchAngle(void) const { return m_pitchAngle; }

private:
    /**
     * @brief 将角度转换为PWM的CCR值
     * @param angle 角度（0~180度）
     * @return CCR值（1000~2000）
     */
    uint32_t angleToPulse(fp32 angle);
};

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/

/* Defines -------------------------------------------------------------------*/