#include "user_SensUpdateTask.h"
#include "HWDataAccess.h"
#include "user_MessageQueue.h"
#include "mpu6050.h"
#include "LSM303.h"
#include "PageManager.h"
#include "ui_helpers.h"
#include "ui_HRPage.h"
#include "ui_SPO2Page.h"

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
                Page_t *current_page = Page_Get_NowPage();
                if (current_page->page_obj == NULL)
                {
                    goto delay;
                }
                if (WRIST_UP == HWInterface.IMU.wrist_state)
                {
                    HWInterface.IMU.wrist_state = WRIST_DOWN;
                    if (current_page->page_obj == &ui_HomePage ||
                        current_page->page_obj == &ui_MenuPage)
                    {
                        // 处于这几个页面放腕sleep
                        uint8_t Stopstr;
                        //osMessageQueuePut(Stop_MessageQueue, &Stopstr, 0, 1);
                    }
                }
                HWInterface.IMU.wrist_state = WRIST_DOWN;
            }
        }
        delay:
        osDelay(300);
    }
}

void HealthDataUpdateTask(void *argument)
{
    for (;;)
    {
        Page_t *current_page = Page_Get_NowPage();
        if (current_page->page_obj == NULL)
        {
            goto delay;
        }
        if (false == HWInterface.MAX30102.ConnectionError && (current_page->page_obj == &ui_HRPage || current_page->page_obj == &ui_SPO2Page))
        {
            if (false == HWInterface.MAX30102.ReadHealthData())
            {
                printf("HealthDataUpdateTask: ReadHealthData failed\n");
            }
        }
        delay:
        osDelay(3000);
    }
}

void SensorDataUpdateTask(void *argument)
{
    uint8_t IdleBreakstr = 0;
    for (;;)
    {
        uint8_t HomeUpdateStr;
        // 主页面更新
        if (osMessageQueueGet(HomeUpdata_MessageQueue, &HomeUpdateStr, NULL, 0) == osOK)
        {
            // //bat
			// uint8_t value_strbuf[5];

			// HWInterface.Power.power_remain = HWInterface.Power.BatCalculate();
			// if(HWInterface.Power.power_remain>0 && HWInterface.Power.power_remain<=100)
			// {}
			// else
			// {HWInterface.Power.power_remain = 0;}

            // steps
            if (false == HWInterface.IMU.ConnectionError)
            {
                HWInterface.IMU.Steps = HWInterface.IMU.GetSteps();
            }

            // temperature humidity
            if (false == HWInterface.GY39.ConnectionError)
            {
                HWInterface.GY39.GetData();
            }
            //send data save message queue
			// uint8_t Datastr = 3;
			// osMessageQueuePut(DataSave_MessageQueue, &Datastr, 0, 1);
        }
        // 当前页面更新
        Page_t *current_page = Page_Get_NowPage();
        if (current_page->page_obj == NULL)
        {
            goto delay;
        }

        // 环境温度湿度
        if (current_page->page_obj == &ui_EnvPage)
        {
            if (false == HWInterface.GY39.ConnectionError)
            {
                HWInterface.GY39.GetData();                 // 获取环境温度湿度
            }
        }
        // 电子罗盘
        if (current_page->page_obj == &ui_CompassPage)
        {
            osMessageQueuePut(IdleBreak_MessageQueue, &IdleBreakstr, 0, 1);

            HWInterface.Ecompass.WakeUp();

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
                    HWInterface.Ecompass.direction = temp;
                }
            }
            if (false == HWInterface.GY39.ConnectionError)
            {
                HWInterface.GY39.GetData();                 // 获取海拔
            }
        }
        delay:
        osDelay(500);
    }
}

