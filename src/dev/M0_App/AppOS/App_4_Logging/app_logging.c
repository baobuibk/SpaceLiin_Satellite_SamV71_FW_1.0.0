/************************************************
 *  @file     : app_logging.c
 *  @date     : January 2026
 *  @author   : CAO HIEU
 *  @version  : 1.0.0
 *-----------------------------------------------
 *  Description :
 *    [-]
 ************************************************/

#include "M0_App/OS/os.h"
#include "M1_SysApp/xlog/xlog.h"
#include "app_logging.h"
#include "stream_buffer.h"
#include "M2_BSP/UART/uart_irq.h"

const osThreadAttr_t log_attr = {
        .name = "Log_Task",
        .stack_size = 256,
        .priority = configMAX_PRIORITIES - 4
};

void App_LogTask(void *argument) {
    uint8_t tx_buf[64];
    size_t received_bytes;

    for (;;) {
        received_bytes = xStreamBufferReceive(xLogStreamBuffer, tx_buf, sizeof(tx_buf), pdMS_TO_TICKS(100));
        osThreadFeed();
        if (received_bytes > 0) {
            for(int i=0; i<received_bytes; i++) {
                 UART2_WriteByte(tx_buf[i]);
            }
        }
    }
}