/* 
 * File:   board_v71_satellite.h
 * Author: HTSANG
 *
 * Created on February 27, 2026, 11:24 AM
 */

#ifndef BOARD_V71_SATELLITE_H
#define	BOARD_V71_SATELLITE_H

#include "samv71q21b.h"

#define LASER_DAC_LATCH_PORT    3    //PC16
#define LASER_DAC_LATCH_PIN     16   //PC16 

#define LASER_SPI_nCS1_PORT    1    //PORTA
#define LASER_SPI_nCS1_PIN     31   //PA31   

#define LASER_SW_INT_CS_PORT    3   //PC07  
#define LASER_SW_INT_CS_PIN     7   //PC07  

#define LASER_SW_EXT_CS_PORT    1   //PA16  
#define LASER_SW_EXT_CS_PIN     16   //PA16  

#define STATUS_LED_PORT         2   //PB12
#define STATUS_LED_PIN          12   //PB12

#endif	/* BOARD_V71_SATELLITE_H */

