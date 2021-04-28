///*
//    _/_/_/    _/_/_/    _/_/_/  _/_/_/  _/      _/    _/_/_/  _/    _/  _/_/_/_/
//   _/    _/    _/    _/          _/    _/_/    _/  _/        _/    _/  _/
//  _/_/_/      _/      _/_/      _/    _/  _/  _/  _/  _/_/  _/_/_/_/  _/_/_/
// _/    _/    _/          _/    _/    _/    _/_/  _/    _/  _/    _/  _/
//_/    _/  _/_/_/  _/_/_/    _/_/_/  _/      _/    _/_/_/  _/    _/  _/
//    (C)2016 RisingHF, all rights reserved.
//*/
#include "drv_th02.h"
#include "sensor.h"

#define LOG_TAG    "th02"
#include <elog.h>


#define TH02_SENSOR_NUM   (1)
#define TH02_PARAM_NUM    (2)

static const char* th_sensor_name[TH02_SENSOR_NUM] = {"th02_1"};


struct sensor_parameter th02_param[TH02_PARAM_NUM] = 
{
	{
		.type                = SENSOR_CLASS_HUMI,                  
		.unit                = SENSOR_UNIT_PERMILLAGE,  
		.range_max           = 100,     
		.range_min           = 0,             
		.sensitivity         = 0,
		.precision           = 1,                
		.resolution          = 1,	
	},
	{
		.type                = SENSOR_CLASS_TEMP,                                  
		.unit                = SENSOR_UNIT_PERMILLAGE,  
		.range_max           = 100,     
		.range_min           = 0,             
		.sensitivity         = 0,
		.precision           = 1,                
		.resolution          = 1,		   
	}
};

struct th02_sensor
{
	struct sensor_device sensor;
};
struct th02_sensor th02_obj[TH02_SENSOR_NUM] ;
 


static s_size_t th02_fetch_data(struct sensor_device *sensor, void *buf, s_size_t len)
{
	GSI_ASSERT(buf);
	GSI_ASSERT(sensor);
	
	if(sensor->config.argc > sensor->info.argc)
	{
		return 0;
	}
	
	struct sensor_data *value = (struct sensor_data *)buf;
	
	if (sensor->info.param[sensor->config.argc].type == SENSOR_CLASS_TEMP)
	{
		value->data.temp = -12;
	}
	else if (sensor->info.param[sensor->config.argc ].type == SENSOR_CLASS_HUMI)
	{
		value->data.humi  = 98;
	}
	 
	return 0;
}

static s_err_t th02_control(struct sensor_device *sensor, int cmd, void *args)
{
    s_err_t result = 0;
	
	switch(cmd)
	{
	  case SENSOR_CTRL_GET_ID:     
	  {
		  break;
	  }
	  case SENSOR_CTRL_GET_INFO:       
	  {
		  break;
	  }
	  case SENSOR_CTRL_SET_POWER:   
	  {
		  break;
	  }
	  case SENSOR_CTRL_SELF_TEST:   
	  {
		  break;
	  }
	}

    return result;
}
static const struct sensor_ops _th02_sensor_ops =
{
    th02_fetch_data,
    th02_control
};

int hw_th02_sensor_init(void)
{
  	int result =0; 
  
	for(uint8_t i=0;i<TH02_SENSOR_NUM;i++)
	{
	    th02_obj[i].sensor.ops = &_th02_sensor_ops;
		th02_obj[i].sensor.info.vendor = SENSOR_VENDOR_MIRAMEMS;
		th02_obj[i].sensor.info.intf_type = SENSOR_INTF_I2C;
		
		th02_obj[i].sensor.info.argc = TH02_PARAM_NUM;
		th02_obj[i].sensor.info.param = th02_param;
		
	    result = hw_sensor_register(&th02_obj[i].sensor,
                                     th_sensor_name[i],
                                     SENSOR_FLAG_RDONLY,
                                     NULL);
		if(result != 0)
		{
			LOG_E("device register err code: %d", result);
			goto __exit;
		}
	}
__exit:
    return -1;   
}
INIT_COMPONENT_EXPORT(hw_th02_sensor_init);



//#include "board.h"
//
//int th02_write(uint8_t addr, uint8_t val)
//{
//    uint8_t buf[2];
//
//    buf[0] = addr;
//    buf[1] = val;
//    if( hal_i2c_write(&i2c1, TH02_I2C_ADDR, (char const *)buf, 2, 1) < 0 ){
//        return -1;
//    }
//
//    return 0;
//}
//
//int16_t th02_read(uint8_t addr)
//{
//    uint8_t reg, ret;
//
//    reg = addr;
//    if(hal_i2c_write(&i2c1, TH02_I2C_ADDR, (char const *)&reg, 1, 0) < 0){
//        return -1;
//    }
//
//    if(hal_i2c_read(&i2c1, TH02_I2C_ADDR, (char *)&ret, 1, 1) < 0){
//        return -1;
//    }
//
//    return ret;
//}
//
//int32_t th02_read_word(uint8_t addr)
//{
//    uint8_t reg;
//    uint8_t buf[2];
//    uint16_t ret;
//
//    reg = addr;
//    if( hal_i2c_write(&i2c1, TH02_I2C_ADDR, (char const *)&reg, 1, 0) < 0 ){
//        return -1;
//    }
//
//    if( hal_i2c_read(&i2c1, TH02_I2C_ADDR, (char *)buf, 2, 1) < 0 ){
//        return -1;
//    }
//    ret = ((uint16_t)buf[0]<<8) + buf[1];
//    return ret;
//}
//
//uint8_t th02_get_id(void)
//{
//    return th02_read(TH02_ID);
//}
//
//int th02_wait2rdy(void)
//{
//    int16_t sta;
//    do{
//        sta = th02_read(TH02_STATUS);
//    }while( (sta>=0) && (sta&0x01) != 0 );
//
//    if(sta < 0){
//        return -1;
//    }
//    return 0;
//}
//
///** temp = return_value/32-50 */
//uint32_t th02_get_temp(void)
//{
//    int32_t ret;
//
//    if( th02_write(TH02_CONFIG, TH02_CONFIG_START|TH02_CONFIG_TEMP) < 0 ){
//        return 0;
//    }
//
//    if( th02_wait2rdy() < 0 ){
//        return 0;
//    }
//    ret = th02_read_word(TH02_DATAH);
//    if(ret<0){
//        return 0;
//    }
//
//    return (ret>>2);
//}
//
///** %RH = return_value/16-24 */
//uint32_t th02_get_rh(void)
//{
//    int32_t ret;
//
//    if( th02_write(TH02_CONFIG, TH02_CONFIG_START|TH02_CONFIG_HUMI) < 0 ){
//        return 0;
//    }
//
//    if( th02_wait2rdy() < 0 ){
//        return 0;
//    }
//    ret = th02_read_word(TH02_DATAH);
//    if(ret<0){
//        return 0;
//    }
//
//    return (ret>>4);
//}
//
