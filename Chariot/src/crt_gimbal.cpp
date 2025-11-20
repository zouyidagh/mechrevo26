/**
 ******************************************************************************
 * @file           : crt_gimbal.cpp
 * @brief          : 云台控制
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 GMaster
 * All rights reserved.
 *
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "crt_gimbal.hpp"
#include <algorithm>
#include <cmath>

/* Typedef -------------------------------------------------------------------*/

/* Define --------------------------------------------------------------------*/

/* Macro ---------------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/* User code -----------------------------------------------------------------*/

/******************************************************************************
 *                            Gimbal类实现
 ******************************************************************************/

/**
 * @brief 构造函数
 * @param htimYaw Yaw舵机定时器句柄
 * @param channelYaw Yaw舵机定时器通道
 * @param htimPitch Pitch舵机定时器句柄
 * @param channelPitch Pitch舵机定时器通道
 * @param shootMotorPort 发射马达GPIO端口
 * @param shootMotorPin 发射马达GPIO引脚
 */
Gimbal::Gimbal(TIM_HandleTypeDef *htimYaw, uint32_t channelYaw,
               TIM_HandleTypeDef *htimPitch, uint32_t channelPitch,
               GPIO_TypeDef *shootMotorPort, uint16_t shootMotorPin)
    : m_htimYaw(htimYaw),
      m_channelYaw(channelYaw),
      m_htimPitch(htimPitch),
      m_channelPitch(channelPitch),
      m_shootMotorPort(shootMotorPort),
      m_shootMotorPin(shootMotorPin),
      m_yawAngle(m_yawCenter),
      m_pitchAngle(m_pitchCenter)
{
}

/**
 * @brief 初始化云台
 */
void Gimbal::init(void)
{
    // 启动PWM输出
    HAL_TIM_PWM_Start(m_htimYaw, m_channelYaw);
    HAL_TIM_PWM_Start(m_htimPitch, m_channelPitch);
    
    // 复位到中位
    reset();
    
    // 关闭发射马达
    setShootMotor(false);
}

/**
 * @brief 将角度转换为PWM的CCR值
 * @param angle 角度（0~180度）
 * @return CCR值（1000~2000）
 * 
 * 转换公式：
 * CCR = pulseMin + (angle / 180) * (pulseMax - pulseMin)
 */
uint32_t Gimbal::angleToPulse(fp32 angle)
{
    // 限制角度范围
    angle = std::clamp(angle, m_minAngle, m_maxAngle);
    
    // 线性插值
    fp32 pulse = m_pulseMin + (angle / 180.0f) * (m_pulseMax - m_pulseMin);
    
    return static_cast<uint32_t>(pulse);
}

/**
 * @brief 设置Yaw舵机角度
 * @param angle 目标角度（0~180度）
 */
void Gimbal::setYawAngle(fp32 angle)
{
    // 限制角度范围
    m_yawAngle = std::clamp(angle, m_minAngle, m_maxAngle);
    
    // 转换为PWM脉宽并设置
    uint32_t pulse = angleToPulse(m_yawAngle);
    __HAL_TIM_SET_COMPARE(m_htimYaw, m_channelYaw, pulse);
}

/**
 * @brief 设置Pitch舵机角度
 * @param angle 目标角度（0~180度）
 */
void Gimbal::setPitchAngle(fp32 angle)
{
    // 限制角度范围
    m_pitchAngle = std::clamp(angle, m_minAngle, m_maxAngle);
    
    // 转换为PWM脉宽并设置
    uint32_t pulse = angleToPulse(m_pitchAngle);
    __HAL_TIM_SET_COMPARE(m_htimPitch, m_channelPitch, pulse);
}

/**
 * @brief 设置发射马达状态
 * @param enable true:启动，false:停止
 */
void Gimbal::setShootMotor(bool enable)
{
    if (enable)
    {
        HAL_GPIO_WritePin(m_shootMotorPort, m_shootMotorPin, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(m_shootMotorPort, m_shootMotorPin, GPIO_PIN_RESET);
    }
}

/**
 * @brief 云台复位到中位
 */
void Gimbal::reset(void)
{
    setYawAngle(m_yawCenter);
    setPitchAngle(m_pitchCenter);
}

/**
 * @brief 云台控制循环（使用遥控器控制）
 * 
 * 右摇杆控制云台朝向：
 * - 右摇杆X轴：控制Yaw角度（左右转动）
 *   - 摇杆向左 (-128) -> 减小Yaw角度
 *   - 摇杆向右 (+127) -> 增大Yaw角度
 * - 右摇杆Y轴：控制Pitch角度（上下转动）
 *   - 摇杆向上 (+127) -> 增大Pitch角度
 *   - 摇杆向下 (-128) -> 减小Pitch角度
 * - RIGHT_1按键：控制发射马达
 */
void Gimbal::gimbalControlLoop(void)
{
    // 读取右摇杆数据
    int16_t joystickX = rc_ps2.RIGHT_X;  // 左右方向 (-128 ~ 127)
    int16_t joystickY = rc_ps2.RIGHT_Y;  // 前后方向 (-128 ~ 127)

    // 摇杆死区处理（忽略小于10的值）
    constexpr int16_t deadzone = 10;
    if (std::abs(joystickX) < deadzone) joystickX = 0;
    if (std::abs(joystickY) < deadzone) joystickY = 0;
    
    // 将摇杆值转换为角度增量
    // 摇杆最大值127对应每次调整m_sensitivity度
    constexpr fp32 joystickMax = 127.0f;
    fp32 yawDelta = (static_cast<fp32>(joystickX) / joystickMax) * m_sensitivity;
    fp32 pitchDelta = (static_cast<fp32>(joystickY) / joystickMax) * m_sensitivity;
    
    // 更新角度（累加方式）
    fp32 newYaw = m_yawAngle + yawDelta;
    fp32 newPitch = m_pitchAngle + pitchDelta;
    
    // 设置新角度
    setYawAngle(newYaw);
    setPitchAngle(newPitch);
    
    // 控制发射马达
    bool shootEnable = (rc_ps2.RIGHT_1 == 1);
    setShootMotor(shootEnable);
}
