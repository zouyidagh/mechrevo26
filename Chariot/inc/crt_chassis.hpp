/**
 ******************************************************************************
 * @file           : cpp_template.h
 * @brief          : header file for crt_chassis.cpp
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
#include "algorithm"
#include "alg_pid.hpp"
#include "para_chassis.hpp"
#include "dvc_motor.hpp"
#include "ps2.h"

/* Exported types ------------------------------------------------------------*/

/**
 * @brief 麦克纳姆轮底盘控制类
 * 
 * 麦克纳姆轮布局（俯视图）：
 *    前方
 *   ↑ x
 * ← y
 * 
 *  左前(LF)    右前(RF)
 *     \  /      /  \
 *      \/      /    \
 *      /\      \    /
 *     /  \      \  /
 *  左后(LR)    右后(RR)
 * 
 * 轮子朝向：
 * - 左前(LF)：斜向右前 /
 * - 右前(RF)：斜向左前 \
 * - 左后(LR)：斜向左前 \
 * - 右后(RR)：斜向右前 /
 */
class Chassis
{
public:
    using Vector3f  = GSRLMath::Vector3f;
    using Matrix33f = GSRLMath::Matrix33f;

private:
    // 机械参数
    constexpr static fp32 m_wheelBase = 0.20f;      // 轴距(前后轮间距, 单位:m)
    constexpr static fp32 m_wheelTrack = 0.20f;     // 轮距(左右轮间距, 单位:m)
    constexpr static fp32 m_wheelRadius = WHEEL_RADIUS_M;  // 轮子半径(单位:m)
    
    // 四个电机(左前, 右前, 左后, 右后)
    Motor *m_motorLF;  // 左前轮电机
    Motor *m_motorRF;  // 右前轮电机
    Motor *m_motorLR;  // 左后轮电机
    Motor *m_motorRR;  // 右后轮电机
    
    // 底盘控制量
    fp32 m_targetVx;      // 目标x方向速度 (前进方向, m/s)
    fp32 m_targetVy;      // 目标y方向速度 (左侧方向, m/s)
    fp32 m_targetOmega;   // 目标角速度 (逆时针为正, rad/s)
    
    // 轮子转速限制 (单位: m/s)
    constexpr static fp32 m_maxWheelSpeed = 1.0f;

public:
    /**
     * @brief 构造函数
     * @param motorLF 左前轮电机指针
     * @param motorRF 右前轮电机指针
     * @param motorLR 左后轮电机指针
     * @param motorRR 右后轮电机指针
     * @param remoteControl 遥控器指针
     */
    Chassis(Motor *motorLF, Motor *motorRF, Motor *motorLR, Motor *motorRR);

    /**
     * @brief 初始化底盘
     */
    void init(void);

    /**
     * @brief 设置目标速度（底盘坐标系）
     * @param vx 前进速度 (m/s, 正值向前)
     * @param vy 横移速度 (m/s, 正值向左)
     * @param omega 旋转角速度 (rad/s, 正值逆时针)
     */
    void setVelocity(fp32 vx, fp32 vy, fp32 omega);

    /**
     * @brief 停止底盘
     */
    void stop(void);

    /**
     * @brief 刹车（所有电机短路制动）
     */
    void brake(void);

    /**
     * @brief 底盘控制循环（使用遥控器控制）
     * 读取遥控器左摇杆数据，转换为底盘速度并控制电机
     */
    void chassisControlLoop(void);

    /**
     * @brief 获取目标x方向速度
     */
    fp32 getTargetVx(void) const { return m_targetVx; }

    /**
     * @brief 获取目标y方向速度
     */
    fp32 getTargetVy(void) const { return m_targetVy; }

    /**
     * @brief 获取目标角速度
     */
    fp32 getTargetOmega(void) const { return m_targetOmega; }

private:
    /**
     * @brief 麦克纳姆轮运动学解算
     * @param vx 前进速度 (m/s)
     * @param vy 横移速度 (m/s)
     * @param omega 旋转角速度 (rad/s)
     * @param wheelSpeed 输出四个轮子的线速度数组 [LF, RF, LR, RR] (m/s)
     */
    void mecanumKinematics(fp32 vx, fp32 vy, fp32 omega, fp32 wheelSpeed[4]);

    /**
     * @brief 将轮子线速度转换为电机PWM值
     * @param linearSpeed 轮子线速度 (m/s)
     * @return PWM值 (-1000 ~ 1000)
     */
    int16_t linearSpeedToPwm(fp32 linearSpeed);
};

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/

/* Defines -------------------------------------------------------------------*/
