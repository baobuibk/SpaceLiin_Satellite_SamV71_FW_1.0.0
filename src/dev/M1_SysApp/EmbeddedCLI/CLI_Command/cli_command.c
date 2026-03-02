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

#include "M3_Devices/FRAM_USART-SPI/fram_usart.h"
#include "M0_App/AppOS/App_3_CSP/app_csp.h"
#include <csp/csp.h>

#include "exp_rparam_table.h"
#include <param/table.h>
#include <param/internal/types.h>

#include "M2_BSP/BSP_Power/bsp_power.h"
#include "M2_BSP/BSP_Led/bsp_led.h"
#include "M2_BSP/BSP_Heater/bsp_heater.h"

/*************************************************
 *                     Extern                    *
 *************************************************/
extern gs_param_table_instance_t g_tables[5];
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

static void CMD_FramReadID(EmbeddedCli *cli, char *args, void *context);
static void CMD_FramWrite(EmbeddedCli *cli, char *args, void *context);
static void CMD_FramRead(EmbeddedCli *cli, char *args, void *context);
static void CMD_FramWriteStr(EmbeddedCli *cli, char *args, void *context);
static void CMD_FramReadStr(EmbeddedCli *cli, char *args, void *context);
static void CMD_FramTest(EmbeddedCli *cli, char *args, void *context);

static void CMD_CanSend(EmbeddedCli *cli, char *args, void *context);
static void CMD_CspPing(EmbeddedCli *cli, char *args, void *context);
static void CMD_CspSend(EmbeddedCli *cli, char *args, void *context);
static void CMD_CspInfo(EmbeddedCli *cli, char *args, void *context);

static void CMD_ParamShow(EmbeddedCli *cli, char *args, void *context);
static void CMD_ParamSet (EmbeddedCli *cli, char *args, void *context);

static void CMD_PowerSOM_ON(EmbeddedCli *cli, char *args, void *context); 
static void CMD_PowerSOM_OFF(EmbeddedCli *cli, char *args, void *context);
static void CMD_PowerSOM_Get(EmbeddedCli *cli, char *args, void *context);
static void CMD_PowerBuckPeri_ON(EmbeddedCli *cli, char *args, void *context);
static void CMD_PowerBuckPeri_OFF(EmbeddedCli *cli, char *args, void *context); 
static void CMD_PowerBuckPeri_Get(EmbeddedCli *cli, char *args, void *context);
static void CMD_PowerTec_ON(EmbeddedCli *cli, char *args, void *context);
static void CMD_PowerTec_OFF(EmbeddedCli *cli, char *args, void *context);
static void CMD_PowerTec_Get(EmbeddedCli *cli, char *args, void *context);
static void CMD_PowerHD4_ON(EmbeddedCli *cli, char *args, void *context);
static void CMD_PowerHD4_OFF(EmbeddedCli *cli, char *args, void *context); 
static void CMD_PowerHD4_Get(EmbeddedCli *cli, char *args, void *context); 
static void CMD_PowerSolenoid_ON(EmbeddedCli *cli, char *args, void *context);
static void CMD_PowerSolenoid_OFF(EmbeddedCli *cli, char *args, void *context);
static void CMD_PowerSolenoid_Get(EmbeddedCli *cli, char *args, void *context);
static void CMD_PowerLP_ON(EmbeddedCli *cli, char *args, void *context);
static void CMD_PowerLP_OFF(EmbeddedCli *cli, char *args, void *context);
static void CMD_PowerLP_Get(EmbeddedCli *cli, char *args, void *context); 
static void CMD_PowerHeater_ON(EmbeddedCli *cli, char *args, void *context);
static void CMD_PowerHeater_OFF(EmbeddedCli *cli, char *args, void *context);
static void CMD_PowerHeater_Get(EmbeddedCli *cli, char *args, void *context); 
static void CMD_PowerAll_ON(EmbeddedCli *cli, char *args, void *context);
static void CMD_PowerAll_OFF(EmbeddedCli *cli, char *args, void *context);
static void CMD_PowerAll_Get(EmbeddedCli *cli, char *args, void *context); 
static void CMD_LED_Set (EmbeddedCli *cli, char *args, void *context);
static void CMD_LED_Reset (EmbeddedCli *cli, char *args, void *context);

static void CMD_HEATER_SetDuty (EmbeddedCli *cli, char *args, void *context);



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
    
    { "FRAM",          "fram_id",      "Read FRAM ID: fram_id",                                false,  NULL, CMD_FramReadID,    },
    { "FRAM",          "fram_write",   "Write byte: fram_write <addr> <val1> [val2] ...",     true,   NULL, CMD_FramWrite,     },
    { "FRAM",          "fram_read",    "Read bytes: fram_read <addr> <len>",                   true,   NULL, CMD_FramRead,      },
    { "FRAM",          "fram_wstr",    "Write string: fram_wstr <addr> <string>",              true,   NULL, CMD_FramWriteStr,  },
    { "FRAM",          "fram_rstr",    "Read string: fram_rstr <addr> <maxlen>",               true,   NULL, CMD_FramReadStr,   },
    { "FRAM",          "fram_test",    "Run FRAM test: fram_test",                             false,  NULL, CMD_FramTest,      },

    { "CAN",            "can_send",  "Raw CAN: can_send <id> [ext] [b0 b1...]",                 true,  NULL, CMD_CanSend,       },
    { "CSP",            "csp_ping",  "CSP ping: csp_ping <dest_addr> [timeout_ms]",             true,  NULL, CMD_CspPing,       },
    { "CSP",            "csp_send",  "CSP send: csp_send <addr> <port> <data...>",              true,  NULL, CMD_CspSend,       },
    { "CSP",            "csp_info",  "CSP status: csp_info",                                    false, NULL, CMD_CspInfo,       },

    { "Param",          "param_show", "Show all fields of a table: param_show <table_id 0-4>",           true, NULL, CMD_ParamShow },
    { "Param",          "param_set",  "Set a field in a table: param_set <table_id> <addr> <type> <value>", true, NULL, CMD_ParamSet  },
    
    { "LED",          "led_set",  "Comment following: led_set",                                 true, NULL, CMD_LED_Set  },
    { "LED",          "led_reset",  "Comment following: led_reset",                             true, NULL, CMD_LED_Reset  },
    
    { "HTR",          "heater_set",  "Comment following: heater_set <channel> <duty> ",         true, NULL, CMD_HEATER_SetDuty  },

    
    

    
    { "POWER",          "power_som_on",  "power_som_on: enable efuse for turn on som",        false, NULL, CMD_PowerSOM_ON, },
    { "POWER",          "power_som_off",      "power_som_off: disable efuse for turn off som",          false, NULL, CMD_PowerSOM_OFF },
    { "POWER",          "power_som_get",      "power_som_get: get power status of som",                 false, NULL, CMD_PowerSOM_Get },

    { "POWER",          "power_buck_peri_on", "power_buck_peri_on: enable efuse for turn on buck peri",   false, NULL, CMD_PowerBuckPeri_ON },
    { "POWER",          "power_buck_peri_off","power_buck_peri_off: disable efuse for turn off buck peri", false, NULL, CMD_PowerBuckPeri_OFF },
    { "POWER",          "power_buck_peri_get","power_buck_peri_get: get power status of buck peri",      false, NULL, CMD_PowerBuckPeri_Get },

    { "POWER",          "power_tec_on",       "power_tec_on: enable efuse for turn on tec",             false, NULL, CMD_PowerTec_ON },
    { "POWER",          "power_tec_off",      "power_tec_off: disable efuse for turn off tec",           false, NULL, CMD_PowerTec_OFF },
    { "POWER",          "power_tec_get",      "power_tec_get: get power status of tec",                  false, NULL, CMD_PowerTec_Get },

    { "POWER",          "power_hd4_on",       "power_hd4_on: enable efuse for turn on hd4",             false, NULL, CMD_PowerHD4_ON },
    { "POWER",          "power_hd4_off",      "power_hd4_off: disable efuse for turn off hd4",           false, NULL, CMD_PowerHD4_OFF },
    { "POWER",          "power_hd4_get",      "power_hd4_get: get power status of hd4",                  false, NULL, CMD_PowerHD4_Get },

    { "POWER",          "power_solenoid_on",  "power_solenoid_on: enable efuse for turn on solenoid",    false, NULL, CMD_PowerSolenoid_ON },
    { "POWER",          "power_solenoid_off", "power_solenoid_off: disable efuse for turn off solenoid",  false, NULL, CMD_PowerSolenoid_OFF },
    { "POWER",          "power_solenoid_get", "power_solenoid_get: get power status of solenoid",       false, NULL, CMD_PowerSolenoid_Get },

    { "POWER",          "power_lp_on",        "power_lp_on: enable efuse for turn on lp",               false, NULL, CMD_PowerLP_ON },
    { "POWER",          "power_lp_off",       "power_lp_off: disable efuse for turn off lp",             false, NULL, CMD_PowerLP_OFF },
    { "POWER",          "power_lp_get",       "power_lp_get: get power status of lp",                   false, NULL, CMD_PowerLP_Get },

    { "POWER",          "power_heater_on",    "power_heater_on: enable efuse for turn on heater",       false, NULL, CMD_PowerHeater_ON },
    { "POWER",          "power_heater_off",   "power_heater_off: disable efuse for turn off heater",      false, NULL, CMD_PowerHeater_OFF },
    { "POWER",          "power_heater_get",   "power_heater_get: get power status of heater",           false, NULL, CMD_PowerHeater_Get },

    { "POWER",          "power_all_on",       "power_all_on: enable efuse for turn on all rails",       false, NULL, CMD_PowerAll_ON },
    { "POWER",          "power_all_off",      "power_all_off: disable efuse for turn off all rails",      false, NULL, CMD_PowerAll_OFF },
    { "POWER",          "power_all_get",      "power_all_get: get power status of all rails",           false, NULL, CMD_PowerAll_Get },

    
    { NULL,         	"reset",       	"Reset MCU: reset",                                 	false, 	NULL, CMD_Reset,     	},
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

static void CMD_FramReadID(EmbeddedCli *cli, char *args, void *context) {
    uint8_t id[9] = {0};
    char buffer[128];
    FRAM_StatusTypeDef status;
    FRAM_USART_HandleTypeDef *hfram = FRAM_USART_GetHandle();
    
    if (hfram->mode == FRAM_MODE_POLLING) {
        status = FRAM_USART_ReadID_Polling(id, 9);
    } else {
        status = FRAM_USART_ReadID_IT(id, 9);
        if (status == FRAM_OK) {
            status = FRAM_USART_WaitForComplete(1000);
        }
    }
    
    if (status == FRAM_OK) {
        embeddedCliPrint(cli, "--> FRAM Device ID:");
        snprintf(buffer, sizeof(buffer), 
                 "    Manufacturer: 0x%02X %02X %02X %02X", 
                 id[0], id[1], id[2], id[3]);
        embeddedCliPrint(cli, buffer);
        
        snprintf(buffer, sizeof(buffer), 
                 "    Product ID:   0x%02X %02X", 
                 id[4], id[5]);
        embeddedCliPrint(cli, buffer);
        
        snprintf(buffer, sizeof(buffer), 
                 "    Density:      0x%02X %02X %02X", 
                 id[6], id[7], id[8]);
        embeddedCliPrint(cli, buffer);
    } else {
        snprintf(buffer, sizeof(buffer), "--> Failed to read ID: %d", status);
        embeddedCliPrint(cli, buffer);
    }
    
    embeddedCliPrint(cli, "");
}

/**
 * @brief Write bytes to FRAM
 * Usage: fram_write <addr> <val1> [val2] [val3] ...
 * Example: fram_write 0x100 0xAA 0xBB 0xCC
 */
static void CMD_FramWrite(EmbeddedCli *cli, char *args, void *context) {
    const char *addrStr = embeddedCliGetToken(args, 1);
    char buffer[128];
    
    if (addrStr == NULL) {
        embeddedCliPrint(cli, "Usage: fram_write <addr> <val1> [val2] ...");
        embeddedCliPrint(cli, "Example: fram_write 0x100 0xAA 0xBB 0xCC");
        return;
    }
    
    uint32_t addr = (uint32_t)strtoul(addrStr, NULL, 0);
    
    // Count and collect values
    uint8_t data[256];
    uint16_t count = 0;
    
    for (uint16_t i = 2; i <= 257 && count < 256; i++) {
        const char *valStr = embeddedCliGetToken(args, i);
        if (valStr == NULL) break;
        data[count++] = (uint8_t)strtoul(valStr, NULL, 0);
    }
    
    if (count == 0) {
        embeddedCliPrint(cli, "Error: No data to write");
        return;
    }
    
    FRAM_StatusTypeDef status;
    FRAM_USART_HandleTypeDef *hfram = FRAM_USART_GetHandle();
    
    if (hfram->mode == FRAM_MODE_POLLING) {
        status = FRAM_USART_WriteMem_Polling(addr, data, count);
    } else {
        status = FRAM_USART_WriteMem_IT(addr, data, count);
        if (status == FRAM_OK) {
            status = FRAM_USART_WaitForComplete(1000);
        }
    }
    
    if (status == FRAM_OK) {
        snprintf(buffer, sizeof(buffer), 
                 "--> Written %u bytes to address 0x%06lX", 
                 count, (unsigned long)addr);
        embeddedCliPrint(cli, buffer);
    } else {
        snprintf(buffer, sizeof(buffer), "--> Write failed: %d", status);
        embeddedCliPrint(cli, buffer);
    }
    
    embeddedCliPrint(cli, "");
}

/**
 * @brief Read bytes from FRAM
 * Usage: fram_read <addr> <len>
 * Example: fram_read 0x100 16
 */
static void CMD_FramRead(EmbeddedCli *cli, char *args, void *context) {
    const char *addrStr = embeddedCliGetToken(args, 1);
    const char *lenStr = embeddedCliGetToken(args, 2);
    char buffer[128];
    
    if (addrStr == NULL || lenStr == NULL) {
        embeddedCliPrint(cli, "Usage: fram_read <addr> <len>");
        embeddedCliPrint(cli, "Example: fram_read 0x100 16");
        return;
    }
    
    uint32_t addr = (uint32_t)strtoul(addrStr, NULL, 0);
    uint16_t len = (uint16_t)strtoul(lenStr, NULL, 0);
    
    if (len == 0 || len > 256) {
        embeddedCliPrint(cli, "Error: Length must be 1-256");
        return;
    }
    
    uint8_t data[256];
    FRAM_StatusTypeDef status;
    FRAM_USART_HandleTypeDef *hfram = FRAM_USART_GetHandle();
    
    if (hfram->mode == FRAM_MODE_POLLING) {
        status = FRAM_USART_ReadMem_Polling(addr, data, len);
    } else {
        status = FRAM_USART_ReadMem_IT(addr, data, len);
        if (status == FRAM_OK) {
            status = FRAM_USART_WaitForComplete(1000);
        }
    }
    
    if (status == FRAM_OK) {
        snprintf(buffer, sizeof(buffer), 
                 "--> Read %u bytes from address 0x%06lX:", 
                 len, (unsigned long)addr);
        embeddedCliPrint(cli, buffer);
        
        // Print in hex format (16 bytes per line)
        for (uint16_t i = 0; i < len; i += 16) {
            snprintf(buffer, sizeof(buffer), "    %04X: ", i);
            size_t offset = strlen(buffer);
            
            for (uint16_t j = 0; j < 16 && (i + j) < len; j++) {
                snprintf(buffer + offset, sizeof(buffer) - offset, 
                         "%02X ", data[i + j]);
                offset += 3;
            }
            
            embeddedCliPrint(cli, buffer);
        }
    } else {
        snprintf(buffer, sizeof(buffer), "--> Read failed: %d", status);
        embeddedCliPrint(cli, buffer);
    }
    
    embeddedCliPrint(cli, "");
}

/**
 * @brief Write string to FRAM
 * Usage: fram_wstr <addr> <string>
 * Example: fram_wstr 0x200 "Hello FRAM!"
 */
static void CMD_FramWriteStr(EmbeddedCli *cli, char *args, void *context) {
    const char *addrStr = embeddedCliGetToken(args, 1);
    const char *strData = embeddedCliGetToken(args, 2);
    char buffer[128];
    
    if (addrStr == NULL || strData == NULL) {
        embeddedCliPrint(cli, "Usage: fram_wstr <addr> <string>");
        embeddedCliPrint(cli, "Example: fram_wstr 0x200 \"Hello FRAM!\"");
        return;
    }
    
    uint32_t addr = (uint32_t)strtoul(addrStr, NULL, 0);
    uint16_t len = strlen(strData) + 1; // Include null terminator
    
    if (len > 256) {
        embeddedCliPrint(cli, "Error: String too long (max 255 chars)");
        return;
    }
    
    FRAM_StatusTypeDef status;
    FRAM_USART_HandleTypeDef *hfram = FRAM_USART_GetHandle();
    
    if (hfram->mode == FRAM_MODE_POLLING) {
        status = FRAM_USART_WriteMem_Polling(addr, (uint8_t*)strData, len);
    } else {
        status = FRAM_USART_WriteMem_IT(addr, (uint8_t*)strData, len);
        if (status == FRAM_OK) {
            status = FRAM_USART_WaitForComplete(1000);
        }
    }
    
    if (status == FRAM_OK) {
        snprintf(buffer, sizeof(buffer), 
                 "--> Written string (%u bytes) to 0x%06lX", 
                 len, (unsigned long)addr);
        embeddedCliPrint(cli, buffer);
        snprintf(buffer, sizeof(buffer), "    \"%s\"", strData);
        embeddedCliPrint(cli, buffer);
    } else {
        snprintf(buffer, sizeof(buffer), "--> Write failed: %d", status);
        embeddedCliPrint(cli, buffer);
    }
    
    embeddedCliPrint(cli, "");
}

/**
 * @brief Read string from FRAM
 * Usage: fram_rstr <addr> <maxlen>
 * Example: fram_rstr 0x200 64
 */
static void CMD_FramReadStr(EmbeddedCli *cli, char *args, void *context) {
    const char *addrStr = embeddedCliGetToken(args, 1);
    const char *lenStr = embeddedCliGetToken(args, 2);
    char buffer[128];
    
    if (addrStr == NULL || lenStr == NULL) {
        embeddedCliPrint(cli, "Usage: fram_rstr <addr> <maxlen>");
        embeddedCliPrint(cli, "Example: fram_rstr 0x200 64");
        return;
    }
    
    uint32_t addr = (uint32_t)strtoul(addrStr, NULL, 0);
    uint16_t maxLen = (uint16_t)strtoul(lenStr, NULL, 0);
    
    if (maxLen == 0 || maxLen > 256) {
        embeddedCliPrint(cli, "Error: Max length must be 1-256");
        return;
    }
    
    uint8_t data[256];
    FRAM_StatusTypeDef status;
    FRAM_USART_HandleTypeDef *hfram = FRAM_USART_GetHandle();
    
    if (hfram->mode == FRAM_MODE_POLLING) {
        status = FRAM_USART_ReadMem_Polling(addr, data, maxLen);
    } else {
        status = FRAM_USART_ReadMem_IT(addr, data, maxLen);
        if (status == FRAM_OK) {
            status = FRAM_USART_WaitForComplete(1000);
        }
    }
    
    if (status == FRAM_OK) {
        data[maxLen - 1] = '\0'; // Ensure null termination
        
        snprintf(buffer, sizeof(buffer), 
                 "--> Read string from address 0x%06lX:", 
                 (unsigned long)addr);
        embeddedCliPrint(cli, buffer);
        snprintf(buffer, sizeof(buffer), "    \"%s\"", (char*)data);
        embeddedCliPrint(cli, buffer);
    } else {
        snprintf(buffer, sizeof(buffer), "--> Read failed: %d", status);
        embeddedCliPrint(cli, buffer);
    }
    
    embeddedCliPrint(cli, "");
}

/**
 * @brief Run comprehensive FRAM test
 * Usage: fram_test
 */
static void CMD_FramTest(EmbeddedCli *cli, char *args, void *context) {
    char buffer[128];
    uint8_t testData[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                            0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    uint8_t readData[16] = {0};
    const uint32_t testAddr = 0x001000;
    
    embeddedCliPrint(cli, "=== FRAM Comprehensive Test ===");
    embeddedCliPrint(cli, "");
    
    FRAM_USART_HandleTypeDef *hfram = FRAM_USART_GetHandle();
    FRAM_StatusTypeDef status;
    
    // Test 1: Read ID
    embeddedCliPrint(cli, "[1] Reading Device ID...");
    uint8_t id[9] = {0};
    
    if (hfram->mode == FRAM_MODE_POLLING) {
        status = FRAM_USART_ReadID_Polling(id, 9);
    } else {
        status = FRAM_USART_ReadID_IT(id, 9);
        if (status == FRAM_OK) status = FRAM_USART_WaitForComplete(1000);
    }
    
    if (status == FRAM_OK) {
        snprintf(buffer, sizeof(buffer), 
                 "    ID: %02X %02X %02X %02X %02X %02X %02X %02X %02X", 
                 id[0], id[1], id[2], id[3], id[4], id[5], id[6], id[7], id[8]);
        embeddedCliPrint(cli, buffer);
        embeddedCliPrint(cli, "    PASS");
    } else {
        embeddedCliPrint(cli, "    FAIL");
    }
    embeddedCliPrint(cli, "");
    
    // Test 2: Write test pattern
    snprintf(buffer, sizeof(buffer), "[2] Writing test pattern to 0x%06lX...", (unsigned long)testAddr);
    embeddedCliPrint(cli, buffer);
    
    if (hfram->mode == FRAM_MODE_POLLING) {
        status = FRAM_USART_WriteMem_Polling(testAddr, testData, 16);
    } else {
        status = FRAM_USART_WriteMem_IT(testAddr, testData, 16);
        if (status == FRAM_OK) status = FRAM_USART_WaitForComplete(1000);
    }
    
    if (status == FRAM_OK) {
        embeddedCliPrint(cli, "    PASS");
    } else {
        embeddedCliPrint(cli, "    FAIL");
    }
    embeddedCliPrint(cli, "");
    
    // Test 3: Read back and verify
    embeddedCliPrint(cli, "[3] Reading back and verifying...");
    
    if (hfram->mode == FRAM_MODE_POLLING) {
        status = FRAM_USART_ReadMem_Polling(testAddr, readData, 16);
    } else {
        status = FRAM_USART_ReadMem_IT(testAddr, readData, 16);
        if (status == FRAM_OK) status = FRAM_USART_WaitForComplete(1000);
    }
    
    if (status == FRAM_OK) {
        bool match = true;
        for (int i = 0; i < 16; i++) {
            if (testData[i] != readData[i]) {
                match = false;
                break;
            }
        }
        
        if (match) {
            embeddedCliPrint(cli, "    Data verified: PASS");
        } else {
            embeddedCliPrint(cli, "    Data mismatch: FAIL");
            embeddedCliPrint(cli, "    Expected:");
            snprintf(buffer, sizeof(buffer), 
                     "    %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
                     testData[0], testData[1], testData[2], testData[3],
                     testData[4], testData[5], testData[6], testData[7],
                     testData[8], testData[9], testData[10], testData[11],
                     testData[12], testData[13], testData[14], testData[15]);
            embeddedCliPrint(cli, buffer);
            embeddedCliPrint(cli, "    Received:");
            snprintf(buffer, sizeof(buffer), 
                     "    %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
                     readData[0], readData[1], readData[2], readData[3],
                     readData[4], readData[5], readData[6], readData[7],
                     readData[8], readData[9], readData[10], readData[11],
                     readData[12], readData[13], readData[14], readData[15]);
            embeddedCliPrint(cli, buffer);
        }
    } else {
        embeddedCliPrint(cli, "    Read failed: FAIL");
    }
    embeddedCliPrint(cli, "");
    
    // Test 4: String write/read
    embeddedCliPrint(cli, "[4] Testing string operations...");
    const char *testStr = "FRAM Test OK!";
    const uint32_t strAddr = testAddr + 0x100;
    uint8_t strBuffer[32] = {0};
    
    if (hfram->mode == FRAM_MODE_POLLING) {
        status = FRAM_USART_WriteMem_Polling(strAddr, (uint8_t*)testStr, strlen(testStr) + 1);
    } else {
        status = FRAM_USART_WriteMem_IT(strAddr, (uint8_t*)testStr, strlen(testStr) + 1);
        if (status == FRAM_OK) status = FRAM_USART_WaitForComplete(1000);
    }
    
    if (status == FRAM_OK) {
        if (hfram->mode == FRAM_MODE_POLLING) {
            status = FRAM_USART_ReadMem_Polling(strAddr, strBuffer, 32);
        } else {
            status = FRAM_USART_ReadMem_IT(strAddr, strBuffer, 32);
            if (status == FRAM_OK) status = FRAM_USART_WaitForComplete(1000);
        }
        
        if (status == FRAM_OK && strcmp((char*)strBuffer, testStr) == 0) {
            snprintf(buffer, sizeof(buffer), "    String verified: \"%s\"", strBuffer);
            embeddedCliPrint(cli, buffer);
            embeddedCliPrint(cli, "    PASS");
        } else {
            embeddedCliPrint(cli, "    String mismatch: FAIL");
        }
    } else {
        embeddedCliPrint(cli, "    String write failed: FAIL");
    }
    
    embeddedCliPrint(cli, "");
    embeddedCliPrint(cli, "=== Test Complete ===");
    embeddedCliPrint(cli, "");
}

/**
 * @brief Raw CAN send (bypasses CSP, for hardware testing)
 * Usage: can_send <id> [ext] [b0 b1 ... b7]
 * Example: can_send 0x100 0xAA 0xBB          (standard 11-bit)
 *          can_send 0x12345 ext 0xAA 0xBB    (extended 29-bit)
 */
static void CMD_CanSend(EmbeddedCli *cli, char *args, void *context) {
    const char *idStr = embeddedCliGetToken(args, 1);
    char buffer[128];

    if (idStr == NULL) {
        embeddedCliPrint(cli, "Usage: can_send <id> [ext] [b0 b1 ... b7]");
        embeddedCliPrint(cli, "  can_send 0x100 0xAA 0xBB       (standard)");
        embeddedCliPrint(cli, "  can_send 0x12345 ext 0xAA 0xBB (extended)");
        return;
    }

    uint32_t canId = (uint32_t)strtoul(idStr, NULL, 0);
    bool extended = false;
    uint8_t data[8] = {0};
    uint8_t dlc = 0;
    uint8_t dataStartToken = 2;

    const char *tok2 = embeddedCliGetToken(args, 2);
    if (tok2 != NULL && strcmp(tok2, "ext") == 0) {
        extended = true;
        dataStartToken = 3;
    }

    if (!extended && canId > 0x7FF) {
        embeddedCliPrint(cli, "Hint: ID > 0x7FF, add 'ext' for extended frame");
        embeddedCliPrint(cli, "  can_send 0x12345 ext 0xAA");
        return;
    }

    for (uint8_t i = dataStartToken; dlc < 8; i++) {
        const char *valStr = embeddedCliGetToken(args, i);
        if (valStr == NULL) break;
        data[dlc++] = (uint8_t)strtoul(valStr, NULL, 0);
    }

    if (CAN_RawSend(canId, data, dlc, extended)) {
        snprintf(buffer, sizeof(buffer),
                 "--> TX OK [%s]: ID=0x%lX DLC=%u",
                 extended ? "EXT" : "STD",
                 (unsigned long)canId, dlc);
        embeddedCliPrint(cli, buffer);
    } else {
        embeddedCliPrint(cli, "--> TX FAILED");
    }
    embeddedCliPrint(cli, "");
}

/**
 * @brief CSP ping another node
 * Usage: csp_ping <dest_addr> [timeout_ms]
 * Example: csp_ping 2          (ping node 2, default 1000ms)
 *          csp_ping 2 500      (ping node 2, 500ms timeout)
 */
static void CMD_CspPing(EmbeddedCli *cli, char *args, void *context) {
    const char *addrStr = embeddedCliGetToken(args, 1);
    const char *timeStr = embeddedCliGetToken(args, 2);
    char buffer[128];

    if (addrStr == NULL) {
        embeddedCliPrint(cli, "Usage: csp_ping <dest_addr> [timeout_ms]");
        return;
    }

    uint8_t dest = (uint8_t)strtoul(addrStr, NULL, 0);
    uint32_t timeout = 1000;
    if (timeStr != NULL) {
        timeout = (uint32_t)strtoul(timeStr, NULL, 0);
    }

    snprintf(buffer, sizeof(buffer), "--> Pinging CSP node %u (timeout %lums)...", dest, (unsigned long)timeout);
    embeddedCliPrint(cli, buffer);

    int rtt = csp_ping(dest, timeout, 100, CSP_O_NONE);

    if (rtt >= 0) {
        snprintf(buffer, sizeof(buffer), "--> Reply from %u: time=%d ms", dest, rtt);
        embeddedCliPrint(cli, buffer);
    } else {
        snprintf(buffer, sizeof(buffer), "--> No reply from %u (timeout)", dest);
        embeddedCliPrint(cli, buffer);
    }
    embeddedCliPrint(cli, "");
}

/**
 * @brief Send CSP message to another node
 * Usage: csp_send <addr> <port> <string_data>
 * Example: csp_send 2 10 Hello
 */
static void CMD_CspSend(EmbeddedCli *cli, char *args, void *context) {
    const char *addrStr = embeddedCliGetToken(args, 1);
    const char *portStr = embeddedCliGetToken(args, 2);
    const char *dataStr = embeddedCliGetToken(args, 3);
    char buffer[128];

    if (addrStr == NULL || portStr == NULL || dataStr == NULL) {
        embeddedCliPrint(cli, "Usage: csp_send <addr> <port> <data>");
        embeddedCliPrint(cli, "Example: csp_send 2 10 Hello");
        return;
    }

    uint8_t dest = (uint8_t)strtoul(addrStr, NULL, 0);
    uint8_t port = (uint8_t)strtoul(portStr, NULL, 0);
    uint16_t len = strlen(dataStr);

    csp_conn_t *conn = csp_connect(CSP_PRIO_NORM, dest, port, 1000, CSP_O_NONE);
    if (conn == NULL) {
        embeddedCliPrint(cli, "--> Connection failed");
        return;
    }

    csp_packet_t *pkt = csp_buffer_get(len);
    if (pkt == NULL) {
        embeddedCliPrint(cli, "--> Buffer alloc failed");
        csp_close(conn);
        return;
    }

    memcpy(pkt->data, dataStr, len);
    pkt->length = len;

    if (csp_send(conn, pkt, 10000) == 0) {
        csp_buffer_free(pkt);
        embeddedCliPrint(cli, "--> Send failed");
    } else {
        snprintf(buffer, sizeof(buffer),
                 "--> Sent %u bytes to %u:%u", len, dest, port);
        embeddedCliPrint(cli, buffer);
    }

    csp_close(conn);
    embeddedCliPrint(cli, "");
}

/**
 * @brief Show CSP interface info
 * Usage: csp_info
 */
static void CMD_CspInfo(EmbeddedCli *cli, char *args, void *context) {
    char buffer[128];

    embeddedCliPrint(cli, "=== CSP Info ===");

    snprintf(buffer, sizeof(buffer), "  My Address: %u", CSP_MY_ADDRESS);
    embeddedCliPrint(cli, buffer);

    snprintf(buffer, sizeof(buffer), "  Free buffers: %u", csp_buffer_remaining());
    embeddedCliPrint(cli, buffer);

    /* Print interface list */
    embeddedCliPrint(cli, "  Interfaces:");
    csp_iflist_print();

    /* Print routing table */
    embeddedCliPrint(cli, "  Route table:");
    csp_rtable_print();

    embeddedCliPrint(cli, "================");
    embeddedCliPrint(cli, "");
}

static void print_param_row(EmbeddedCli *cli,
                            gs_param_table_instance_t *tinst,
                            const gs_param_table_row_t *row)
{
    char buf[128];
    uint8_t cnt = (row->array_size > 1) ? row->array_size : 1;

    for (uint8_t i = 0; i < cnt; i++) {
        uint16_t addr = row->addr + (uint16_t)(i * row->size);
        char suffix[8] = "";
        if (cnt > 1) snprintf(suffix, sizeof(suffix), "[%u]", i);

        switch (row->type) {
            case GS_PARAM_UINT8: {
                uint8_t v = 0;
                gs_param_get(tinst, addr, GS_PARAM_UINT8, &v, sizeof(v), 0);
                snprintf(buf, sizeof(buf), "  %-13s%-6s = %u", row->name, suffix, v);
                break;
            }
            case GS_PARAM_UINT16: {
                uint16_t v = 0;
                gs_param_get(tinst, addr, GS_PARAM_UINT16, &v, sizeof(v), 0);
                snprintf(buf, sizeof(buf), "  %-13s%-6s = %u", row->name, suffix, v);
                break;
            }
            case GS_PARAM_UINT32: {
                uint32_t v = 0;
                gs_param_get(tinst, addr, GS_PARAM_UINT32, &v, sizeof(v), 0);
                snprintf(buf, sizeof(buf), "  %-13s%-6s = %lu", row->name, suffix, (unsigned long)v);
                break;
            }
            case GS_PARAM_INT8: {
                int8_t v = 0;
                gs_param_get(tinst, addr, GS_PARAM_INT8, &v, sizeof(v), 0);
                snprintf(buf, sizeof(buf), "  %-13s%-6s = %d", row->name, suffix, (int)v);
                break;
            }
            case GS_PARAM_INT16: {
                int16_t v = 0;
                gs_param_get(tinst, addr, GS_PARAM_INT16, &v, sizeof(v), 0);
                snprintf(buf, sizeof(buf), "  %-13s%-6s = %d", row->name, suffix, (int)v);
                break;
            }
            case GS_PARAM_INT32: {
                int32_t v = 0;
                gs_param_get(tinst, addr, GS_PARAM_INT32, &v, sizeof(v), 0);
                snprintf(buf, sizeof(buf), "  %-13s%-6s = %ld", row->name, suffix, (long)v);
                break;
            }
            case GS_PARAM_FLOAT: {
                float v = 0.0f;
                gs_param_get(tinst, addr, GS_PARAM_FLOAT, &v, sizeof(v), 0);
                snprintf(buf, sizeof(buf), "  %-13s%-6s = %.4f", row->name, suffix, (double)v);
                break;
            }
            case GS_PARAM_BOOL: {
                uint8_t v = 0;
                gs_param_get(tinst, addr, GS_PARAM_BOOL, &v, sizeof(v), 0);
                snprintf(buf, sizeof(buf), "  %-13s%-6s = %s", row->name, suffix, v ? "true" : "false");
                break;
            }
            case GS_PARAM_STRING: {
                char strbuf[32] = {0};
                gs_param_get_string(tinst, addr, strbuf, sizeof(strbuf), 0);
                snprintf(buf, sizeof(buf), "  %-13s%-6s = \"%s\"", row->name, suffix, strbuf);
                break;
            }
            default:
                snprintf(buf, sizeof(buf), "  %-13s%-6s = (type %u @ 0x%04X)",
                         row->name, suffix, (unsigned)row->type, addr);
                break;
        }
        embeddedCliPrint(cli, buf);
    }
}

static void CMD_ParamShow(EmbeddedCli *cli, char *args, void *context) {
    const char *tidStr = embeddedCliGetToken(args, 1);
    char buf[80];

    if (tidStr == NULL) {
        embeddedCliPrint(cli, "Usage: param_show <table_id>");
        embeddedCliPrint(cli, "  0=Board  1=SysCfg  2=ManCtrl  3=Exp  4=Telemetry");
        return;
    }

    uint8_t tid = (uint8_t)strtoul(tidStr, NULL, 0);
    if (tid > 4) {
        embeddedCliPrint(cli, "Error: table_id must be 0-4");
        return;
    }

    gs_param_table_instance_t *tinst = &g_tables[tid];

    snprintf(buf, sizeof(buf), "=== Table %u: %s ===", tid, data_table_get_name(tid));
    embeddedCliPrint(cli, buf);
    snprintf(buf, sizeof(buf), "    Rows: %u | Mem: %u bytes | CRC: 0x%04X",
             tinst->row_count, tinst->memory_size,
             gs_param_table_checksum_be(tinst));
    embeddedCliPrint(cli, buf);
    embeddedCliPrint(cli, "---");

    for (unsigned int i = 0; i < tinst->row_count; i++) {
        print_param_row(cli, tinst, &tinst->rows[i]);
    }

    embeddedCliPrint(cli, "===================");
    embeddedCliPrint(cli, "");
}

/* ============================================================
 *  CMD_ParamSet
 *  Usage: param_set <table_id> <addr> <type> <value>
 *  type: u8 u16 u32 i8 i16 i32 f32 bool str
 *  eg:
 *    param_set 4 8   i16 250         -> BRD_TEMP = 250 (25.0°C)
 *    param_set 4 0   u32 1748000000  -> TIME_NOW = epoch
 *    param_set 1 121 u8  0           -> led_en = 0
 *    param_set 4 15  str NOMINAL     -> dev_status
 * ============================================================ */
static void CMD_ParamSet(EmbeddedCli *cli, char *args, void *context) {
    const char *tidStr  = embeddedCliGetToken(args, 1);
    const char *addrStr = embeddedCliGetToken(args, 2);
    const char *typeStr = embeddedCliGetToken(args, 3);
    const char *valStr  = embeddedCliGetToken(args, 4);
    char buf[128];

    if (!tidStr || !addrStr || !typeStr || !valStr) {
        embeddedCliPrint(cli, "Usage: param_set <table_id> <addr> <type> <value>");
        embeddedCliPrint(cli, "  type: u8 u16 u32 i8 i16 i32 f32 bool str");
        embeddedCliPrint(cli, "  Example: param_set 4 8 i16 250");
        return;
    }

    uint8_t  tid  = (uint8_t)strtoul(tidStr,  NULL, 0);
    uint16_t addr = (uint16_t)strtoul(addrStr, NULL, 0);

    if (tid > 4) {
        embeddedCliPrint(cli, "Error: table_id must be 0-4");
        return;
    }

    gs_param_table_instance_t *tinst = &g_tables[tid];
    gs_error_t err = GS_ERROR_ARG;

    if (strcmp(typeStr, "u8") == 0) {
        uint8_t v = (uint8_t)strtoul(valStr, NULL, 0);
        err = gs_param_set(tinst, addr, GS_PARAM_UINT8, &v, sizeof(v), 0);
    } else if (strcmp(typeStr, "u16") == 0) {
        uint16_t v = (uint16_t)strtoul(valStr, NULL, 0);
        err = gs_param_set(tinst, addr, GS_PARAM_UINT16, &v, sizeof(v), 0);
    } else if (strcmp(typeStr, "u32") == 0) {
        uint32_t v = (uint32_t)strtoul(valStr, NULL, 0);
        err = gs_param_set(tinst, addr, GS_PARAM_UINT32, &v, sizeof(v), 0);
    } else if (strcmp(typeStr, "i8") == 0) {
        int8_t v = (int8_t)atoi(valStr);
        err = gs_param_set(tinst, addr, GS_PARAM_INT8, &v, sizeof(v), 0);
    } else if (strcmp(typeStr, "i16") == 0) {
        int16_t v = (int16_t)atoi(valStr);
        err = gs_param_set(tinst, addr, GS_PARAM_INT16, &v, sizeof(v), 0);
    } else if (strcmp(typeStr, "i32") == 0) {
        int32_t v = (int32_t)atol(valStr);
        err = gs_param_set(tinst, addr, GS_PARAM_INT32, &v, sizeof(v), 0);
    } else if (strcmp(typeStr, "f32") == 0) {
        float v = (float)atof(valStr);
        err = gs_param_set(tinst, addr, GS_PARAM_FLOAT, &v, sizeof(v), 0);
    } else if (strcmp(typeStr, "bool") == 0) {
        uint8_t v = (atoi(valStr) != 0) ? 1 : 0;
        err = gs_param_set(tinst, addr, GS_PARAM_BOOL, &v, sizeof(v), 0);
    } else if (strcmp(typeStr, "str") == 0) {
        err = gs_param_set_string(tinst, addr, valStr, 0);
    } else {
        snprintf(buf, sizeof(buf), "Unknown type '%s'. Use: u8 u16 u32 i8 i16 i32 f32 bool str", typeStr);
        embeddedCliPrint(cli, buf);
        return;
    }

    if (err == GS_OK) {
        snprintf(buf, sizeof(buf), "--> OK: T%u[0x%04X] = %s (%s)", tid, addr, valStr, typeStr);
    } else {
        snprintf(buf, sizeof(buf), "--> FAIL: err=%d (addr out of range?)", (int)err);
    }
    embeddedCliPrint(cli, buf);
    embeddedCliPrint(cli, "");
}


/*
 * POWER ON/OFF AND STATUS
 */
static void CMD_PowerSOM_ON(EmbeddedCli *cli, char *args, void *context) {
    bsp_power_som_on();
    embeddedCliPrint(cli, "CMD_PowerSOM_ON");
    embeddedCliPrint(cli, "");
}

static void CMD_PowerSOM_OFF(EmbeddedCli *cli, char *args, void *context) {
    bsp_power_som_off();
    embeddedCliPrint(cli, "CMD_PowerSOM_OFF");
    embeddedCliPrint(cli, "");
}

static void CMD_PowerSOM_Get(EmbeddedCli *cli, char *args, void *context) {
    if (bsp_power_som_status())    
        embeddedCliPrint(cli, "Power SOM is ON");
    else
        embeddedCliPrint(cli, "Power SOM is OFF");
    embeddedCliPrint(cli, "");
}


static void CMD_PowerBuckPeri_ON(EmbeddedCli *cli, char *args, void *context) {
    bsp_power_buck_peri_on();
    embeddedCliPrint(cli, "CMD_PowerBuckPeri_ON");
    embeddedCliPrint(cli, "");
}

static void CMD_PowerBuckPeri_OFF(EmbeddedCli *cli, char *args, void *context) {
    bsp_power_buck_peri_off();
    embeddedCliPrint(cli, "CMD_PowerBuckPeri_OFF");
    embeddedCliPrint(cli, "");
}

static void CMD_PowerBuckPeri_Get(EmbeddedCli *cli, char *args, void *context) {
    if (bsp_power_buck_peri_status())    
        embeddedCliPrint(cli, "Power Buck Peripherals is ON");
    else
        embeddedCliPrint(cli, "Power Buck Peripherals is OFF");
    embeddedCliPrint(cli, "");
}


static void CMD_PowerTec_ON(EmbeddedCli *cli, char *args, void *context) {
    bsp_power_tec_on();
    embeddedCliPrint(cli, "CMD_PowerTec_ON");
    embeddedCliPrint(cli, "");
}

static void CMD_PowerTec_OFF(EmbeddedCli *cli, char *args, void *context) {
    bsp_power_tec_off();
    embeddedCliPrint(cli, "CMD_PowerTec_OFF");
    embeddedCliPrint(cli, "");
}

static void CMD_PowerTec_Get(EmbeddedCli *cli, char *args, void *context) {
    if (bsp_power_tec_status())    
        embeddedCliPrint(cli, "Power Tec is ON");
    else
        embeddedCliPrint(cli, "Power Tec is OFF");
    embeddedCliPrint(cli, "");
}


static void CMD_PowerHD4_ON(EmbeddedCli *cli, char *args, void *context) {
    bsp_power_hd4_on();
    embeddedCliPrint(cli, "CMD_PowerHD4_ON");
    embeddedCliPrint(cli, "");
}

static void CMD_PowerHD4_OFF(EmbeddedCli *cli, char *args, void *context) {
    bsp_power_hd4_off();
    embeddedCliPrint(cli, "CMD_PowerHD4_OFF");
    embeddedCliPrint(cli, "");
}

static void CMD_PowerHD4_Get(EmbeddedCli *cli, char *args, void *context) {
    if (bsp_power_hd4_status())    
        embeddedCliPrint(cli, "Power HD4 is ON");
    else
        embeddedCliPrint(cli, "Power HD4 is OFF");
    embeddedCliPrint(cli, "");
}


static void CMD_PowerSolenoid_ON(EmbeddedCli *cli, char *args, void *context) {
    bsp_power_solenoid_on();
    embeddedCliPrint(cli, "CMD_PowerSolenoid_ON");
    embeddedCliPrint(cli, "");
}

static void CMD_PowerSolenoid_OFF(EmbeddedCli *cli, char *args, void *context) {
    bsp_power_solenoid_off();
    embeddedCliPrint(cli, "CMD_PowerSolenoid_OFF");
    embeddedCliPrint(cli, "");
}

static void CMD_PowerSolenoid_Get(EmbeddedCli *cli, char *args, void *context) {
    if (bsp_power_solenoid_status())    
        embeddedCliPrint(cli, "Power Solenoid is ON");
    else
        embeddedCliPrint(cli, "Power Solenoid is OFF");
    embeddedCliPrint(cli, "");
}


static void CMD_PowerLP_ON(EmbeddedCli *cli, char *args, void *context) {
    bsp_power_lp_on();
    embeddedCliPrint(cli, "CMD_PowerLP_ON");
    embeddedCliPrint(cli, "");
}

static void CMD_PowerLP_OFF(EmbeddedCli *cli, char *args, void *context) {
    bsp_power_lp_off();
    embeddedCliPrint(cli, "CMD_PowerLP_OFF");
    embeddedCliPrint(cli, "");
}

static void CMD_PowerLP_Get(EmbeddedCli *cli, char *args, void *context) {
    if (bsp_power_lp_status())    
        embeddedCliPrint(cli, "Power LP is ON");
    else
        embeddedCliPrint(cli, "Power LP is OFF");
    embeddedCliPrint(cli, "");
}

static void CMD_PowerHeater_ON(EmbeddedCli *cli, char *args, void *context) {
    bsp_power_heater_on();
    embeddedCliPrint(cli, "CMD_PowerHeater_ON");
    embeddedCliPrint(cli, "");
}

static void CMD_PowerHeater_OFF(EmbeddedCli *cli, char *args, void *context) {
    bsp_power_heater_off();
    embeddedCliPrint(cli, "CMD_PowerHeater_OFF");
    embeddedCliPrint(cli, "");
}

static void CMD_PowerHeater_Get(EmbeddedCli *cli, char *args, void *context) {
    if (bsp_power_heater_status())    
        embeddedCliPrint(cli, "Power Heater is ON");
    else
        embeddedCliPrint(cli, "Power Heater is OFF");
    embeddedCliPrint(cli, "");
}  


static void CMD_PowerAll_ON(EmbeddedCli *cli, char *args, void *context) {
    power_on_all();
    embeddedCliPrint(cli, "CMD_PowerAll_ON");
    embeddedCliPrint(cli, "");
}

static void CMD_PowerAll_OFF(EmbeddedCli *cli, char *args, void *context) {
    power_off_all();
    embeddedCliPrint(cli, "CMD_PowerAll_OFF");
    embeddedCliPrint(cli, "");
}

static void CMD_PowerAll_Get(EmbeddedCli *cli, char *args, void *context) {
    if (bsp_power_som_status())    
        embeddedCliPrint(cli, "Power SOM is ON");
    else
        embeddedCliPrint(cli, "Power SOM is OFF");
    
    if (bsp_power_buck_peri_status())    
        embeddedCliPrint(cli, "Power Buck Peripherals is ON");
    else
        embeddedCliPrint(cli, "Power Buck Peripherals is OFF");
    
    if (bsp_power_tec_status())    
        embeddedCliPrint(cli, "Power Tec is ON");
    else
        embeddedCliPrint(cli, "Power Tec is OFF");
    
    if (bsp_power_tec_status())    
        embeddedCliPrint(cli, "Power Tec is ON");
    else
        embeddedCliPrint(cli, "Power Tec is OFF");

    if (bsp_power_hd4_status())    
        embeddedCliPrint(cli, "Power HD4 is ON");
    else
        embeddedCliPrint(cli, "Power HD4 is OFF");

    if (bsp_power_solenoid_status())    
        embeddedCliPrint(cli, "Power Solenoid is ON");
    else
        embeddedCliPrint(cli, "Power Solenoid is OFF");
    
    if (bsp_power_lp_status())    
        embeddedCliPrint(cli, "Power LP is ON");
    else
        embeddedCliPrint(cli, "Power LP is OFF");
    
    if (bsp_power_heater_status())    
        embeddedCliPrint(cli, "Power Heater is ON");
    else
    {
        embeddedCliPrint(cli, "Power Heater is OFF");
    }
    embeddedCliPrint(cli, "");
} 



////--------IDEA OPTIMIZE POWER FUNCTION-----------
//
//static void CMD_PowerGuide_ON(EmbeddedCli *cli, char *args, void *context)
//{
//    embeddedCliPrint(cli, "power_on + parameters: <som> <buck_peri> <tec> <hd4> <solenoid> <lp> <heater> <all>");
//}
//
//static void CMD_PowerGuide_OFF(EmbeddedCli *cli, char *args, void *context)
//{
//    embeddedCliPrint(cli, "power_on + parameters: <som> <buck_peri> <tec> <hd4> <solenoid> <lp> <heater> <all>");
//} 
//
//static void CMD_Power_ON(EmbeddedCli *cli, char *args, void *context)
//{
//    int i = 1;
//    
//} 


static void CMD_LED_Set (EmbeddedCli *cli, char *args, void *context)
{
    bsp_led_set();
    embeddedCliPrint(cli, "LED_ON");
    embeddedCliPrint(cli, "");
}
static void CMD_LED_Reset (EmbeddedCli *cli, char *args, void *context)
{
    bsp_led_reset();
    embeddedCliPrint(cli, "LED_OFF");
    embeddedCliPrint(cli, "");
}

static void CMD_HEATER_SetDuty (EmbeddedCli *cli, char *args, void *context)
{
    (void)context;

    const char *HTR_CH   = embeddedCliGetToken(args, 1);
    const char *HTR_Duty = embeddedCliGetToken(args, 2);
    char buf[128];

    if (HTR_CH == NULL || HTR_Duty == NULL)
    {
        embeddedCliPrint(cli, "Usage  : heater_set <channel 1-8> <duty 0-100>");
        embeddedCliPrint(cli, "Example: heater_set 1 80");
        return;
    }

    char *endptr;

    uint32_t ch = strtoul(HTR_CH, &endptr, 10);
    if (*endptr != '\0')
    {
        embeddedCliPrint(cli, "Error: Channel must be a number (1-8)");
        return;
    }

    uint32_t duty = strtoul(HTR_Duty, &endptr, 10);
    if (*endptr != '\0')
    {
        embeddedCliPrint(cli, "Error: Duty must be a number (0-100)");
        return;
    }

    if (ch < 1 || ch > 8)
    {
        embeddedCliPrint(cli, "Error: Channel out of range (1-8)");
        return;
    }

    if (duty > 100)
    {
        embeddedCliPrint(cli, "Error: Duty out of range (0-100%)");
        return;
    }

    bsp_heater_set_duty((uint8_t)(ch - 1), (uint8_t)duty);

    snprintf(buf, sizeof(buf), "--> OK: Heater[%lu] Duty = %lu%%", ch, duty);

    embeddedCliPrint(cli, buf);
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


