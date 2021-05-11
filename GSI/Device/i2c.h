
#ifndef I2C_H
#define I2C_H

#include "gsi_def.h"

#define I2C_WR                0x0000
#define I2C_RD               (1u << 0)
#define I2C_ADDR_10BIT       (1u << 2)  /* this is a ten bit chip address */
#define I2C_NO_START         (1u << 4)
#define I2C_NO_STOP          (1u << 5)
#define I2C_IGNORE_NACK      (1u << 6)
#define I2C_NO_READ_ACK      (1u << 7)  /* when I2C reading, we do not ACK */


#define I2C_CTRL_ADDR_10BIT    0
#define I2C_CTRL_TIMEOUT       1
#define I2C_CTRL_RETRY         2 

struct i2c_msg
{
    s_uint16_t addr;
    s_uint16_t flags;
    s_uint16_t len;
    s_uint8_t  *buf;
};


struct i2c_bus_device;

struct i2c_bus_device_ops
{
	void (*open)(struct i2c_bus_device *bus, s_uint32_t flag);
                           
    s_size_t (*master_xfer)(struct i2c_bus_device *bus,
                             struct i2c_msg* msg);
                           
    s_err_t (*control)(struct i2c_bus_device *bus,
                                s_uint16_t cmd ,
                                void* param);
};

/*for i2c bus driver*/
struct i2c_bus_device
{
  	struct i2c_bus_device* next;
    const struct i2c_bus_device_ops *ops;
	const char* name;
    s_uint16_t  flags;
    s_uint32_t  timeout;
    s_uint32_t  retry;
    void *priv;
};

void i2c_bus_register(struct i2c_bus_device* bus , const char* name);

#endif