#include "drv_i2c.h"
#include "i2c.h"
#include "i2c_bit.h"


/*hardware i2c driver--------------------------------------------------------*/
#define I2C1_CONFIG                                               \
    {                                                             \
        .name = "hw_i2c1",                                           \
        .Instance = I2C1,                                         \
    }
#define I2C2_CONFIG                                               \
    {                                                             \
        .name = "hw_i2c2",                                           \
        .Instance = I2C2,                                         \
    }


struct stm32_hw_i2c_config
{
	const char* name;
	I2C_TypeDef	 *Instance; 
};

struct stm32_hw_i2c_bus
{
	struct i2c_bus_device bus;
	I2C_HandleTypeDef handle;
	struct stm32_hw_i2c_config* config;
};


struct stm32_hw_i2c_config hw_i2c_config[]=
{
	I2C1_CONFIG,
	I2C2_CONFIG
};
#define HW_I2C_BUS_NUM   sizeof(hw_i2c_config)/sizeof(hw_i2c_config[0])


struct stm32_hw_i2c_bus hw_i2c_bus_obj[HW_I2C_BUS_NUM];

void stm32_i2c_bus_open(struct i2c_bus_device *bus , uint32_t flag)
{
	struct stm32_hw_i2c_bus *hi2c;

    GSI_ASSERT(bus != NULL);
    hi2c = rt_container_of(bus, struct stm32_hw_i2c_bus , bus);

	hi2c->handle.Instance = hi2c->config->Instance;
	hi2c->handle.Init.ClockSpeed = 100000;
	hi2c->handle.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c->handle.Init.OwnAddress1 = 0;
	hi2c->handle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c->handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c->handle.Init.OwnAddress2 = 0;
	hi2c->handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c->handle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c->handle) != HAL_OK)
	{
	
	}
}

s_size_t stm32_master_xfer(struct i2c_bus_device *bus,
                             struct i2c_msg* msg)
{	
	struct stm32_hw_i2c_bus *hi2c;

    GSI_ASSERT(bus != NULL);
    hi2c = rt_container_of(bus, struct stm32_hw_i2c_bus , bus);

	if(bus->flags & I2C_WR)
	{
		HAL_I2C_Master_Transmit(&hi2c->handle, msg->addr, msg->buf, msg->len, bus->timeout);
	}
	else if(bus->flags & I2C_RD)
	{
		HAL_I2C_Master_Receive(&hi2c->handle, msg->addr, msg->buf, msg->len, bus->timeout);
	}
	return 0;
}
s_err_t stm32_i2c_bus_control(struct i2c_bus_device *bus,
                                s_uint16_t cmd ,
                                void* param)
{
	
  return 0;
}


const struct i2c_bus_device_ops   stm32_i2c_ops =
{
  	stm32_i2c_bus_open,
	stm32_master_xfer,
	stm32_i2c_bus_control
};


/*software i2c driver--------------------------------------------------------*/
#define SW_I2C1_CONFIG                                         \
    {                                                          \
        .name = "sw_i2c1",                                      \
        .sda_pin = 32,                                         \
		.scl_pin = 33,                                         \
    }
#define SW_I2C2_CONFIG                                         \
    {                                                          \
        .name = "sw_i2c2",                                      \
        .sda_pin = 22,                                         \
		.scl_pin = 23,                                         \
    }

struct stm32_sw_i2c_config
{
	const char* name;
    uint32_t  sda_pin;
	uint32_t  scl_pin;
};

struct stm32_sw_i2c_bus
{
	struct i2c_bus_device bus;
	struct stm32_sw_i2c_config* config;
};

struct stm32_sw_i2c_config sw_i2c_config[]=
{
	SW_I2C1_CONFIG,
	SW_I2C2_CONFIG
};

#define SW_I2C_BUS_NUM   sizeof(sw_i2c_config)/sizeof(sw_i2c_config[0])

struct stm32_sw_i2c_bus sw_i2c_bus_obj[SW_I2C_BUS_NUM];

static void stm32_i2c_bit_open(struct i2c_bus_device* bus , uint32_t flag)
{
	struct stm32_sw_i2c_bus *si2c;

    GSI_ASSERT(bus != NULL);
    si2c = rt_container_of(bus, struct stm32_sw_i2c_bus , bus);
	
	pin_mode(si2c->config->scl_pin , PIN_MODE_OUTPUT_OD);
	pin_mode(si2c->config->sda_pin , PIN_MODE_OUTPUT_OD);
	
}

static void  stm32_set_sda(struct i2c_bus_device* bus , s_uint8_t sta)
{
	struct stm32_sw_i2c_bus *si2c;

    GSI_ASSERT(bus != NULL);
    si2c = rt_container_of(bus, struct stm32_sw_i2c_bus , bus);
	
	pin_write(si2c->config->sda_pin , sta);
}
static void stm32_set_scl(struct i2c_bus_device* bus, s_uint8_t sta)
{
	struct stm32_sw_i2c_bus *si2c;

    GSI_ASSERT(bus != NULL);
    si2c = rt_container_of(bus, struct stm32_sw_i2c_bus , bus);
	
	pin_write(si2c->config->scl_pin , sta);
}
static s_uint8_t stm32_get_sda(struct i2c_bus_device* bus)
{
	struct stm32_sw_i2c_bus *si2c;

    GSI_ASSERT(bus != NULL);
    si2c = rt_container_of(bus, struct stm32_sw_i2c_bus , bus);
	
	return pin_read(si2c->config->sda_pin);
}
static s_uint8_t stm32_get_scl(struct i2c_bus_device* bus)
{
	struct stm32_sw_i2c_bus *si2c;

    GSI_ASSERT(bus != NULL);
    si2c = rt_container_of(bus, struct stm32_sw_i2c_bus , bus);
	
	return pin_read(si2c->config->scl_pin);
}
static void stm32_udelay(struct i2c_bus_device* bus)
{
	s_uint32_t Delay = 20 * 168/4;
    do
    {
        __NOP();
    }
    while (Delay --);
}
 struct i2c_bit_ops stm32_i2c_bit_ops =
{
  	stm32_i2c_bit_open,
	stm32_set_sda,
	stm32_set_scl,
	stm32_get_sda,
	stm32_get_scl,
	stm32_udelay
};


int hw_i2c_init(void)
{
  	/*for hardware i2c*/
    for(uint8_t i=0;i<HW_I2C_BUS_NUM;i++)
	{
		hw_i2c_bus_obj[i].bus.ops = &stm32_i2c_ops;
		hw_i2c_bus_obj[i].config = &hw_i2c_config[i];
		
		i2c_bus_register(&hw_i2c_bus_obj[i].bus , hw_i2c_config[i].name);
	}
	
	/*for analog i2c*/
	for(uint8_t i=0;i<SW_I2C_BUS_NUM;i++)
	{
	  	sw_i2c_bus_obj[i].bus.priv = &stm32_i2c_bit_ops;
		sw_i2c_bus_obj[i].config = &sw_i2c_config[i];
		
		i2c_bit_add_bus(&sw_i2c_bus_obj[i].bus , sw_i2c_config[i].name);
	}
                        
	return 0;
}
INIT_BOARD_EXPORT(hw_i2c_init);

