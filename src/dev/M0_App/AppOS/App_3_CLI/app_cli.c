/************************************************
 *  @file     : app_cli.c
 *  @date     : January 2026
 *  @author   : CAO HIEU
 *  @version  : 1.0.0
 *-----------------------------------------------
 *  Description :
 *    [-]
 ************************************************/

#include "app_cli.h"

#include "M1_SysApp/EmbeddedCLI/CLI_Setup/cli_setup.h"
#include "M2_BSP/UART/uart_irq.h"
#include "M5_Utils/DateTime/date_time.h"
#include "M1_SysApp/EmbeddedCLI/CLI_Auth/simple_shield.h"

ShieldInstance_t auth_uart;

static void writeChar_auth_UART2(char c) {
    uint8_t c_to_send = c;
    UART2_WriteByte(c);
}

static void CLI_Handle_Thread(void)
{
    ShieldAuthState_t auth_state;

    auth_state = Shield_GetState(&auth_uart);
    if(auth_state == AUTH_ADMIN || auth_state == AUTH_USER)
    {
     	if(auth_uart.initreset == 1)
        {
            embeddedCliPrint(get_UART2_CliPointer(), "");
            auth_uart.initreset = 0;
     	}
            embeddedCliProcess(get_UART2_CliPointer());
    	}
}

static void UART2_RX_Thread(void)
{
    int c;
    while ((c = UART2_ReadByte()) != -1)
    {
        // Mode NORMAL: CLI
      	ShieldAuthState_t auth_state = Shield_GetState(&auth_uart);
       	if(auth_state == AUTH_ADMIN || auth_state == AUTH_USER)
        {
       		Shield_ResetTimer(&auth_uart);
            embeddedCliReceiveChar(get_UART2_CliPointer(), (char)c);
            embeddedCliProcess(get_UART2_CliPointer());
        }
        else
        {
            Shield_ReceiveChar(&auth_uart, (char)c);
        }
    }

}

/*-----------------------------------------------------------------*/
const osThreadAttr_t cli_attr = {
        .name = "CLI",
        .stack_size = configMINIMAL_STACK_SIZE * 10,
        .priority = configMAX_PRIORITIES - 3
};

void App_CLITask(void *param) {
    Shield_Init(&auth_uart, writeChar_auth_UART2);
    
    for(;;) {
 
        UART2_RX_Thread();
        CLI_Handle_Thread();
        osThreadFeed();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}