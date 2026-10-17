/* Private includes -----------------------------------------------------------*/
// includes
#include "user_TasksInit.h"

// sys includes
#include "sys.h"
#include "stdio.h"

// bsp includes
#include "key.h"

// gui includes
#include "lvgl.h"

// task includes
#include "user_HardwareInitTask.h"
#include "user_LVGLTask.h"
#include "user_SensUpdateTask.h"
#include "user_MessageQueue.h"
#include "user_PrintSensDataTask.h"
#include "user_RunModeTask.h"
#include "user_ScrRenewTask.h"
#include "user_KeyTask.h"



/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
osTimerId_t IdleTimerHandle = NULL;


/* Timers --------------------------------------------------------------------*/


/* Tasks ---------------------------------------------------------------------*/
// HardwareInitTask Init
osThreadId_t user_HardwareInitTaskHandle = NULL;
const osThreadAttr_t user_HardwareInitTask_attributes = {
    .name = "user_HardwareInitTask",
    .stack_size = 128 * 10,
    .priority = (osPriority_t)osPriorityHigh,
};

// LVGLTask Init
osThreadId_t user_LVGLTaskHandle = NULL;
const osThreadAttr_t user_LVGLTask_attributes = {
    .name = "user_LVGLTask",
    .stack_size = 128 * 24,
    .priority = (osPriority_t)osPriorityNormal,
};

// // AHT20Task Init
// osThreadId_t user_AHT20TaskHandle = NULL;
// const osThreadAttr_t user_AHT20Task_attributes = {
//     .name = "user_AHT20Task",
//     .stack_size = 128 * 10,
//     .priority = (osPriority_t)osPriorityLow1,
// };

// // MPU6050Task Init
// osThreadId_t user_MPU6050TaskHandle = NULL;
// const osThreadAttr_t user_MPU6050Task_attributes = {
//     .name = "user_MPU6050Task",
//     .stack_size = 128 * 10,
//     .priority = (osPriority_t)osPriorityLow1,
// };

// // MAX30102Task Init
// osThreadId_t user_MAX30102TaskHandle = NULL;
// const osThreadAttr_t user_MAX30102Task_attributes = {
//     .name = "user_MAX30102Task",
//     .stack_size = 128 * 10,
//     .priority = (osPriority_t)osPriorityLow1,
// };

// MPUCheckTask Init
osThreadId_t MPUCheckTaskHandle = NULL;
const osThreadAttr_t MPUCheckTask_attributes = {
    .name = "MPUCheckTask",
    .stack_size = 128 * 10,
    .priority = (osPriority_t)osPriorityLow1,
};

// HealthDataUpdateTask Init
osThreadId_t HealthDataUpdateTaskHandle = NULL;
const osThreadAttr_t HealthDataUpdateTask_attributes = {
    .name = "HealthDataUpdateTask",
    .stack_size = 128 * 6,
    .priority = (osPriority_t)osPriorityLow1,
};


// StopEnterTask Init
osThreadId_t StopEnterTaskHandle = NULL;
const osThreadAttr_t StopEnterTask_attributes = {
    .name = "StopEnterTask",
    .stack_size = 128 * 16,
    .priority = (osPriority_t)osPriorityLow3,
};


// IdleEnterTask Init
osThreadId_t IdleEnterTaskHandle = NULL;
const osThreadAttr_t IdleEnterTask_attributes = {
    .name = "IdleEnterTask",
    .stack_size = 128 * 3,
    .priority = (osPriority_t)osPriorityLow1,
};

// KeyTask Init
osThreadId_t KeyTaskHandle = NULL;
const osThreadAttr_t KeyTask_attributes = {
    .name = "KeyTask",
    .stack_size = 128 * 3,
    .priority = (osPriority_t)osPriorityLow1,
};

// ScrRenewTask Init
osThreadId_t ScrRenewTaskHandle = NULL;
const osThreadAttr_t ScrRenewTask_attributes = {
    .name = "ScrRenewTask",
    .stack_size = 128 * 10,
    .priority = (osPriority_t)osPriorityLow1,
};

// SensorDataUpdateTask Init
osThreadId_t SensorDataUpdateTaskHandle = NULL;
const osThreadAttr_t SensorDataUpdateTask_attributes = {
    .name = "SensorDataUpdateTask",
    .stack_size = 128 * 10,
    .priority = (osPriority_t)osPriorityLow1,
};

/* Message queues ------------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void user_Tasks_Init(void)
{
    /* add mutexes, ... */

    /* add semaphores, ... */

    /* start timers, add new ones, ... */
    IdleTimerHandle = osTimerNew(IdleTimerCallback, osTimerPeriodic, NULL, NULL);
	osTimerStart(IdleTimerHandle, 100);  //100ms

    /* add queues, ... */
    User_MessageQueueInit();
	/* add threads, ... */
    user_HardwareInitTaskHandle = osThreadNew(user_HardwareInitTask, NULL, &user_HardwareInitTask_attributes);
    user_LVGLTaskHandle = osThreadNew(user_LVGLTask, NULL, &user_LVGLTask_attributes);
    HealthDataUpdateTaskHandle = osThreadNew(HealthDataUpdateTask, NULL, &HealthDataUpdateTask_attributes);
    MPUCheckTaskHandle = osThreadNew(MPUCheckTask, NULL, &MPUCheckTask_attributes);
    StopEnterTaskHandle = osThreadNew(StopEnterTask, NULL, &StopEnterTask_attributes);
    IdleEnterTaskHandle = osThreadNew(IdleEnterTask, NULL, &IdleEnterTask_attributes);  
    SensorDataUpdateTaskHandle = osThreadNew(SensorDataUpdateTask, NULL, &SensorDataUpdateTask_attributes);
    KeyTaskHandle = osThreadNew(KeyTask, NULL, &KeyTask_attributes);
    ScrRenewTaskHandle = osThreadNew(ScrRenewTask, NULL, &ScrRenewTask_attributes);

    /* add events, ... */

	/* add  others ... */
}
