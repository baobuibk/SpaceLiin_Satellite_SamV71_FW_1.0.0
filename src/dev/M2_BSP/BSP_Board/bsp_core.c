#include "bsp_core.h"
#include "board_v71_iss.h"

#include "board.h"


spi_io_t onboard_adc_spi =
{
    .ui32SpiPort = 1,
};
do_t laser_dac_cs =
{
    .port = LASER_SPI_nCS1_PORT,
    .pin  = LASER_SPI_nCS1_PIN,
    .bStatus = false,
};

do_t laser_dac_latch =
{
    .port = LASER_DAC_LATCH_PORT,
    .pin  = LASER_DAC_LATCH_PIN,
    .bStatus = false,
};
do_t laser_sw_int_cs =
{
    .port = LASER_SW_INT_CS_PORT,
    .pin  = LASER_SW_INT_CS_PIN,
    .bStatus = false,
};
do_t laser_sw_ext_cs =
{
    .port = LASER_SW_EXT_CS_PORT,
    .pin  = LASER_SW_EXT_CS_PIN,
    .bStatus = false,
};