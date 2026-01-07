/*
 * app_root.c
 *
 *  Created on: Feb 21, 2025
 *      Author: CAO HIEU
 */

#include "os.h"
#include "stdio.h"

#include "app_root.h"
#include "M0_App/AppOS/App_4_Alive/app_alive.h"
#include "M0_App/AppOS/App_3_CLI/app_cli.h"
#include "M0_App/AppOS/App_4_Logging/app_logging.h"
#include "M0_App/AppOS/App_3_Network/app_network.h"
#include "M0_App/AppOS/App_3_USB/app_usb.h"

#include "M1_SysApp/xlog/xlog.h"
#include "M1_SysApp/dmesg/dmesg.h"

#define ALIVETASK_TIMEOUT_MS    4000
#define CLITASK_TIMEOUT_MS      100
#define LOGTASK_TIMEOUT_MS      500
#define USBTASK_TIMEOUT_MS      1000
#define NETWORKTASK_TIMEOUT_MS  1000

// configMAX_PRIORITIES - 1: Highest Priority
// Lower: configMAX_PRIORITIES - 2
/**
 * @brief  Initialize and create all RTOS tasks and objects
 * | Task Name       | Priority | WDT Timeout (ms) | Description/Trigger                 |
 * | --------------- | :------: | :--------------: | ----------------------------------- |
 * | Root            |    1     |       N/A        | System Init (High Priority)         |
 * | Monitor         |    1     |       N/A        | Task Watchdog & System Health       |
 * | USB_Task        |    3     |       N/A        | USB Device / Host Processing        |
 * | Network_Task    |    3     |       N/A        | TCP/IP Stack & Network Services     |
 * | CLI (App_CLI)   |    3     |       100        | Command Line Interface              |
 * | LED_Alive       |    4     |       4000       | Heartbeat/Blinky (Low Priority)     |
 * | Log_Task        |    4     |       500        | Logging/Flash Write                 |
 * | IDLE            |    0     |       N/A        | Idle Task                           |
 */

// Timeout callback for monitored tasks
static void AliveTimeoutCallback(void)
{
    // printf("Task timeout detected!\n");
}

static void CLITimeoutCallback(void)
{
    // printf("Task timeout detected!\n");
}

// Root initialization function - creates all application tasks
void AppRoot_GrowUp(void)
{
    xlog_Init();
    Dmesg_Init();
    osThreadNew_WithMonitor(App_AliveTask,      NULL, &alive_attr,      ALIVETASK_TIMEOUT_MS,   AliveTimeoutCallback);
    osThreadNew_WithMonitor(App_CLITask,        NULL, &cli_attr,        CLITASK_TIMEOUT_MS,     CLITimeoutCallback);
    osThreadNew_WithMonitor(App_LogTask,        NULL, &log_attr,        LOGTASK_TIMEOUT_MS,     NULL);
    osThreadNew_WithMonitor(App_USBTask,        NULL, &usb_attr,        USBTASK_TIMEOUT_MS,     NULL);
    osThreadNew_WithMonitor(App_NetworkTask,    NULL, &network_attr,    NETWORKTASK_TIMEOUT_MS, NULL);
    // printf("All application tasks created\n");
}

void App_Start(void)
{
    osRegisterRootInit(AppRoot_GrowUp);
    osKernelStart();
}