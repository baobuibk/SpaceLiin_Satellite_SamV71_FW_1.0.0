#include "bsp_solenoid.h"
#include "bsp_core.h"

#include "M3_Driver/devices/tca6416/tca6416.h"

extern i2c_io_t sol_expander_i2c;

tca6416a_t expander = {
    .bus = &sol_expander_i2c,
    .addr7 = 0x20,
};

void bsp_solenoid_init()
{
    ((pio_registers_t*)PIO_PORT_C)->PIO_PER |= (1u << 17);
    ((pio_registers_t*)PIO_PORT_C)->PIO_PUER |= (1u << 17);
    ((pio_registers_t*)PIO_PORT_C)->PIO_OER |= (1u << 17);
    ((pio_registers_t*)PIO_PORT_C)->PIO_SODR |= (1u << 17);
    
    tca6416a_init(&expander, &sol_expander_i2c, 0x20);
    tca6416a_write_outputs(&expander, 0);
    tca6416a_write_modes(&expander, 0);
    
}

uint32_t bsp_sol_single_on(uint8_t index)
{
    uint32_t rc;
    rc = tca6416a_pin_write(&expander,index,1);
    return rc;
}

uint32_t bsp_sol_single_off(uint8_t index)
{
    uint32_t rc;
    rc = tca6416a_pin_write(&expander,index,0);
    return rc;
}

uint32_t bsp_sol_single_status(uint8_t index)
{
    uint32_t rc=0;
    rc = tca6416a_pin_read(&expander,index);
    return rc;
}
