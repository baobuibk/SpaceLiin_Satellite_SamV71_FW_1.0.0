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

void BSP_board_init(void);

#endif /* __BSP_CORE_H__ */