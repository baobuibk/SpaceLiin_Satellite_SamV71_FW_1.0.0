#ifndef BSP_BSP_BMP390_H_
#define BSP_BSP_BMP390_H_

#include "define.h"
#include "bmp390.h"

Std_ReturnType bsp_bmp390_onboard_read(bmp390_data_t* p_data);
Std_ReturnType bsp_bmp390_connector_read(bmp390_data_t* p_data);

void bsp_bmp390_connector_enable(void);
void bsp_bmp390_connector_disable(void);
bool bsp_bmp390_connector_status(void);

#endif