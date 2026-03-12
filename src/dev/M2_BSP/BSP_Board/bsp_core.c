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

do_t power_som = 
{
    .port = POWER_SOM_PORT, 
    .pin = POWER_SOM_PIN, 
    .bStatus = false
}; 

do_t power_buck_peri = 
{
    .port = POWER_PERI_PORT, 
    .pin = POWER_PERI_PIN, 
    .bStatus = false
}; 

do_t power_tec = 
{
    .port = POWER_TEC_PORT, 
    .pin = POWER_TEC_PIN, 
    .bStatus = false
}; 

do_t power_hd4 = 
{
    .port = POWER_HD4_PORT, 
    .pin = POWER_HD4_PIN, 
    .bStatus = false
}; 

do_t power_solenoid = 
{
    .port = POWER_SLN_PORT, 
    .pin = POWER_SLN_PIN, 
    .bStatus = false
}; 

do_t power_lp = 
{
    .port = POWER_LP_PORT, 
    .pin = POWER_LP_PIN, 
    .bStatus = false
}; 

do_t power_heater = 
{
    .port = POWER_HEATER_PORT, 
    .pin = POWER_HEATER_PIN, 
    .bStatus = false
}; 


do_t status_led = 
{
    .port = STATUS_LED_PORT,
    .pin = STATUS_LED_PIN,
    .bStatus = false,
};


/*
 * FOR BMP390 onboard and connnector
*/
i2c_io_t i2c0 =
{
    .ui32I2cPort = 0,
};

do_t bmp390_ext_sensor5 = 
{
    .port= SENSOR5_EN_PORT,
    .pin = SENSOR5_EN_PIN,
    .bStatus = false,
};

i2c_io_t i2c1 =
{
    .ui32I2cPort = 1,
};

i2c_io_t sol_expander_i2c = 
{
  .ui32I2cPort = 2  
};

spi_io_t tec_usart_spi =
{
    .ui32SpiPort = 2,
};

do_t tec_1_cs =
{
    .port = TEC_1_CS_PORT,
    .pin = TEC_1_CS_PIN,
    .bStatus = true,
};

do_t tec_1_sw =
{
    .port = TEC_1_SW_PORT,
    .pin = TEC_1_SW_PIN,
    .bStatus = false,
};

do_t tec_2_cs =
{
    .port = TEC_2_CS_PORT,
    .pin = TEC_2_CS_PIN,
    .bStatus = true,
};

do_t tec_2_sw =
{
    .port = TEC_2_SW_PORT,
    .pin = TEC_2_SW_PIN,
    .bStatus = false,
};

do_t tec_3_cs =
{
    .port = TEC_3_CS_PORT,
    .pin = TEC_3_CS_PIN,
    .bStatus = true,
};

do_t tec_3_sw =
{
    .port = TEC_3_SW_PORT,
    .pin = TEC_3_SW_PIN,
    .bStatus = false,
};

do_t tec_4_cs =
{
    .port = TEC_4_CS_PORT,
    .pin = TEC_4_CS_PIN,
    .bStatus = true,
};

do_t tec_4_sw =
{
    .port = TEC_4_SW_PORT,
    .pin = TEC_4_SW_PIN,
    .bStatus = false,
};