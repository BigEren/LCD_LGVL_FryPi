#include "bno055.h"
#include "my_delay.h"

static iic_bus_my_t *BNO055_bus = &SensorBus;       // IIC总线结构体指针，指向SensorBus

/**
 * @brief 向 BNO055 写入一个字节
 */
static bool BNO055_WriteReg(uint8_t reg, uint8_t dat)
{
    return IIC_Write_One_Byte(BNO055_bus, 
                            BNO055ADDR, 
                            reg, 
                            dat) == SUCCESS;
}

/**
 * @brief 从 BNO055 读取一个或多个字节
 */
static bool BNO055_ReadRegs(uint8_t reg, uint8_t *buff, uint8_t length)
{
    return IIC_Read_Multi_Byte(BNO055_bus, 
                            BNO055ADDR, 
                            reg, 
                            length, 
                            buff) == SUCCESS;
}

/**
 * @brief 初始化 BNO055，进入 NDOF 模式
 */
bool BNO055_Init(void)
{
    uint8_t chip_id = 0x00;

    delay_ms_noOS(100);                                                  // 等待 BNO055 使能完成
    // 1. 读取芯片 ID
    if (!BNO055_ReadRegs(BNO055_CHIP_ID_REG, &chip_id, 1))
    {
        printf("ID read failed\n");
        return false;
    }

    // 2. 检查芯片 ID 是否正确
    if (chip_id != BNO055_CHIP_ID)
    {
        printf("ID check failed %d\n", chip_id);
        return false;
    }

    // 3. 软件复位
    BNO055_WriteReg(BNO055_SYS_TRIGGER_REG, 0x20);
    delay_ms_noOS(700);                                                  // 等待复位完成

    // 4. 设置单位为度 (可选, 默认就是度)
    BNO055_WriteReg(0x3B, 0x00);                                         // 设置为度
    delay_ms_noOS(20);                                                   // 等待单位切换完成

    // 5. 设置为 NDOF 模式
    BNO055_WriteReg(BNO055_OPR_MODE_REG, BNO055_MODE_NDOF);
    delay_ms_noOS(50);                                                   // 等待模式切换完成

    printf("BNO055 init done, NDOF mode\r\n");
    return true;
}

/**
 * @brief 读取欧拉角 (航向、横滚、俯仰)
 * @param euler 欧拉角结构体指针
 * @note 数据单位为度
 */
bool BNO055_ReadEuler(BNO055_Euler_t *euler)
{
    uint8_t buff[6];
    if (euler == NULL || !BNO055_ReadRegs(BNO055_EULER_H_LSB_REG, buff, 6))
    {
        printf("Euler read failed\n");
        return false;
    }

    // 小端格式，每个值 2 字节，实际角度 = raw / 16.0
    int16_t h_raw = (int16_t)(buff[1] << 8 | buff[0]);
    int16_t r_raw = (int16_t)(buff[3] << 8 | buff[2]);
    int16_t p_raw = (int16_t)(buff[5] << 8 | buff[4]);

    // 转换为度
    euler->heading  = (float)h_raw / 16.0f;
    euler->roll     = (float)r_raw / 16.0f;
    euler->pitch    = (float)p_raw / 16.0f;
    return true;
}

/**
 * @brief 读取校准状态
 */
bool BNO055_ReadCalibStat(BNO055_CalibStat_t *calib)
{
    uint8_t reg = 0;
    if (calib == NULL || !BNO055_ReadRegs(BNO055_CALIB_STAT_REG, &reg, 1))
    {
        printf("Calib read failed\n");
        return false;
    }

    calib->sys = (reg >> 6) & 0x03;   // 系统校准状态
    calib->gyro = (reg >> 4) & 0x03;  // 陀螺仪校准状态
    calib->accel = (reg >> 2) & 0x03; // 加速度计校准状态
    calib->mag = (reg >> 0) & 0x03;    // 磁力计校准状态
    return true;
}

bool BNO055_WakeUp(void)
{
    // 1. 唤醒
    if (!BNO055_WriteReg(BNO055_SYS_TRIGGER_REG, 0x00))
    {
        printf("BNO055 WakeUp failed\n");
        return false;
    }
    delay_ms(10); // 等待唤醒完成

    // 2. 设置为 NDOF 模式
    if (!BNO055_WriteReg(BNO055_OPR_MODE_REG, BNO055_MODE_CONFIG))
    {
        printf("BNO055 CONFIG failed\n");
        return false;
    }
    delay_ms(20); // 等待模式切换完成
    if (!BNO055_WriteReg(BNO055_OPR_MODE_REG, BNO055_MODE_NDOF))
    {
        printf("BNO055 NDOF failed\n");
        return false;
    }
    delay_ms(50); // 等待模式切换完成
    return true;
}

bool BNO055_Sleep(void)
{
    // 1. 进入休眠
    if (!BNO055_WriteReg(BNO055_SYS_TRIGGER_REG, 0x02))
    {
        printf("BNO055 Sleep failed\n");
        return false;
    }
    return true;
}