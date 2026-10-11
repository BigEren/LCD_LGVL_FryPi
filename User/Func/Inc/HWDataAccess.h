/**
 * @addtogroup Hardware_Middle_Layer
 * @brief      Hardware Middle Layer, to access data from BSP and STM32 HAL Library.
 * @{
 * @file       HWDataAccess.h
 * @brief      Middleware for UI and APP layer to get hardware data.
 * @details    You can enable or disable hardware modules in this file.
 *
 *             添加此文件的目的：
 *             1. 方便 UI 移植：若将工程移植到 PC 仿真，可直接复制 MidFunc 与 UI 文件，
 *                并关闭 HW_USE_HARDWARE。
 *             2. 方便控制硬件：将对应的 HW_USE_xxx 设为 0，即可失能不需要的硬件。
 */

#ifndef __HW_DATA_ACCESS_H__
#define __HW_DATA_ACCESS_H__

#ifdef __cplusplus
extern "C" {
#endif

/*==================== Hardware Define ====================*/
/**
 * @brief 硬件总开关。
 * @note  若仅测试 UI、不接硬件，可关闭 HW_USE_HARDWARE，例如注释掉该宏。
 *        若使用硬件，则按需将下面的 HW_USE_xxx 设为 0/1。
 */
#define HW_USE_HARDWARE 1

#ifdef HW_USE_HARDWARE
    #define HW_USE_RTC        1  /**< RTC 实时时钟 */
    #define HW_USE_LCD        1  /**< LCD 显示屏 */
    #define HW_USE_AHT20      1  /**< AHT20 温湿度传感器 */
    #define HW_USE_MAX30102   1  /**< MAX30102 心率血氧传感器 */
    #define HW_USE_IMU        1  /**< MPU6050 IMU */
    #define HW_USE_GY39       1  /**< GY39 传感器 */
    #define HW_USE_Ecompass   1  /**< 电子罗盘 */
#endif

/* Ecompass 子开关 */
#if HW_USE_Ecompass
    #define HW_USE_BNO055      0  /**< 使用 BNO055 */
    #define HW_USE_LSM303DLH   1  /**< 使用 LSM303DLH */
#endif

/*==================== Include ====================*/
#include <stdint.h>
#include <stdbool.h>

#if HW_USE_RTC
    #include "rtc.h"
#endif

#if HW_USE_LCD
    #include "lcd_init.h"
#endif

#if HW_USE_AHT20
    #include "AHT20.h"
#endif

#if HW_USE_MAX30102
    #include "max30102.h"
    #include "health_sensor.h"
#endif

#if HW_USE_IMU
    #include "mpu6050.h"
    #include "inv_mpu.h"
    #include "inv_mpu_dmp_motion_driver.h"
#endif

#if HW_USE_GY39
    #include "gy39.h"
#endif

#if HW_USE_Ecompass
    #include "LSM303.h"
    #include "bno055.h"
#endif

#include "step_conter.h"
#include "health_sensor.h"
#include "wrist_detect.h"

/*==================== RTC ====================*/

/**
 * @brief RTC 日期时间数据
 */
typedef struct
{
    uint8_t WeekDay;  /**< 星期几 */
    uint8_t Month;    /**< 月份 */
    uint8_t Date;     /**< 日期 */
    uint8_t Year;     /**< 年份 */
    uint8_t Hour;     /**< 小时 */
    uint8_t Minute;   /**< 分钟 */
    uint8_t Second;   /**< 秒 */
} HW_DateTime_Interface_t;

/**
 * @brief RTC 接口
 */
typedef struct
{
    HW_DateTime_Interface_t nowdateTime;  /**< 当前日期时间 */

    void (*GetTimeDate)(HW_DateTime_Interface_t *nowdateTime);  /**< 获取当前日期时间 */
    void (*SetDate)(uint8_t year, uint8_t month, uint8_t date); /**< 设置日期 */
    void (*SetTime)(uint8_t hour, uint8_t minute, uint8_t second); /**< 设置时间 */
    uint8_t (*CalculateWeekDay)(uint8_t setyear, uint8_t setmonth, uint8_t setdate, uint8_t century); /**< 计算星期几 */
} HW_RTC_Interface_t;

/*==================== LCD ====================*/

/**
 * @brief LCD 接口
 */
typedef struct
{
    void (*SetLight)(uint8_t light);  /**< 设置 LCD 亮度 */
} HW_LCD_Interface_t;

/*==================== AHT20 ====================*/

/**
 * @brief AHT20 接口
 */
typedef struct
{
    bool ConnectionError;  /**< 连接错误标志 */
    AHT20_Data_t data;     /**< AHT20 数据 */

    bool (*Init)(void);    /**< 初始化 AHT20 */
    bool (*GetData)(void); /**< 获取 AHT20 数据 */
} HW_AHT20_Interface_t;

/*==================== MAX30102 ====================*/

/**
 * @brief MAX30102 接口
 */
typedef struct
{
    bool ConnectionError;          /**< 连接错误标志 */
    MAX30102_Data_t data;          /**< MAX30102 原始数据 */
    HealthData_t health_data;      /**< 健康数据 */

    bool (*Init)(void);            /**< 初始化 MAX30102 */
    bool (*GetI_RData)(void);      /**< 获取红外原始数据 */
    bool (*ReadHealthData)(void);  /**< 读取健康数据 */
    void (*Sleep)(void);           /**< 进入休眠 */
    void (*Start)(void);           /**< 启动测量 */
} HW_MAX30102_Interface_t;

/*==================== MPU6050 ====================*/

/**
 * @brief 手腕状态
 */
#define WRIST_UP   1  /**< 手腕抬起 */
#define WRIST_DOWN 0  /**< 手腕放下 */

/**
 * @brief MPU6050 / IMU 接口
 */
typedef struct
{
    bool ConnectionError;      /**< 连接错误标志 */
    uint16_t Steps;            /**< 步数 */
    uint8_t wrist_state;       /**< 手腕状态 */
    uint8_t wrist_is_enabled;  /**< 手腕检测使能状态 */

    bool (*Init)(void);        /**< 初始化 IMU */
    uint16_t (*GetSteps)(void);/**< 获取步数 */
    void (*WristEnable)(void); /**< 使能手腕检测 */
    void (*WristDisable)(void);/**< 失能手腕检测 */
    int (*SetSteps)(unsigned long count); /**< 设置步数 */
} HW_IMU_Interface_t;

/*==================== GY39 ====================*/

/**
 * @brief GY39 接口
 */
typedef struct
{
    bool ConnectionError;  /**< 连接错误标志 */
    GY39_Data_t data;      /**< GY39 数据 */

    bool (*Init)(void);    /**< 初始化 GY39 */
    bool (*GetData)(void); /**< 获取 GY39 数据 */
} HW_GY39_Interface_t;

/*==================== BNO055 / Ecompass ====================*/

/**
 * @brief 电子罗盘接口
 */
typedef struct
{
    bool ConnectionError;          /**< 连接错误标志 */
    BNO055_Euler_t euler;          /**< 欧拉角 */
    BNO055_CalibStat_t calib;      /**< 校准状态 */

    bool (*Init)(void);            /**< 初始化电子罗盘 */
    bool (*WakeUp)(void);          /**< 唤醒 */
    void (*Sleep)(void);           /**< 休眠 */
} HW_Ecompass_Interface_t;

/*==================== Hardware Interface ====================*/

/**
 * @brief 全局硬件接口集合
 */
typedef struct
{
    HW_RTC_Interface_t RealTimeClock;      /**< RTC 接口 */
    HW_LCD_Interface_t LCD;                /**< LCD 接口 */
    HW_AHT20_Interface_t AHT20;            /**< AHT20 接口 */
    HW_MAX30102_Interface_t MAX30102;      /**< MAX30102 接口 */
    HW_IMU_Interface_t IMU;                /**< IMU 接口 */
    HW_GY39_Interface_t GY39;              /**< GY39 接口 */
    HW_Ecompass_Interface_t Ecompass;      /**< 电子罗盘接口 */
} HW_Interface_t;

/* 全局唯一的硬件接口实例 */
extern HW_Interface_t HWInterface;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __HW_DATA_ACCESS_H__ */

/** @} */ /* Hardware_Middle_Layer */