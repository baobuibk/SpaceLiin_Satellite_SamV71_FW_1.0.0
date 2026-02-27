#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "os_hal.h"
#include "samv71q21b.h"
#include "spi_io.h"
#include "define.h"
//#include "peripheral/pio/plib_pio.h"
#define SPI_TIMEOUT_MS 1000
#define SPI_MAX_BUS_NUMBER 2

static spi_registers_t * const spi_periph[SPI_MAX_BUS_NUMBER + 1] = {
    NULL,
    SPI0_REGS,
    SPI1_REGS
};

static uint32_t const spi_clock_en[SPI_MAX_BUS_NUMBER + 1] = {
    0,
    ID_SPI0,
    ID_SPI1
};

static void spi_enable_clock(uint32_t spiNum)
{
    if (spiNum == 0 || spiNum > SPI_MAX_BUS_NUMBER)
        return;

    uint32_t id = spi_clock_en[spiNum];

    if (id < 32U)
        PMC_REGS->PMC_PCER0 = (1u << id);
    else
        PMC_REGS->PMC_PCER1 = (1u << (id - 32U));
}




uint32_t spi_io_transfer_sync(spi_io_t *me,
                              const uint8_t *pui8TxBuff,
                              uint8_t *pui8RxBuff,
                              uint32_t ui32Length)
{
    if (!me) return ERROR_INVALID_PARAM;
    if (ui32Length == 0) return ERROR_OK;

    spi_registers_t *spi = spi_periph[me->ui32SpiPort];
    if (!spi) return ERROR_INVALID_PARAM;

    // (Tuỳ chọn) đảm bảo SPI enabled:
    // if (!(spi->SPI_SR & SPI_SR_SPIENS_Msk)) return ERROR_SPI_DISABLED;

    for (uint32_t i = 0; i < ui32Length; i++)
    {
        // Wait Transmit Data Register Empty (sẵn sàng nhận dữ liệu mới)
        if (ERROR_OK != os_reg_wait_flag_blocking(&spi->SPI_SR, SPI_SR_TDRE_Msk, 100))
            return ERROR_TIMEOUT;

        uint8_t tx = pui8TxBuff ? pui8TxBuff[i] : 0xFF;

        // Ghi đúng field TD (và có thể OR PCS/LASTXFER nếu bạn dùng HW CS)
        uint32_t tdr = SPI_TDR_TD(tx);

        // Nếu cần nhả CS ở byte cuối (khi dùng CSAAT/HW CS), bạn có thể:
        // if (i == (ui32Length - 1)) tdr |= SPI_TDR_LASTXFER_Msk;

        spi->SPI_TDR = tdr;

        // Wait Receive Data Register Full
        if (ERROR_OK != os_reg_wait_flag_blocking(&spi->SPI_SR, SPI_SR_RDRF_Msk, 100))
            return ERROR_TIMEOUT;

        // Check error flags (cleared on read)
        uint32_t sr = spi->SPI_SR;
        if (sr & (SPI_SR_OVRES_Msk | SPI_SR_MODF_Msk))
        {
            (void)spi->SPI_RDR; // clear RDRF path if needed
            return ERROR_SPI;
        }

        uint8_t rx = (uint8_t)(spi->SPI_RDR & SPI_RDR_RD_Msk);

        if (pui8RxBuff) pui8RxBuff[i] = rx;
    }

    // Flush: đợi truyền xong hẳn
    if (ERROR_OK != os_reg_wait_flag_blocking(&spi->SPI_SR, SPI_SR_TXEMPTY_Msk, 100))
        return ERROR_TIMEOUT;

    return ERROR_OK;
}


/******************************************************************************/
/*************************** Synchronous Functions ****************************/
/******************************************************************************/

uint32_t spi_io_set_mode(spi_io_t *me, uint8_t spi_mode)
{
    if (me == NULL)
        return ERROR_INVALID_PARAM;

    // Giữ đúng logic bạn đang dùng: port==0 là invalid.
    // Nếu hệ bạn dùng index từ 0..SPI_MAX_BUS_NUMBER-1 thì đổi điều kiện này.
    if (me->ui32SpiPort == 0 || me->ui32SpiPort > SPI_MAX_BUS_NUMBER)
        return ERROR_INVALID_PARAM;

    spi_registers_t *spi = spi_periph[me->ui32SpiPort];
    if (spi == NULL)
        return ERROR_INVALID_PARAM;

    // Chờ SPI rảnh (TX regs empty) trước khi đổi mode
    if (ERROR_OK != os_reg_wait_flag_blocking(&spi->SPI_SR, SPI_SR_TXEMPTY_Msk, 100))
        return ERROR_TIMEOUT;

    // Disable SPI
    spi->SPI_CR = SPI_CR_SPIDIS_Msk;

    // (Khuyến nghị) đợi SPI disable thật
    if (ERROR_OK != os_reg_wait_clear_flag_blocking(&spi->SPI_SR, SPI_SR_SPIENS_Msk, 100))
    {
        return ERROR_TIMEOUT;
    }

    // Chỉ cho phép 0..3
    spi_mode &= 0x03u;

    // Chọn CSR index (mặc định NPCS0). Nếu me có thông tin NPCS, dùng me->npcs.
    const uint32_t cs_idx = 0;

    uint32_t csr = spi->SPI_CSR[cs_idx];

    // Clear CPOL & NCPHA
    csr &= ~(SPI_CSR_CPOL_Msk | SPI_CSR_NCPHA_Msk);

    /*
     * Mapping SPI mode (0..3) sang (CPOL, NCPHA) theo SAMV71:
     * Mode 0: CPOL=0, NCPHA=1
     * Mode 1: CPOL=0, NCPHA=0
     * Mode 2: CPOL=1, NCPHA=1
     * Mode 3: CPOL=1, NCPHA=0
     *
     * => CPOL = (mode & 0x2)
     * => NCPHA = !(mode & 0x1)
     */
    if (spi_mode & 0x02u)
        csr |= SPI_CSR_CPOL_Msk;

    if ((spi_mode & 0x01u) == 0u)
        csr |= SPI_CSR_NCPHA_Msk;

    spi->SPI_CSR[cs_idx] = csr;

    // Enable SPI
    spi->SPI_CR = SPI_CR_SPIEN_Msk;

    return ERROR_OK;
}

uint32_t spi_io_read_sync(spi_io_t *me, uint8_t *pui8RxBuff, uint32_t ui32Length)
{
    if (!me) return ERROR_INVALID_PARAM;
    if (!pui8RxBuff) return ERROR_INVALID_PARAM;
    if (ui32Length == 0) return ERROR_OK;

    // Read = transmit dummy (NULL Tx => transfer_sync sẽ gửi 0xFF)
    return spi_io_transfer_sync(me, NULL, pui8RxBuff, ui32Length);
}

uint32_t spi_io_write_sync(spi_io_t *me, const uint8_t *pui8TxBuff, uint32_t ui32Length)
{
    if (!me) return ERROR_INVALID_PARAM;
    if (!pui8TxBuff) return ERROR_INVALID_PARAM;
    if (ui32Length == 0) return ERROR_OK;

    // Write = transmit buffer, Rx không cần lưu (transfer_sync vẫn đọc RDR để clear RDRF)
    return spi_io_transfer_sync(me, pui8TxBuff, NULL, ui32Length);
}


