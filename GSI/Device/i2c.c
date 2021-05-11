
#include "i2c.h"
#include "gsi_def.h"


struct i2c_bus_device _hw_i2c_bus_device;

void i2c_bus_register(struct i2c_bus_device* bus , const char* name)
{
	struct i2c_bus_device* cur_bus = &_hw_i2c_bus_device;
	
	bus->name = name;

	while(cur_bus->next !=NULL)
	{
		cur_bus = cur_bus->next;
	}
	cur_bus->next = bus;
	bus->next = NULL;
}

struct i2c_bus_device* i2c_bus_find(const char* name)
{
	struct i2c_bus_device* cur_bus = &_hw_i2c_bus_device;
	
	while(cur_bus->next !=NULL)
	{
		cur_bus = cur_bus->next;
		if(0 == strcmp(name , cur_bus->name))
		{
			return cur_bus;
		}
	}
	return GSI_NULL;
}

void i2c_bus_open(struct i2c_bus_device* bus , s_uint16_t flag)
{

}

void i2c_bus_transfer(struct i2c_bus_device* bus ,  struct i2c_msg* msg)
{
	GSI_ASSERT(bus);
	bus->ops->master_xfer(bus , msg);
}


void i2c_bus_control(struct i2c_bus_device* bus , 
				     s_uint16_t cmd ,  
					 void* param)
{
	GSI_ASSERT(bus);
	switch(cmd)
	{
		case I2C_CTRL_ADDR_10BIT:
		  
		case I2C_CTRL_TIMEOUT:
		  bus->timeout = (s_uint32_t)param;
		  break;
		case I2C_CTRL_RETRY:
		  bus->retry = (s_uint32_t)param;
		  break;
	}
}

struct i2c_bus_device*  test;

void i2c_test(void)
{
	test = i2c_bus_find("i2c1");
	if(test!= NULL)
	{
		i2c_bus_open(test , 0);
	}
}


void write_reg(s_uint8_t id , s_uint8_t reg ,s_uint8_t* buf ,s_uint8_t len)
{
	struct i2c_msg msg = {0};

	msg.addr = id;
	msg.flags = I2C_WR|I2C_NO_STOP;
	msg.buf = &reg;
	msg.len = 1;
	i2c_bus_transfer(test , &msg);
	
	msg.flags = I2C_WR|I2C_NO_START;
	msg.buf = buf;
	msg.len = len;
	i2c_bus_transfer(test , &msg);
}

void read_reg(s_uint8_t id , s_uint8_t reg ,s_uint8_t* buf ,s_uint8_t len)
{
	struct i2c_msg msg = {0};

	msg.addr = id;
	msg.flags = I2C_WR|I2C_NO_STOP;
	msg.buf = &reg;
	msg.len = 1;
	i2c_bus_transfer(test , &msg);
	
	msg.flags = I2C_RD;
	msg.buf = buf;
	msg.len = len;
	i2c_bus_transfer(test , &msg);
}


