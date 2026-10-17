#include "user_ScrRenewTask.h"
#include "main.h"
#include "lvgl.h"
#include "user_MessageQueue.h"
#include "PageManager.h"
#include "ui_MenuPage.h"
#include "ui_HomePage.h"
#include "cmsis_os2.h"
#include "HWDataAccess.h"

extern osThreadId_t ScrRenewTaskHandle;


void ScrRenewTask(void *argument)
{
    uint8_t keystr = 0;

    for(;;)
    {
        if (osMessageQueueGet(Key_MessageQueue, &keystr, NULL, 0) == osOK)
        {
            // uint32_t stack_rest = osThreadGetStackSpace(ScrRenewTaskHandle);
            // printf("ScrRenewTask: stack_rest = %d\r\n", stack_rest);
            switch (keystr)
            {
                case 1:
                    // Handle short press
                    Page_Back();
                    Page_t *nowPage = Page_Get_NowPage();
                    if (nowPage->page_obj == &ui_MenuPage)
                    {
                        HWInterface.Ecompass.Sleep();
                        HWInterface.MAX30102.Sleep();
                    }
                    break;
                case 2:
                    // Handle long press
                    Page_Back_Bottom();
                    if (nowPage->page_obj == &ui_HomePage)
                    {
                        HWInterface.Ecompass.Sleep();
                        HWInterface.MAX30102.Sleep();
                    }
                    break;
                default:
                    break;
            }
        }

        osDelay(10);
    }
}