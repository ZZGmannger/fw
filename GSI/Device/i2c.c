
#include "i2c.h"
#include "gsi_def.h"

 
#define I2C_WRITE_EN    0
#define I2C_READ_EN     1

#define I2C_HIGH     1
#define I2C_LOW      0

#define I2C_NOACK    0
#define I2C_ACK      1


#define I2C_SDA(x)  x
#define I2C_SCL(x)  x
#define I2C_Delay() 1

#define I2C_SDA_READ    1

void i2c_start(void)
{
	I2C_SCL(I2C_HIGH);
	I2C_Delay();
	
	I2C_SDA(I2C_HIGH);
	I2C_Delay();
	I2C_SDA(I2C_LOW);
	I2C_Delay();
}

void i2c_stop(void)
{
	I2C_SDA(I2C_LOW);
	I2C_Delay();
	
	I2C_SCL(I2C_HIGH);
	I2C_Delay();
	
	I2C_SDA(I2C_HIGH);
	I2C_Delay();
}

s_uint8_t i2c_get_ack(void)
{
  s_uint8_t ack;

  I2C_SCL(I2C_LOW);      
  I2C_Delay();
  
  /*release sda in opne drain mode*/
  I2C_SDA(I2C_HIGH);     
  I2C_Delay();

  /*scl high then sda is valid*/ 
  I2C_SCL(I2C_HIGH);    
  I2C_Delay();

  ack = I2C_SDA_READ ? I2C_NOACK:I2C_ACK;
  
//  I2C_SCL_LOW;    
//  I2C_Delay();

  return ack;
}

void i2c_put_ack(s_uint8_t ack)
{
	I2C_SCL(I2C_LOW);      
	I2C_Delay();
	
	if(ack == I2C_NOACK)
	  I2C_SDA(I2C_LOW);
	else
	  I2C_SDA(I2C_HIGH);
	I2C_Delay();
	
	I2C_SCL(I2C_HIGH);      
	I2C_Delay();
}

void i2c_write_byte(s_uint8_t data)
{
	for(s_uint8_t i=0;i<8;i++)
	{
	  	I2C_SCL(I2C_LOW);      
		I2C_Delay();
	  
		if(data & 0x80)
		  I2C_SDA(I2C_HIGH);
		else
		  I2C_SDA(I2C_LOW);
		data <<= 1;
		
		I2C_SCL(I2C_HIGH);      
		I2C_Delay();
	}
	
	i2c_get_ack();
}

s_uint8_t i2c_read_byte(s_uint8_t ack)
{
  	s_uint8_t data =0;
	
	I2C_SCL(I2C_LOW);      
	I2C_Delay();
	
	for(s_uint8_t i=0;i<8;i++)
	{
		I2C_SCL(I2C_HIGH);      
		I2C_Delay();
		
		data <<= 1;
		if(I2C_SDA_READ)
		  data |= 0x01;

		I2C_SCL(I2C_LOW);      
		I2C_Delay();  
	}
	i2c_put_ack(ack);

	return data;
}


void i2c_write_regs(s_uint8_t id ,s_uint16_t addr ,s_uint8_t*buf ,s_uint8_t len)
{
  	s_uint8_t i=0;
	i2c_start();
	
	i2c_write_byte(id<<1|I2C_WRITE_EN);
	i2c_write_byte(addr);
	
	for(i=0; i<len;i++)
	{
		i2c_write_byte(*buf++);
	}
 
	i2c_stop();
}


void i2c_read_regs(s_uint8_t id ,s_uint8_t addr ,s_uint8_t*buf ,s_uint8_t len)
{
	s_uint8_t i=0;
	i2c_start();
	
	i2c_write_byte(id<<1|I2C_WRITE_EN);
	i2c_write_byte(addr);
	
	i2c_start();
	i2c_write_byte(id<<1|I2C_READ_EN);
	
	for(i=0; i<len-1;i++)
	{
		*buf++ = i2c_read_byte(I2C_ACK);
	}
	*buf++ = i2c_read_byte(I2C_NOACK);
	i2c_stop();
}
