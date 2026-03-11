#ifndef  __BSP_CORE_H__
#define __BSP_CORE_H__

#include "board.h"
#include "do.h"
#include "spi_io.h"
#include "i2c_io.h"

extern spi_io_t onboard_adc_spi;

extern i2c_io_t sol_expander_i2c;

extern do_t laser_dac_cs ;

extern do_t laser_dac_latch;

extern do_t laser_sw_int_cs ;
extern do_t laser_sw_ext_cs ;
 
extern do_t power_som;
extern do_t power_buck_peri;
extern do_t power_tec;
extern do_t power_hd4;
extern do_t power_solenoid;
extern do_t power_lp;
extern do_t power_heater;

extern do_t status_led;

extern spi_io_t tec_usart_spi;
extern do_t tec_1_cs;
extern do_t tec_1_sw;
extern do_t tec_2_cs;
extern do_t tec_2_sw;
extern do_t tec_3_cs;
extern do_t tec_3_sw;
extern do_t tec_4_cs;
extern do_t tec_4_sw;

extern i2c_io_t bmp390_connector;
extern do_t bmp390_connector_sensor5;
extern i2c_io_t bmp390_onboard;

void BSP_board_init(void);

#endif /* __BSP_CORE_H__ */
