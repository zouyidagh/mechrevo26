/**
 ******************************************************************************
 * @file           : crt_chassis.cpp
 * @brief          : 底盘控制
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 GMaster
 * All rights reserved.
 *
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "crt_chassis.hpp"
#include "math_const.h"
#include "alg_general.hpp"
#include "ps2.h"
#include <algorithm>
#include <cmath>

/* Typedef -------------------------------------------------------------------*/

/* Define --------------------------------------------------------------------*/

/* Macro ---------------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/* User code -----------------------------------------------------------------*/

/******************************************************************************
 *                            Chassis类实现
 ******************************************************************************/

/**
 * @brief 构造函数
 * @param motorLF 左前轮电机指针
 * @param motorRF 右前轮电机指针
 * @param motorLR 左后轮电机指针
 * @param motorRR 右后轮电机指针
 * @param remoteControl 遥控器指针
 */
Chassis::Chassis(Motor *motorLF, Motor *motorRF, Motor *motorLR, Motor *motorRR)
    : m_motorLF(motorLF),
      m_motorRF(motorRF),
      m_motorLR(motorLR),
      m_motorRR(motorRR),
      m_targetVx(0.0f),
      m_targetVy(0.0f),
      m_targetOmega(0.0f)
{
}

/**
 * @brief 初始化底盘
 */
void Chassis::init(void)
{
    // 初始化所有电机
    if (m_motorLF != nullptr) m_motorLF->init();
    if (m_motorRF != nullptr) m_motorRF->init();
    if (m_motorLR != nullptr) m_motorLR->init();
    if (m_motorRR != nullptr) m_motorRR->init();
    
    // 初始化控制量
    m_targetVx = 0.0f;
    m_targetVy = 0.0f;
    m_targetOmega = 0.0f;
}

/**
 * @brief 麦克纳姆轮运动学解算
 * @param vx 前进速度 (m/s, 正值向前)
 * @param vy 横移速度 (m/s, 正值向左)
 * @param omega 旋转角速度 (rad/s, 正值逆时针)
 * @param wheelSpeed 输出四个轮子的线速度数组 [LF, RF, LR, RR] (m/s)
 * 
 * 麦克纳姆轮运动学公式：
 * v_LF = vx - vy - omega * (wheelBase/2 + wheelTrack/2)
 * v_RF = vx + vy + omega * (wheelBase/2 + wheelTrack/2)
 * v_LR = vx + vy - omega * (wheelBase/2 + wheelTrack/2)
 * v_RR = vx - vy + omega * (wheelBase/2 + wheelTrack/2)
 */
void Chassis::mecanumKinematics(fp32 vx, fp32 vy, fp32 omega, fp32 wheelSpeed[4])
{
    // 计算底盘半径（从中心到轮子的距离）
    fp32 L = (m_wheelBase + m_wheelTrack) / 2.0f;
    
    // 麦克纳姆轮运动学正解
    wheelSpeed[0] = vx - vy - omega * L;  // 左前轮 (LF)
    wheelSpeed[1] = vx + vy + omega * L;  // 右前轮 (RF)
    wheelSpeed[2] = vx + vy - omega * L;  // 左后轮 (LR)
    wheelSpeed[3] = vx - vy + omega * L;  // 右后轮 (RR)
    
    // 找到最大轮速
    fp32 maxSpeed = 0.0f;
    for (int i = 0; i < 4; i++)
    {
        fp32 absSpeed = std::fabs(wheelSpeed[i]);
        if (absSpeed > maxSpeed)
        {
            maxSpeed = absSpeed;
        }
    }
    
    // 如果最大轮速超过限制，按比例缩放所有轮速
    if (maxSpeed > m_maxWheelSpeed)
    {
        fp32 scale = m_maxWheelSpeed / maxSpeed;
        for (int i = 0; i < 4; i++)
        {
            wheelSpeed[i] *= scale;
        }
    }
}

/**
 * @brief 将轮子线速度转换为电机PWM值
 * @param linearSpeed 轮子线速度 (m/s)
 * @return PWM值 (-1000 ~ 1000)
 * 
 * 转换关系：
 * PWM = (linearSpeed / maxWheelSpeed) * 1000
 */
int16_t Chassis::linearSpeedToPwm(fp32 linearSpeed)
{
    // 线速度转换为PWM值
    fp32 pwmFloat = (linearSpeed / m_maxWheelSpeed) * 1000.0f;
    
    // 限幅并转换为整数
    int16_t pwm = static_cast<int16_t>(std::clamp(pwmFloat, -1000.0f, 1000.0f));
    
    return pwm;
}

/**
 * @brief 设置目标速度（底盘坐标系）
 * @param vx 前进速度 (m/s, 正值向前)
 * @param vy 横移速度 (m/s, 正值向左)
 * @param omega 旋转角速度 (rad/s, 正值逆时针)
 */
void Chassis::setVelocity(fp32 vx, fp32 vy, fp32 omega)
{
    // 保存目标速度
    m_targetVx = vx;
    m_targetVy = vy;
    m_targetOmega = omega;
    
    // 计算四个轮子的线速度
    fp32 wheelSpeed[4] = {0};
    mecanumKinematics(vx, vy, omega, wheelSpeed);
    
    // 转换为PWM值并发送给电机
    int16_t pwmLF = linearSpeedToPwm(wheelSpeed[0]);
    int16_t pwmRF = linearSpeedToPwm(wheelSpeed[1]);
    int16_t pwmLR = linearSpeedToPwm(wheelSpeed[2]);
    int16_t pwmRR = linearSpeedToPwm(wheelSpeed[3]);
    
    // 设置电机速度
    if (m_motorLF != nullptr) m_motorLF->setSpeed(pwmLF);
    if (m_motorRF != nullptr) m_motorRF->setSpeed(pwmRF);
    if (m_motorLR != nullptr) m_motorLR->setSpeed(pwmLR);
    if (m_motorRR != nullptr) m_motorRR->setSpeed(pwmRR);
}

/**
 * @brief 停止底盘
 */
void Chassis::stop(void)
{
    m_targetVx = 0.0f;
    m_targetVy = 0.0f;
    m_targetOmega = 0.0f;
    
    if (m_motorLF != nullptr) m_motorLF->stop();
    if (m_motorRF != nullptr) m_motorRF->stop();
    if (m_motorLR != nullptr) m_motorLR->stop();
    if (m_motorRR != nullptr) m_motorRR->stop();
}

/**
 * @brief 刹车（所有电机短路制动）
 */
void Chassis::brake(void)
{
    m_targetVx = 0.0f;
    m_targetVy = 0.0f;
    m_targetOmega = 0.0f;
    
    if (m_motorLF != nullptr) m_motorLF->brake();
    if (m_motorRF != nullptr) m_motorRF->brake();
    if (m_motorLR != nullptr) m_motorLR->brake();
    if (m_motorRR != nullptr) m_motorRR->brake();
}

/**
 * @brief 底盘控制循环（使用遥控器控制）
 * 
 * 读取遥控器左摇杆数据，转换为底盘速度并控制电机
 * 
 * 摇杆映射关系：
 * - 左摇杆Y轴：控制前后移动 (vx)
 *   - 摇杆向上 (+127) -> 向前移动 (+vx)
 *   - 摇杆向下 (-128) -> 向后移动 (-vx)
 * - 左摇杆X轴：控制左右移动 (vy)
 *   - 摇杆向左 (-128) -> 向左移动 (+vy)
 *   - 摇杆向右 (+127) -> 向右移动 (-vy)
 * 
 * 速度映射：
 * - 摇杆值范围：-128 ~ 127
 * - 目标速度范围：-1.0 ~ 1.0 m/s (可调整)
 */
void Chassis::chassisControlLoop(void)
{
    // 读取左摇杆数据
    int16_t joystickX = rc_ps2.LEFT_X;  // 左右方向 (-128 ~ 127)
    int16_t joystickY = rc_ps2.LEFT_Y;  // 前后方向 (-128 ~ 127)

    // 摇杆死区处理（忽略小于10的值）
    constexpr int16_t deadzone = 10;
    if (std::abs(joystickX) < deadzone) joystickX = 0;
    if (std::abs(joystickY) < deadzone) joystickY = 0;
    
    // 将摇杆值转换为速度 (m/s)
    // 最大速度设为 1.0 m/s
    constexpr fp32 maxSpeed = 1.0f;
    constexpr fp32 joystickMax = 127.0f;
    
    // 注意：PS2摇杆Y轴向上为正，向下为负
    // X轴向右为正，向左为负
    fp32 vx = (static_cast<fp32>(joystickY) / joystickMax) * maxSpeed;   // 前后速度
    fp32 vy = (static_cast<fp32>(joystickX) / joystickMax) * maxSpeed;  // 左右速度（取反，使左为正）

    // 角速度控制
    fp32 omega = 0.0f;
    if (rc_ps2.LEFT_2 == 1)
        omega = -10.0f; // 左肩键逆时针旋转
    else if (rc_ps2.RIGHT_2 == 1)
        omega = 10.0f; // 右肩键顺时针旋转

    // 设置底盘速度
    setVelocity(vx, vy, omega);
}

