
#ifndef I2C_BIT_H
#define I2C_BIT_H

#include "gsi_def.h"
#include "i2c.h"

struct i2c_bit_pos
{
  	void* data;
	void (*set_sda)(void* data , s_uint8_t sta);
	void (*set_scl)(void* data , s_uint8_t sta);
	s_uint8_t (*get_sda)(void* data);
	s_uint8_t (*get_scl)(void* data);
	
	void(*udelay)(void);
};

int i2c_bit_add_bus(struct i2c_bus_device *bus , const char *bus_name);                     

#endif