/************************************************
 *  @file     : define.h
 *  @date     : October 2025
 *  @author   : CAO HIEU
 *  @version  : 1.0.0
 *-----------------------------------------------
 *  Description :
 *    [-]
 ************************************************/

#ifndef define_H
#define define_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    E_OK       = 0x00U,  /* Operation successful */
    E_ERROR    ,  /* Operation failed */
    E_BUSY     ,  /* Resource is busy */
    E_TIMEOUT   ,  /* Operation timed out */
    ERROR_OK      ,  /* Operation successful */
    ERROR_FAIL ,  /* Operation failed */
    ERROR_BUSY ,  /* Resource is busy */
    ERROR_TIMEOUT ,  /* Operation timed out */
    ERROR_INVALID_PARAM,
    ERROR_NOT_READY,
    ERROR_I2C_NACK, 
    ERROR_I2C_BUS,
    ERROR_I2C_TIMEOUT,
    ERROR_SPI
} Std_ReturnType;

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifdef __cplusplus
}
#endif

#endif /* define_H */