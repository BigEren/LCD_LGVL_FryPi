#include "user_KeyTask.h"
#include "main.h"
#include "key.h"
#include "user_MessageQueue.h"

void KeyTask(void* argument)
{
    uint8_t keystr = 0;
    uint8_t Stopstr = 0;
    uint8_t IdleBreadstr = 0;

    for(;;)
    {
        switch (Key_GetValue())
        {
            case KEY_NONE:
                keystr = 0;
                break;

            case KEY_SHORT_PRESS:
                keystr = 1;
                osStatus_t ret = osMessageQueuePut(Key_MessageQueue, &keystr, 0, 0);

                printf("Key queue ret = %d\r\n", (int)ret);
                
                osMessageQueuePut(IdleBreak_MessageQueue, &IdleBreadstr, 0, 0);
                break;

            case KEY_LONG_PRESS:
                keystr = 2;
                ret = osMessageQueuePut(
                Key_MessageQueue, &keystr, 0, 0);
                break;

            default:
                break;
        }
        osDelay(8);
    }
}
