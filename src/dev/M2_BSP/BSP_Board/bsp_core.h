#ifndef  __BSP_CORE_H__
#define __BSP_CORE_H__

#include "board.h"
#include "do.h"
#include "spi_io.h"
extern spi_io_t onboard_adc_spi;


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

void BSP_board_init(void);

#endif /* __BSP_CORE_H__ */