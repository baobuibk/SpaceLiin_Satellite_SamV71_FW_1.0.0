/*
 * cli_setup.c
 *
 *  Created on: December, 2025
 *      Author: CAO HIEU
 */
#include "cli_command.h"
#include "stdio.h"
#include "M1_SysApp/EmbeddedCLI/CLI_Setup/cli_setup.h"
#include "stdlib.h"
#include "string.h"
//#include "main.h"
//#include "board.h"
/*************************************************
 *              More User Include                *
 *************************************************/
#include "M5_Utils/DateTime/date_time.h"
#include "M2_BSP/UART/uart_irq.h"
#include "M1_SysApp/EmbeddedCLI/CLI_Auth/simple_shield.h"

#include "M0_App/OS/os.h"
#include "M1_SysApp/dmesg/dmesg.h"
/*************************************************
 *                Command Define                 *
 *************************************************/
static void CMD_ClearCLI(EmbeddedCli *cli, char *args, void *context);
static void CMD_RtcSet(EmbeddedCli *cli, char *args, void *context);
static void CMD_RtcGet(EmbeddedCli *cli, char *args, void *context);
static void CMD_RtcSetEpoch(EmbeddedCli *cli, char *args, void *context);

static void CMD_OsStat(EmbeddedCli *cli, char *args, void *context);
static void CMD_Dmesg(EmbeddedCli *cli, char *args, void *context);

static void CMD_Reset(EmbeddedCli *cli, char *args, void *context);

/*************************************************
 * Command Define "Dev"              *
 *************************************************/
// static void CMD_DevCM4TestConnection(EmbeddedCli *cli, char *args, void *context);

/*************************************************
 *                 Command  Array                *
 *************************************************/
// Guide: Command bindings are declared in the following order:
// { category, name, help, tokenizeArgs, context, binding }
// - category: Command group; set to NULL if grouping is not needed.
// - name: Command name (required)
// - help: Help string describing the command (required)
// - tokenizeArgs: Set to true to automatically split arguments when the command is called.
// - context: Pointer to a command-specific context; can be NULL.
// - binding: Callback function that handles the command.

static const CliCommandBinding cliStaticBindings_internal[] = {
	{ "Ultis",          "dev",          "Print list of developer commands",             		false,  NULL, CMD_Dev,           },
    { "Ultis",		 	"help",        	"Print list of commands [Firmware: 1]",             	false,  NULL, CMD_Help,			 },
    { "Ultis",			"cls",         	"Clears the console",                               	false,  NULL, CMD_ClearCLI,  	 },
    { "Time", 			"rtc_set",     	"Set RTC time: rtc_set <h> <m> <s> <DD> <MM> <YY>",   	true,  	NULL, CMD_RtcSet,    	 },
    { "Time",         	"rtc_get",     	"Get RTC. Usage: rtc_get <hard|soft|work|epoch|all>",  	true,  	NULL, CMD_RtcGet,    	 },
    { "Time",         	"epoch_set",   	"Set RTC time by epoch: rtc_setepoch <epoch>",        	true,  	NULL, CMD_RtcSetEpoch, 	 },

    { "System",         "os_stat",   	"Check FreeRTOS tasks: os_stat",        	            false,  NULL, CMD_OsStat, 	     },
    { "System",         "dmesg",        "Print dmesg logs: dmesg [N]",                          true,   NULL, CMD_Dmesg,         },

    { NULL,         	"reset",       	"Reset MCU: reset",                                 	false, 	NULL, CMD_Reset,     	 },
};
/*************************************************
 *                 External Declarations         *
 *************************************************/

/*************************************************
 *             Command List Function             *
 *************************************************/
static void CMD_ClearCLI(EmbeddedCli *cli, char *args, void *context) {
    char buffer[10];

    snprintf(buffer, sizeof(buffer), "\33[2J");
    embeddedCliPrint(cli, buffer);
}

static void CMD_RtcSet(EmbeddedCli *cli, char *args, void *context) {
    const char *arg1 = embeddedCliGetToken(args, 1); // hour
    const char *arg2 = embeddedCliGetToken(args, 2); // minute
    const char *arg3 = embeddedCliGetToken(args, 3); // second
    const char *arg4 = embeddedCliGetToken(args, 4); // day
    const char *arg5 = embeddedCliGetToken(args, 5); // month
    const char *arg6 = embeddedCliGetToken(args, 6); // year

    char buffer[100];
    if (arg1 == NULL || arg2 == NULL || arg3 == NULL ||
        arg4 == NULL || arg5 == NULL || arg6 == NULL) {
        snprintf(buffer, sizeof(buffer),
                 "Usage: rtc_set <hour> <minute> <second> <day> <month> <year>");
        embeddedCliPrint(cli, buffer);
        return;
    }

    int hour   = atoi(arg1);
    int minute = atoi(arg2);
    int second = atoi(arg3);
    int day    = atoi(arg4);
    int month  = atoi(arg5);
    int year   = atoi(arg6);

    if (hour < 0 || hour > 23) {
        embeddedCliPrint(cli, "Invalid hour (must be 0-23). Please enter again.");
        return;
    }
    if (minute < 0 || minute > 59) {
        embeddedCliPrint(cli, "Invalid minute (must be 0-59). Please enter again.");
        return;
    }
    if (second < 0 || second > 59) {
        embeddedCliPrint(cli, "Invalid second (must be 0-59). Please enter again.");
        return;
    }
    if (day < 1 || day > 31) {
        embeddedCliPrint(cli, "Invalid day (must be 1-31). Please enter again.");
        return;
    }
    if (month < 1 || month > 12) {
        embeddedCliPrint(cli, "Invalid month (must be 1-12). Please enter again.");
        return;
    }
    if (year < 0 || year > 99) {
        embeddedCliPrint(cli, "Invalid year (must be 2 digits, e.g., 25 for 2025). Please enter again.");
        return;
    }

    s_DateTime dt;
    dt.hour   = (uint8_t)hour;
    dt.minute = (uint8_t)minute;
    dt.second = (uint8_t)second;
    dt.day    = (uint8_t)day;
    dt.month  = (uint8_t)month;
    dt.year   = (uint8_t)year;

    Utils_SetRTC(&dt);

    snprintf(buffer, sizeof(buffer),
             "--> RTC set to %02d:%02d:%02d, %02d/%02d/20%02d",
             dt.hour, dt.minute, dt.second, dt.day, dt.month, dt.year);
    embeddedCliPrint(cli, buffer);
    embeddedCliPrint(cli, "");
}

static void CMD_RtcGet(EmbeddedCli *cli, char *args, void *context) {
    const char *mode = embeddedCliGetToken(args, 1);
    char buffer[100];

    if (mode == NULL) {
        snprintf(buffer, sizeof(buffer), "Usage: rtc_get <hard|soft|work|epoch|all>");
        embeddedCliPrint(cli, buffer);
        return;
    }

    //Hard
    if (strcmp(mode, "hard") == 0) {
        embeddedCliPrint(cli, "Failed to get hard RTC\r\n");
    } else if (strcmp(mode, "soft") == 0) {
        s_DateTime rtc;
        Utils_GetRTC(&rtc);
        snprintf(buffer, sizeof(buffer),
                 "--> Soft RTC: Time: %02d:%02d:%02d, Date: %02d/%02d/20%02d",
                 rtc.hour, rtc.minute, rtc.second,
                 rtc.day, rtc.month, rtc.year);
        embeddedCliPrint(cli, buffer);
    } else if (strcmp(mode, "work") == 0) {
        uint32_t days = 0;
        uint8_t hours = 0, minutes = 0, seconds = 0;
        Utils_GetWorkingTime(&days, &hours, &minutes, &seconds);
        snprintf(buffer, sizeof(buffer),
                        "--> Working Uptime: Time: %02d:%02d:%02d, Days: %d",
                        hours, minutes, seconds, (uint8_t)days);
        embeddedCliPrint(cli, buffer);
    }else if (strcmp(mode, "epoch") == 0) {
        uint32_t epoch = Utils_GetEpoch();
        snprintf(buffer, sizeof(buffer), "--> Epoch: %lu", (unsigned long)epoch);
        embeddedCliPrint(cli, buffer);
    } else if (strcmp(mode, "all") == 0) {
        // Soft RTC
        s_DateTime rtc;
        Utils_GetRTC(&rtc);
        snprintf(buffer, sizeof(buffer),
                 "--> Soft RTC: Time: %02d:%02d:%02d, Date: %02d/%02d/20%02d",
                 rtc.hour, rtc.minute, rtc.second,
                 rtc.day, rtc.month, rtc.year);
        embeddedCliPrint(cli, buffer);
        // Working uptime:
        uint32_t days = 0;
        uint8_t hours = 0, minutes = 0, seconds = 0;
        Utils_GetWorkingTime(&days, &hours, &minutes, &seconds);
        snprintf(buffer, sizeof(buffer),
                        "--> Working Uptime: Time: %02d:%02d:%02d, Days: %d",
                        hours, minutes, seconds, (uint8_t)days);
        embeddedCliPrint(cli, buffer);
        // Epoch
        uint32_t epoch = Utils_GetEpoch();
        snprintf(buffer, sizeof(buffer), "--> Epoch: %lu", (unsigned long)epoch);
        embeddedCliPrint(cli, buffer);
    } else {
        snprintf(buffer, sizeof(buffer), "Unknown mode. Use: rtc_get <hard|soft|work|epoch|all>");
        embeddedCliPrint(cli, buffer);
    }
    embeddedCliPrint(cli, "");
}

static void CMD_RtcSetEpoch(EmbeddedCli *cli, char *args, void *context) {
    const char *arg1 = embeddedCliGetToken(args, 1);
    char buffer[100];
    if (arg1 == NULL) {
        snprintf(buffer, sizeof(buffer), "Usage: rtc_setepoch <epoch>");
        embeddedCliPrint(cli, buffer);
        return;
    }

    uint32_t epoch = (uint32_t)strtoul(arg1, NULL, 0);

    if (epoch < EPOCH_OFFSET_UNIX) {
        snprintf(buffer, sizeof(buffer), "Invalid epoch. Must be >= %lu", (unsigned long)EPOCH_OFFSET_UNIX);
        embeddedCliPrint(cli, buffer);
        return;
    }

    Utils_SetEpoch(epoch);

    s_DateTime dt;
    EpochToDateTime(epoch - EPOCH_OFFSET_UNIX, &dt);

    snprintf(buffer, sizeof(buffer),
             "--> RTC set to %02d:%02d:%02d, %02d/%02d/20%02d",
             dt.hour, dt.minute, dt.second, dt.day, dt.month, dt.year);
    embeddedCliPrint(cli, buffer);
    embeddedCliPrint(cli, "");
}

static void CMD_OsStat(EmbeddedCli *cli, char *args, void *context) {
    TaskStatus_t taskStatusArray[22];
    UBaseType_t arraySize = 22;
    UBaseType_t totalTasks;
    char buffer[256];

    UBaseType_t numTasks = uxTaskGetNumberOfTasks();
        snprintf(buffer, sizeof(buffer), "Number of tasks running: %lu", (unsigned long)numTasks);
        embeddedCliPrint(cli, buffer);

    totalTasks = uxTaskGetSystemState(taskStatusArray, arraySize, NULL);

    if (totalTasks == 0) {
        embeddedCliPrint(cli, "No tasks found or error occurred.");
        embeddedCliPrint(cli, "");
        return;
    }

    snprintf(buffer, sizeof(buffer), "Total Tasks: %lu", (unsigned long)totalTasks);
    embeddedCliPrint(cli, buffer);
    embeddedCliPrint(cli, "Task Name          State     Prio  Stack Left");
    embeddedCliPrint(cli, "----------------------------------------");

	for (UBaseType_t i = 0; i < totalTasks; i++) {
		const char *stateStr;
		switch (taskStatusArray[i].eCurrentState) {
		case eRunning:
			stateStr = "Running";
			break;
		case eReady:
			stateStr = "Ready";
			break;
		case eBlocked:
			stateStr = "Blocked";
			break;
		case eSuspended:
			stateStr = "Suspend";
			break;
		case eDeleted:
			stateStr = "Deleted";
			break;
		default:
			stateStr = "Unknown";
			break;
		}

		snprintf(buffer, sizeof(buffer), "%-18s %-10s %-4lu %10lu",
				taskStatusArray[i].pcTaskName, stateStr,
				taskStatusArray[i].uxCurrentPriority,
				taskStatusArray[i].usStackHighWaterMark);
		embeddedCliPrint(cli, buffer);
	}

	embeddedCliPrint(cli, "----------------------------------------");
	embeddedCliPrint(cli, "");
}

static void CMD_Dmesg(EmbeddedCli *cli, char *args, void *context) {
    const char *arg1 = embeddedCliGetToken(args, 1);
    char buffer[64];
    embeddedCliPrint(cli, "Dmesg - Logger Message:");

    if (arg1 == NULL) {
        embeddedCliPrint(cli, "--> Oldest >>");
        Dmesg_GetLogs();
        embeddedCliPrint(cli, "--> Latest <<");
    } else {
        size_t N = (size_t)strtoul(arg1, NULL, 10);
        snprintf(buffer, sizeof(buffer), "Latest %lu Logs:", (unsigned long)N);
        embeddedCliPrint(cli, buffer);
        Dmesg_GetLatestN(N);
    }

    embeddedCliPrint(cli, "");
}



static void CMD_Reset(EmbeddedCli *cli, char *args, void *context) {
	NVIC_SystemReset();
    embeddedCliPrint(cli, "");
}

/*************************************************
 * Command Function "Dev"            *
 *************************************************/


/*************************************************
 *                  End CMD List                 *
 *************************************************/

/*************************************************
 *                Getter - Helper                *
 *************************************************/
const CliCommandBinding *getCliStaticBindings(void) {
    return cliStaticBindings_internal;
}

uint16_t getCliStaticBindingCount(void) {
    return sizeof(cliStaticBindings_internal) / sizeof(cliStaticBindings_internal[0]);
}


