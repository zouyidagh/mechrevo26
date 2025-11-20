/**
 ******************************************************************************
 * @file           : board_config.h
 * @brief          : board configuration, used to configure the peripheral used
 *                   开发板配置信息，用于配置使用的外设组件
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 GMaster
 * All rights reserved.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BOARD_CONFIG_H
#define __BOARD_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Exported macro ------------------------------------------------------------*/
/* System core ------------------------------*/
/* DMA */
/* GPIO */
/* NVIC */
/* RCC */
/* SYS */
/* Analog -----------------------------------*/
/* ADC */
/* DAC */
/* Timers -----------------------------------*/
/* RTC */
/* TIM */
/* Connectivity -----------------------------*/
/* CAN */
/* I2C */
/* SPI */
#define USE_SPI2
/* UART */
#define USE_USART3
/* USB */

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_CONFIG_H */
