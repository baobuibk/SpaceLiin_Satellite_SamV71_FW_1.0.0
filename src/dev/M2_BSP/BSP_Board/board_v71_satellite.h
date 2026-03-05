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

#define HTR_PWM_1_PORT         1  //PA27
#define HTR_PWM_1_PIN          27   //PA27

#define HTR_PWM_2_PORT         1  //PA00
#define HTR_PWM_2_PIN          0   //PA00

#define HTR_PWM_3_PORT         1  //PA01
#define HTR_PWM_3_PIN          1   //PA01

#define HTR_PWM_4_PORT         1  //PA26
#define HTR_PWM_4_PIN          26   //PA26

#define HTR_PWM_5_PORT         3  //PC8
#define HTR_PWM_5_PIN          8   //PC8

#define HTR_PWM_6_PORT         3  //PC23
#define HTR_PWM_6_PIN          23   //PC23

#define HTR_PWM_7_PORT         3  //PC5
#define HTR_PWM_7_PIN          5   //PC5

#define HTR_PWM_8_PORT         3  //PC6
#define HTR_PWM_8_PIN          6   //PC6



#endif	/* BOARD_V71_SATELLITE_H */

