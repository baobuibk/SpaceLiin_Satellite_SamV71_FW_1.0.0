/* 
 * File:   bsp_heater.h
 * Author: HTSANG
 *
 * Created on February 27, 2026, 5:52 PM
 */

#ifndef BSP_HEATER_H
#define	BSP_HEATER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "stdint.h"

void bsp_heater_set_duty(uint8_t duty);
void bsp_heater_off(void);



#ifdef	__cplusplus
}
#endif

#endif	/* BSP_HEATER_H */

