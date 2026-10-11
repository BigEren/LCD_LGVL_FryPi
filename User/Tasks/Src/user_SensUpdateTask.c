#include "user_SensUpdateTask.h"
#include "HWDataAccess.h"
#include "user_MessageQueue.h"
#include "mpu6050.h"
#include "LSM303.h"

#include "cmsis_os2.h"

uint8_t IdleBreakstr=0;


/**
 * @brief 手表水平状态检查任务
 * 
 * @param argument 任务参数
 */
void MPUCheckTask(void *argument)
{
    for (;;)
    {
        if (HWInterface.IMU.wrist_is_enabled)
        {
            if (MPU_isHorizontal())
            {
                HWInterface.IMU.wrist_state = WRIST_UP;
            }
            else
            {
                HWInterface.IMU.wrist_state = WRIST_DOWN;
            }
        }
        osDelay(50);
    }
}

void HealthDataUpdateTask(void *argument)
{
    if (false == HWInterface.MAX30102.ConnectionError)
    {
        HWInterface.MAX30102.Start();
    }
    for (;;)
    {
        if (false == HWInterface.MAX30102.ConnectionError)
        {
            if (false == HWInterface.MAX30102.ReadHealthData())
            {
                printf("HealthDataUpdateTask: ReadHealthData failed\n");
            }
        }
        osDelay(3000);
    }
}

void AHT20DataUpdateTask(void *argument)
{
    for (;;)
    {
        if (false == HWInterface.AHT20.ConnectionError)
        {
            if (false == HWInterface.AHT20.GetData())
            {
                printf("AHT20DataUpdateTask: GetData failed\n");
            }
        }
        osDelay(1000);
    }
}

void StepsDataUpdateTask(void *argument)
{
    for (;;)
    {
        if (false == HWInterface.IMU.ConnectionError)
        {
           HWInterface.IMU.GetSteps();
        }
        osDelay(500);
    }
}

void EcompassDataUpdateTask(void *argument)
{
    if (false == HWInterface.Ecompass.ConnectionError)
    {
        HWInterface.Ecompass.WakeUp();
    }
    for (;;)
    {
        // if (false == HWInterface.Ecompass.ConnectionError)
        // {
        //     /* 1. 唤醒 BNO055（从挂起模式回到 NDOF 模式） */
        //     if (false == HWInterface.Ecompass.WakeUp())
        //     {
        //         printf("EcompassDataUpdateTask: WakeUp failed\n");
        //         goto delay;
        //     }

        //     /* 2. 检查校准状态（磁力计必须是 3 才准） */
        //     if (BNO055_ReadCalibStat(&HWInterface.Ecompass.calib))
        //     {
        //         if (HWInterface.Ecompass.calib.mag < 3)
        //         {
        //             printf("Ecompass not calibrated: sys=%d gyro=%d accel=%d mag=%d\n",
        //                    HWInterface.Ecompass.calib.sys, 
        //                    HWInterface.Ecompass.calib.gyro, 
        //                    HWInterface.Ecompass.calib.accel, 
        //                    HWInterface.Ecompass.calib.mag);
        //             /* 可选：在 UI 上提示用户做"8"字校准 */
        //         }
        //     }
        //     else
        //     {
        //         printf("EcompassDataUpdateTask: ReadCalibStat failed\n");
        //         goto delay;
        //     }

        //     /* 3. 读取欧拉角（heading 就是方位角） */
        //     if (BNO055_ReadEuler(&HWInterface.Ecompass.euler))
        //     {
        //         /* 4. 应用安装偏移 + 磁偏角 */
        //         HWInterface.Ecompass.euler.heading += COMPASS_HEADING_OFFSET + MAGNETIC_DECLINATION;

        //         /* 归一化到 0~360 */
        //         while (HWInterface.Ecompass.euler.heading < 0)
        //         {
        //             HWInterface.Ecompass.euler.heading += 360.0f;
        //         }
        //         while (HWInterface.Ecompass.euler.heading >= 360.0f)
        //         {
        //             HWInterface.Ecompass.euler.heading -= 360.0f;
        //         }

        //         /* 5. 低通滤波*/
        //         static float heading_filtered = 0.0f;
        //         float diff = HWInterface.Ecompass.euler.heading - heading_filtered;
        //         if (diff > 180.0f) diff -= 360.0f;
        //         if (diff < -180.0f) diff += 360.0f;
        //         heading_filtered += diff * 0.2f; // 低通滤波系数 0.2
        //         if (heading_filtered >= 360.0f) heading_filtered -= 360.0f;
        //         if (heading_filtered < 0.0f) heading_filtered += 360.0f;

        //         HWInterface.Ecompass.euler.heading = heading_filtered;
        //     }
        // }

        if (false == HWInterface.Ecompass.ConnectionError)
        {
            int16_t Xa, Ya, Za, Xm, Ym, Zm;
            LSM303_ReadAcceleration(&Xa, &Ya, &Za);
            LSM303_ReadMagnetic(&Xm, &Ym, &Zm);
            float temp = Azimuth_Calculate(Xa, Ya, Za, Xm, Ym, Zm) + COMPASS_HEADING_OFFSET;
            if (temp < 0)
            {
                temp += 360.0f;
            }
            if (temp >=0 && temp <= 360)
            {
                HWInterface.Ecompass.euler.heading = temp;
            }
        }
        delay:
        osDelay(500);
    }
}

void GY39DataUpdateTask(void *argument)
{
    for (;;)
    {
        if (false == HWInterface.GY39.ConnectionError)
        {
            if (false == HWInterface.GY39.GetData())
            {
                printf("GY39DataUpdateTask: GetData failed\n");
            }
        }
        osDelay(1000);
    }
}

