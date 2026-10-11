#include "user_ScrRenewTask.h"
#include "main.h"
#include "lvgl.h"
#include "user_MessageQueue.h"
#include "PageManager.h"
#include "ui_MenuPage.h"


void ScrRenewTask(void *argument)
{
    uint8_t keystr = 0;

    for(;;)
    {
        if (osMessageQueueGet(Key_MessageQueue, &keystr, NULL, 0) == osOK)
        {
            switch (keystr)
            {
                case 1:
                    // Handle short press
                    Page_Back();
                    if (Page_Get_NowPage()->page_obj == &ui_MenuPage)
                    printf("Short Press\n");
                    break;
                case 2:
                    // Handle long press
                    Page_Back_Bottom();
                    printf("Long Press\n");
                    break;
                default:
                    break;
            }
        }
        osDelay(10);
    }
}