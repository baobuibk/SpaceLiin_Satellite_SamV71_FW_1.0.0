/* 
 * File:   board_v71_satellite.h
 * Author: HTSANG
 *
 * Created on February 27, 2026, 11:24 AM
 */

#ifndef BOARD_V71_SATELLITE_H
#define	BOARD_V71_SATELLITE_H

#include "samv71q21b.h"

#define POWER_SOM_PORT          4  //PD5
#define POWER_SOM_PIN           5   //PD5

#define POWER_PERI_PORT          5 //PE4
#define POWER_PERI_PIN           4  //PE4

#define POWER_TEC_PORT          4   //PD4
#define POWER_TEC_PIN           4   //PD4

#define POWER_HD4_PORT          4   //PD3
#define POWER_HD4_PIN           3   //PD3

#define POWER_SLN_PORT          4   //PD2
#define POWER_SLN_PIN           2   //PD2

#define POWER_LP_PORT          4    //PD6
#define POWER_LP_PIN           6    //PD6

#define POWER_HEATER_PORT       5   //PE3
#define POWER_HEATER_PIN        3   //PE3

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

#define TEC_1_CS_PORT         3  //PC18
#define TEC_1_CS_PIN          18   //PC18

#define TEC_1_SW_PORT         1  //PA30
#define TEC_1_SW_PIN          30   //PA30

#define TEC_2_CS_PORT         3  //PC20
#define TEC_2_CS_PIN          20   //PC20

#define TEC_2_SW_PORT         4  //PD13
#define TEC_2_SW_PIN          13   //PD13

#define TEC_3_CS_PORT         3  //PC22
#define TEC_3_CS_PIN          22   //PC22

#define TEC_3_SW_PORT         3  //PC01
#define TEC_3_SW_PIN          1   //PC01

#define TEC_4_CS_PORT         4  //PD11
#define TEC_4_CS_PIN          11   //PD11

#define TEC_4_SW_PORT         3  //PC02
#define TEC_4_SW_PIN          2   //PC02

#define I2C0_SDA_PORT           1    //PA03
#define I2C0_SDA_PIN            3    //PA03

#define I2C0_SCL_PORT           1    //PA04   
#define I2C0_SCL_PIN            4    //PA04

#define I2C1_SDA_PORT           2     //PB04
#define I2C1_SDA_PIN            4     //PB04

#define I2C1_SCL_PORT           2     //PB05
#define I2C1_SCL_PIN            5     //PB05

#define SENSOR5_EN_PORT         3   //PC11
#define SENSOR5_EN_PIN          11  //PC11

#endif	/* BOARD_V71_SATELLITE_H */

