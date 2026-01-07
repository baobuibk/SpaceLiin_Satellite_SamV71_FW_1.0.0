/*
 * cli_setup.c
 *
 *  Created on: Feb 27, 2025
 *      Author: CAO HIEU
 */

#include "cli_setup.h"
#include "M2_BSP/UART/uart_irq.h"
//#include "board.h"
#include "M1_SysApp/EmbeddedCli/CLI_Command/cli_command.h"

// CLI buffer
/*************************************************
 *           CLI Static Buffer Define            *
 *************************************************/

#define UART2_CLI_BUFFER_SIZE 3072
static  CLI_UINT uart2_cliStaticBuffer[BYTES_TO_CLI_UINTS(UART2_CLI_BUFFER_SIZE)];
#define NET_CLI_BUFFER_SIZE   3072
static  CLI_UINT net_cliStaticBuffer[BYTES_TO_CLI_UINTS(NET_CLI_BUFFER_SIZE)];

/*************************************************
 *             ----------------------            *
 *************************************************/
static CliWriteByte_t pf_Net_WriteChar = NULL;

static EmbeddedCli *cli_uart2;
static EmbeddedCli *cli_net;

// Bool to disable the interrupts, if CLI is not yet ready.
static _Bool cliIsReady = false;

/*************************************************
 *          Tx Transmit CLI Byte Buffer          *
 *************************************************/

static void writeCharToCli_UART2(EmbeddedCli *embeddedCli, char c) {
    uint8_t c_to_send = c;
    UART2_WriteByte(c_to_send);
}

static void writeCharToCli_Net(EmbeddedCli *embeddedCli, char c) {
    (void)embeddedCli;
    if (pf_Net_WriteChar != NULL) {
        pf_Net_WriteChar(c); 
    }
}
//Call before FREERTOS be initialized
//Call After UART Driver Init (or Peripheral use CLI)

Std_ReturnType SystemCLI_Init() {
    /* ------------------------------------------------ */
    /* 1. SETUP & INIT UART CLI                         */
    /* ------------------------------------------------ */
    EmbeddedCliConfig *config = embeddedCliDefaultConfig(); // Lấy config mặc định

    config->cliBuffer = uart2_cliStaticBuffer;
    config->cliBufferSize = UART2_CLI_BUFFER_SIZE;
    config->rxBufferSize = CLI_RX_BUFFER_SIZE;
    config->cmdBufferSize = CLI_CMD_BUFFER_SIZE;
    config->historyBufferSize = CLI_HISTORY_SIZE;
    config->maxBindingCount = CLI_MAX_BINDING_COUNT;
    config->enableAutoComplete = CLI_AUTO_COMPLETE;
    config->invitation = CLI_INITATION_UART2; // "DEBUG@MCU"
    config->staticBindings = getCliStaticBindings();
    config->staticBindingCount = getCliStaticBindingCount();

    cli_uart2 = embeddedCliNew(config);
    if (cli_uart2 == NULL) {
        return E_ERROR;
    }
    cli_uart2->writeChar = writeCharToCli_UART2;


    /* ------------------------------------------------ */
    /* 2. SETUP & INIT NET CLI                          */
    /* ------------------------------------------------ */
    config = embeddedCliDefaultConfig(); 

    config->cliBuffer = net_cliStaticBuffer;
    config->cliBufferSize = NET_CLI_BUFFER_SIZE;
    config->rxBufferSize = CLI_RX_BUFFER_SIZE;
    config->cmdBufferSize = CLI_CMD_BUFFER_SIZE;
    config->historyBufferSize = CLI_HISTORY_SIZE;
    config->maxBindingCount = CLI_MAX_BINDING_COUNT;
    config->enableAutoComplete = 0;
    config->invitation = CLI_INITATION_NET; // "USB@MCU"
    config->staticBindings = getCliStaticBindings();
    config->staticBindingCount = getCliStaticBindingCount();

    cli_net = embeddedCliNew(config);
    if (cli_net == NULL) {
        return E_ERROR;
    }
    cli_net->writeChar = writeCharToCli_Net;

    // CLI has now been initialized
    cliIsReady = true;

    return E_OK;
}

/*************************************************
 *             Get CLI Pointers                  *
 *************************************************/
EmbeddedCli *get_UART2_CliPointer(void) {
    return cli_uart2;
}

EmbeddedCli *get_Net_CliPointer(void) {
    return cli_net;
}

void Cli_Net_Register_Tx(CliWriteByte_t tx_cb) {
    pf_Net_WriteChar = tx_cb;
}