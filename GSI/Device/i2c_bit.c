
#include "i2c_bit.h"

#include "gsi_def.h"


#define SET_SDA(ops, val)   ops->set_sda(ops->data , val)
#define SET_SCL(ops, val)   ops->set_scl(ops->data , val)
#define GET_SDA(ops)        ops->get_sda(ops->data)
#define GET_SCL(ops)        ops->get_scl(ops->data)


#define SDA_L(ops)          SET_SDA(ops, 0)
#define SDA_H(ops)          SET_SDA(ops, 1)
#define SCL_L(ops)          SET_SCL(ops, 0)
#define SCL_H(ops)          SET_SCL(ops, 1)

#define U_DELAY(ops)        ops->udelay()


static void i2c_start(struct i2c_bit_pos* ops)
{
  	GSI_ASSERT(ops);
  
	SCL_H(ops);
	U_DELAY(ops);
	
	SDA_H(ops);
	U_DELAY(ops);
	SDA_L(ops);
	U_DELAY(ops);
}

static void i2c_stop(struct i2c_bit_pos* ops)
{
	GSI_ASSERT(ops);
	
	SDA_L(ops);
	U_DELAY(ops);
	
	SCL_H(ops);
	U_DELAY(ops);
	
	SDA_H(ops);
	U_DELAY(ops);
}

static s_uint8_t i2c_wait_ack(struct i2c_bit_pos* ops)
{
  s_uint8_t ack;

  SCL_L(ops);      
  U_DELAY(ops);
  
  /*release sda in opne drain mode*/
  SDA_H(ops);     
  U_DELAY(ops);

  /*scl high then sda is valid*/ 
  SCL_H(ops);    
  U_DELAY(ops);

  ack = GET_SDA(ops) ? 0 : 1;
  
//  I2C_SCL_LOW;    
//  U_DELAY(ops);

  return ack;
}

static void i2c_send_ack_or_nack(struct i2c_bit_pos* ops , s_uint8_t ack)
{
	SCL_L(ops);      
 	U_DELAY(ops);
	
	if(ack)
	  SDA_L(ops);
	else
	  SDA_H(ops);
	U_DELAY(ops);
	
	SCL_H(ops);     
	U_DELAY(ops);
}

static void i2c_write_byte(struct i2c_bit_pos* ops ,s_uint8_t data)
{
	for(s_uint8_t i=0;i<8;i++)
	{
	  	SCL_L(ops);      
		U_DELAY(ops);
	  
		if(data & 0x80)
		  SDA_H(ops);
		else
		  SDA_L(ops);
		data <<= 1;
		
		SCL_H(ops);      
		U_DELAY(ops);
	}
	
	i2c_wait_ack(ops);
}

static s_uint8_t i2c_read_byte(struct i2c_bit_pos* ops , s_uint8_t ack)
{
  	s_uint8_t data =0;
	
	SCL_L(ops);      
	U_DELAY(ops);
	
	for(s_uint8_t i=0;i<8;i++)
	{
		SCL_H(ops);      
		U_DELAY(ops);
		
		data <<= 1;
		if(GET_SDA(ops))
		  data |= 0x01;

		SCL_L(ops);      
		U_DELAY(ops);  
	}
	i2c_put_ack(ops , ack);

	return data;
}

s_size_t i2c_bit_xfer(struct i2c_bus_device *bus,
                             struct i2c_msg* msg)
{
  	struct i2c_bit_pos* ops = (struct i2c_bit_pos*)bus->priv;
	
	GSI_ASSERT(ops);
	
	
  
  
	return 0;
}

static const struct i2c_bus_device_ops i2c_bit_bus_ops =
{
    i2c_bit_xfer,
    NULL,
    NULL
};

int i2c_bit_add_bus(struct i2c_bus_device *bus , const char *bus_name)                     
{
    bus->ops = &i2c_bit_bus_ops;

	i2c_bus_register(bus , bus_name);
	return 0;
}

