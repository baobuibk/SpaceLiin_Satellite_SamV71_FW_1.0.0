#include "bsp_bmp390.h"
#include "stdbool.h"
#include "bsp_core.h" // ?? dùng vTaskDelay ho?c os_delay
#include "D:\down_repo_for_test\SpaceLiin_Satellite_SamV71_FW_1.0.0-main\src\dev\M4_Middlewares\FreeRTOS\FreeRTOSv20240604-LTS\portable\GCC\ARM_CM7\r0p1\portmacro.h"
//#include "D:\down_repo_for_test\SpaceLiin_Satellite_SamV71_FW_1.0.0-main\src\dev\M4_Middlewares\FreeRTOS\FreeRTOSv20240604-LTS\include\task.h"
#include "D:\down_repo_for_test\SpaceLiin_Satellite_SamV71_FW_1.0.0-main\src\dev\M3_Driver\components\dio\do.h"


Std_ReturnType bsp_bmp390_onboard_read(bmp390_data_t* p_data)
{
    if (p_data == NULL) {
        return ERROR_INVALID_PARAM;
    }

    Std_ReturnType status = i2c_io_enable(&bmp390_onboard);
    if (status != ERROR_OK) {
        return status; 
    }
    
    BMP390_init(&bmp390_onboard, BMP390_ONBOARD_ADDRESS);
    
    BMP390_read_value(&bmp390_onboard, p_data, BMP390_ONBOARD_ADDRESS);

    if (p_data->Temp != 0.0f || p_data->Pressure != 0.0f) {
        return ERROR_OK; 
    } else 
    {
        return ERROR_FAIL; 
    }
}


void bsp_bmp390_connector_enable(void)
{
    do_set(&bmp390_connector_sensor5);
}

void bsp_bmp390_connector_disable(void)
{
    do_reset(&bmp390_connector_sensor5);
}

bool bsp_bmp390_connector_status(void)
{
    return bmp390_connector_sensor5.bStatus;
}

Std_ReturnType bsp_bmp390_connector_read(bmp390_data_t* p_data)
{   
    if (p_data == NULL) {
        return ERROR_INVALID_PARAM;
    }
    
    if (bsp_bmp390_connector_status()==0)
    {
        return ERROR_FAIL;
    }
    
    Std_ReturnType status = i2c_io_enable(&bmp390_connector);
    if (status != ERROR_OK)
    {
        return status; 
    }
    
    BMP390_init(&bmp390_connector, BMP390_CONNECTOR_ADDRESS);
    
    BMP390_read_value(&bmp390_connector, p_data, BMP390_CONNECTOR_ADDRESS);

    if (p_data->Temp != 0.0f || p_data->Pressure != 0.0f) 
    {
        return ERROR_OK; 
    } 
    else 
    {
        return ERROR_FAIL; 
    }
}