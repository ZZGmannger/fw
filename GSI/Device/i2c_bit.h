
#ifndef I2C_BIT_H
#define I2C_BIT_H

#include "gsi_def.h"
#include "i2c.h"

struct i2c_bit_ops
{
  	void (*i2c_bit_open)(struct i2c_bus_device* bus , s_uint32_t flag);
	void (*set_sda)(struct i2c_bus_device *bus ,s_uint8_t sta);
	void (*set_scl)(struct i2c_bus_device *bus ,s_uint8_t sta);
	s_uint8_t (*get_sda)(struct i2c_bus_device *bus);
	s_uint8_t (*get_scl)(struct i2c_bus_device *bus);
	
	void(*udelay)(struct i2c_bus_device *bus);
};




int i2c_bit_add_bus(struct i2c_bus_device *bus , const char *bus_name);                     

#endif