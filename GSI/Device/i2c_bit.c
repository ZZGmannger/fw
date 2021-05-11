
#include "i2c_bit.h"

#include "gsi_def.h"


#define SET_SDA(bus, val)   ((struct i2c_bit_ops*)(bus->priv))->set_sda(bus , val)
#define SET_SCL(bus, val)   ((struct i2c_bit_ops*)(bus->priv))->set_scl(bus , val)
#define GET_SDA(bus)        ((struct i2c_bit_ops*)(bus->priv))->get_sda(bus)
#define GET_SCL(bus)        ((struct i2c_bit_ops*)(bus->priv))->get_scl(bus)


#define SDA_L(bus)          SET_SDA(bus, 0)
#define SDA_H(bus)          SET_SDA(bus, 1)
#define SCL_L(bus)          SET_SCL(bus, 0)
#define SCL_H(bus)          SET_SCL(bus, 1)

#define U_DELAY(bus)        ((struct i2c_bit_ops*)(bus->priv))->udelay(bus)


static void i2c_start(struct i2c_bus_device *bus)
{
  	GSI_ASSERT(bus);
  
	SCL_H(bus);
	U_DELAY(bus);
	
	SDA_H(bus);
	U_DELAY(bus);
	SDA_L(bus);
	U_DELAY(bus);
}

static void i2c_stop(struct i2c_bus_device *bus)
{
	GSI_ASSERT(bus);
	
	SDA_L(bus);
	U_DELAY(bus);
	
	SCL_H(bus);
	U_DELAY(bus);
	
	SDA_H(bus);
	U_DELAY(bus);
}

static s_uint8_t i2c_wait_ack(struct i2c_bus_device *bus)
{
  s_uint8_t ack;

  SCL_L(bus);      
  U_DELAY(bus);
  
  /*release sda in opne drain mode*/
  SDA_H(bus);     
  U_DELAY(bus);

  /*scl high then sda is valid*/ 
  SCL_H(bus);    
  U_DELAY(bus);

  ack = GET_SDA(bus) ? 0 : 1;
  
//  I2C_SCL_LOW;    
//  U_DELAY(bus);

  return ack;
}

static void i2c_send_ack_or_nack(struct i2c_bus_device *bus , s_uint8_t ack)
{
	SCL_L(bus);      
 	U_DELAY(bus);
	
	if(ack)
	  SDA_L(bus);
	else
	  SDA_H(bus);
	U_DELAY(bus);
	
	SCL_H(bus);     
	U_DELAY(bus);
}

static void i2c_write_byte(struct i2c_bus_device *bus ,s_uint8_t data)
{
	for(s_uint8_t i=0;i<8;i++)
	{
	  	SCL_L(bus);      
		U_DELAY(bus);
	  
		if(data & 0x80)
		  SDA_H(bus);
		else
		  SDA_L(bus);
		data <<= 1;
		
		SCL_H(bus);      
		U_DELAY(bus);
	}
	
	i2c_wait_ack(bus);
}

static s_uint8_t i2c_read_byte(struct i2c_bus_device *bus , s_uint8_t ack)
{
  	s_uint8_t data =0;
	
	SCL_L(bus);      
	U_DELAY(bus);
	
	for(s_uint8_t i=0;i<8;i++)
	{
		SCL_H(bus);      
		U_DELAY(bus);
		
		data <<= 1;
		if(GET_SDA(bus))
		  data |= 0x01;

		SCL_L(bus);      
		U_DELAY(bus);  
	}
	i2c_send_ack_or_nack(bus , ack);

	return data;
}

void i2c_bit_open(struct i2c_bus_device *bus , s_uint32_t flag)
{
	((struct i2c_bit_ops*)(bus->priv))->i2c_bit_open(bus , flag);
}

s_size_t i2c_bit_xfer(struct i2c_bus_device *bus,
                             struct i2c_msg* msg)
{
	GSI_ASSERT(bus);
	
	i2c_start(bus);
	
	if(msg->flags & I2C_RD)
	{
		i2c_write_byte(bus  , msg->addr|I2C_RD);
		for(s_uint8_t i=0;i<msg->len -1;i++)
		{
			 msg->buf[i] = i2c_read_byte(bus , 1);
		}
		msg->buf[msg->len -1] = i2c_read_byte(bus , 0);
	}
	else if(msg->flags & I2C_WR)
	{
		i2c_write_byte(bus  , msg->addr|I2C_WR);
		for(s_uint8_t i=0;i<msg->len;i++)
		{
			i2c_write_byte(bus , msg->buf[i]);
		}
	}
	
	if(!(msg->flags & I2C_NO_STOP))
	{
		i2c_stop(bus);
	}
	return 0;
}

static const struct i2c_bus_device_ops i2c_bit_bus_ops =
{
  	i2c_bit_open,
    i2c_bit_xfer,
    NULL
};

int i2c_bit_add_bus(struct i2c_bus_device *bus , const char *bus_name)                     
{
    bus->ops = &i2c_bit_bus_ops;

	i2c_bus_register(bus , bus_name);
	return 0;
}

