/************************************************
 *  @file     : app_alive.c
 *  @date     : January 2026
 *  @author   : CAO HIEU
 *  @version  : 1.0.0
 *-----------------------------------------------
 *  Description :
 *    [-]
 ************************************************/

#include "app_alive.h"
#include "peripheral/pio/plib_pio.h"
#include "M1_SysApp/xlog/xlog.h"
#include "M1_SysApp/dmesg/dmesg.h"
#include "stdio.h"


const osThreadAttr_t alive_attr = {
        .name = "LED_Alive",
        .stack_size = 256,
        .priority = configMAX_PRIORITIES - 4
};

void App_AliveTask(void *param)
{
    static uint8_t counter = 0;
    char buf[32];
    for(;;)
    {
        LED_PA23_Toggle();
        osThreadFeed();
        snprintf(buf, sizeof(buf), "Alive counter: %lu", counter++);
//        xlog("Hello World %d\r\n", counter++);
        Dmesg_Write(buf);
        osDelay(pdMS_TO_TICKS(2000));
    }
}