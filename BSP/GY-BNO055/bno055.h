#ifndef __BNO055_H__
#define __BNO055_H__

#include "main.h"
#include "stdbool.h"
#include "my_iic_hal.h"

/* ======== 罗盘校准参数 ======== */
// 安装偏移（需要实际测试后填写）
#define COMPASS_HEADING_OFFSET  0.0f  

// 磁偏角（长沙磁偏角是 4°10' W）
#define MAGNETIC_DECLINATION    (-4.17f) 

// BNO055寄存器定义

// BNO055 默认 7位地址为 0x28，若 ADR 引脚接地，则地址为 0x28，若 ADR 引脚接 VDD，则地址为 0x29
#define BNO055ADDR              0x28

// 操作模式
#define BNO055_MODE_CONFIG      0x00
// 九轴绝对方向融合模式
#define BNO055_MODE_NDOF        0x0C

// BNO055寄存器地址
#define BNO055_CHIP_ID_REG       0x00
#define BNO055_OPR_MODE_REG      0x3D
#define BNO055_EULER_H_LSB_REG   0x1A
#define BNO055_CALIB_STAT_REG    0x35
#define BNO055_SYS_TRIGGER_REG   0x3F

// 芯片 ID
#define BNO055_CHIP_ID           0xA0


// 数据结构体定义
typedef struct
{
    float heading; // 航向角 (0°~360°)
    float roll;    // 横滚角 (-180°~180°)
    float pitch;   // 俯仰角 (-180°~180°)
} BNO055_Euler_t;

typedef struct
{
    uint8_t sys;   // 系统校准状态 (0~3)
    uint8_t gyro;  // 陀螺仪校准状态 (0~3)
    uint8_t accel; // 加速度计校准状态 (0~3)
    uint8_t mag;   // 磁力计校准状态 (0~3)
} BNO055_CalibStat_t;

// 函数声明
bool BNO055_Init(void);
bool BNO055_ReadEuler(BNO055_Euler_t *euler);
bool BNO055_ReadCalibStat(BNO055_CalibStat_t *calib);
bool BNO055_WakeUp(void);
bool BNO055_Sleep(void);


#endif /* __BNO055_H__ */
