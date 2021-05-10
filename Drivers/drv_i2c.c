#include "drv_i2c.h"
#include "i2c.h"
#include "i2c_bit.h"



#define I2C_BUS_NUM   4

const char* i2c_dev_name[I2C_BUS_NUM]=
{
	"i2c1",
	"i2c2",
};


struct i2c_bus_device i2c_bus_obj[sizeof(i2c_dev_name)];


s_size_t stm32_master_xfer(struct i2c_bus_device *bus,
                             struct i2c_msg* msg)
{	
	//i2c_start();
	
	//i2c_write_byte(msg->addr<<1 | msg->flags);
	
	for(uint16_t i=0;i<msg->len;i++)
	{
	//	i2c_write_byte(msg->buf++);
	}
	
	if(!(bus->flags & I2C_NO_STOP))
	{
	//	i2c_stop();
	}
}
s_err_t stm32_i2c_bus_control(struct i2c_bus_device *bus,
                                s_uint16_t cmd ,
                                void* param)
{

}


const struct i2c_bus_device_ops   stm32_i2c_ops =
{
	stm32_master_xfer,
	NULL,
	stm32_i2c_bus_control
};


/*----------------------------------------------------------------------*/
static void  stm32_set_sda(void* data , s_uint8_t sta)
{

}
static void stm32_set_scl(void* data , s_uint8_t sta)
{

}
static s_uint8_t stm32_get_sda(void* data)
{
	return 0;
}
static s_uint8_t stm32_get_scl(void* data)
{
	return 0;
}
static void stm32_udelay(void)
{

}
 struct i2c_bit_pos stm32_i2c_bit_ops =
{
  	NULL,
	stm32_set_sda,
	stm32_set_scl,
	stm32_get_sda,
	stm32_get_scl,
	stm32_udelay
};

/**/

int hw_i2c_init(void)
{
  	/*for hardware i2c*/
	i2c_bus_obj[0].ops = &stm32_i2c_ops;
	i2c_bus_register(&i2c_bus_obj[0] , i2c_dev_name[0]);
	
	/*for analog i2c*/
	i2c_bus_obj[1].priv = &stm32_i2c_bit_ops;
	i2c_bit_add_bus(&i2c_bus_obj[1],i2c_dev_name[1]);
                           
	return 0;
}
INIT_BOARD_EXPORT(hw_i2c_init);

