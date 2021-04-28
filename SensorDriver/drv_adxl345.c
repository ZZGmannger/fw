///*
//    _/_/_/    _/_/_/    _/_/_/  _/_/_/  _/      _/    _/_/_/  _/    _/  _/_/_/_/
//   _/    _/    _/    _/          _/    _/_/    _/  _/        _/    _/  _/
//  _/_/_/      _/      _/_/      _/    _/  _/  _/  _/  _/_/  _/_/_/_/  _/_/_/
// _/    _/    _/          _/    _/    _/    _/_/  _/    _/  _/    _/  _/
//_/    _/  _/_/_/  _/_/_/    _/_/_/  _/      _/    _/_/_/  _/    _/  _/
//    (C)2016 RisingHF, all rights reserved.
//*/
#include "drv_adxl345.h"
#include "sensor.h"


#define LOG_TAG    "adxl"
#include <elog.h>


#define ADXL_SENSOR_NUM   (1)
#define ADXL_PARAM_NUM    (1)

static const char* adxl_sensor_name[ADXL_SENSOR_NUM] = {"adxl_1"};


struct sensor_parameter adxl_param[ADXL_PARAM_NUM] = 
{
	{
		.type                = SENSOR_CLASS_ACCE,                  
		.unit                = SENSOR_UNIT_MG,  
		.range_max           = 100,     
		.range_min           = 0,             
		.sensitivity         = 0,
		.precision           = 1,                
		.resolution          = 1,	
	},
};

struct adxl_sensor
{
  	/*add some local parameter*/
	struct sensor_device sensor;
};
struct adxl_sensor adxl_obj[ADXL_SENSOR_NUM];
 
 


static s_size_t adxl_fetch_data(struct sensor_device *sensor, void *buf, s_size_t len)
{
	GSI_ASSERT(buf);
	GSI_ASSERT(sensor);
	
	if(sensor->config.argc > sensor->info.argc)
	{
		return 0;
	}
	
	struct sensor_data *value = (struct sensor_data *)buf;
	
	if (sensor->info.param[sensor->config.argc].type == SENSOR_CLASS_ACCE)
	{
		value->data.acce.x = 26;
		value->data.acce.y = 123;
		value->data.acce.z = -231;
	}
	return 0;
}

static s_err_t adxl_control(struct sensor_device *sensor, int cmd, void *args)
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
static const struct sensor_ops _adxl_sensor_ops =
{
    adxl_fetch_data,
    adxl_control
};

int hw_adxl_sensor_init(void)
{
  	int result =0; 
  
	for(uint8_t i=0;i<ADXL_SENSOR_NUM;i++)
	{
	    adxl_obj[i].sensor.ops = &_adxl_sensor_ops;
		adxl_obj[i].sensor.info.intf_type = SENSOR_INTF_SPI;
		adxl_obj[i].sensor.info.vendor = SENSOR_VENDOR_GOERTEK;
	
		adxl_obj[i].sensor.info.argc = ADXL_PARAM_NUM;
		adxl_obj[i].sensor.info.param = adxl_param;
		
	    result = hw_sensor_register(&adxl_obj[i].sensor,
                                     adxl_sensor_name[i],
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
INIT_COMPONENT_EXPORT(hw_adxl_sensor_init);


//#include "board.h"
//
//int adxl345_write(uint8_t addr, uint8_t val)
//{
//    uint8_t buf[2];
//
//    buf[0] = addr;
//    buf[1] = val;
//    if( hal_i2c_write(&i2c1, ADXL345_I2C_ADDR, (char const *)buf, 2, 1) < 0 ){
//        return -1;
//    }
//
//    return 0;
//}
//
//int16_t adxl345_read(uint8_t addr)
//{
//    uint8_t reg, ret;
//
//    reg = addr;
//    if(hal_i2c_write(&i2c1, ADXL345_I2C_ADDR, (char const *)&reg, 1, 0) < 0){
//        return -1;
//    }
//
//    if(hal_i2c_read(&i2c1, ADXL345_I2C_ADDR, (char *)&ret, 1, 1) < 0){
//        return -1;
//    }
//
//    return ret;
//}
//
//int adxl345_read_buf(uint8_t addr, uint8_t *buf, int len)
//{
//    uint8_t reg;
//
//    reg = addr;
//    if( hal_i2c_write(&i2c1, ADXL345_I2C_ADDR, (char const *)&reg, 1, 0) < 0 ){
//        return -1;
//    }
//
//    if( hal_i2c_read(&i2c1, ADXL345_I2C_ADDR, (char *)buf, len, 1) < 0 ){
//        return -1;
//    }
//
//    return len;
//}
//
//int adxl345_set_range(uint8_t gRange, uint8_t fullRes)
//{
//    int16_t oldDataFormat = 0;
//    unsigned char newDataFormat = 0;
//
//    oldDataFormat = adxl345_read(ADXL345_DATA_FORMAT);
//    if(oldDataFormat < 0){
//        return -1;
//    }
//
//    newDataFormat = oldDataFormat & ~(ADXL345_RANGE(0x3) | ADXL345_FULL_RES);
//    newDataFormat =  newDataFormat | ADXL345_RANGE(gRange) | fullRes;
//    if(adxl345_write(ADXL345_DATA_FORMAT, newDataFormat)<0){
//        return -1;
//    }
//
//    return 0;
//}
//
///* 1: measure, 0: standby */
//int adxl345_set_mode(uint8_t mode)
//{
//    uint8_t reg;
//
//    reg = adxl345_read(ADXL345_POWER_CTL);
////    if( (mode && (reg&ADXL345_PCTL_MEASURE)) || (!mode && !(reg&ADXL345_PCTL_MEASURE)) ){
////        // already in meaure mode
////        return 0;
////    }
//
//    if(mode){
//        reg |= ADXL345_PCTL_MEASURE;
//    }else{
//        reg &= ~ADXL345_PCTL_MEASURE;
//    }
//
//    if( adxl345_write(ADXL345_POWER_CTL, reg)<0 ){
//        return -1;
//    }
//
//    return 0;
//}
//
//uint8_t adxl345_get_id(void)
//{
//    return adxl345_read(ADXL345_DEVID);
//}
//
///* buf size must larger than 6
//    buf[0] --> X-MSBs
//    buf[1] --> X-LSBs
//    buf[2] --> Y-MSBs
//    buf[3] --> Y-LSBs
//    buf[4] --> Z-MSBs
//    buf[5] --> Z-LSBs
//    */
//int adxl345_read_xyz(uint8_t *buf)
//{
//    uint8_t tmp;
//    if(adxl345_read_buf(ADXL345_DATAX0, buf, 6)<0){
//        return -1;
//    }
//
//    // swap MSBs and LSBs
//    tmp = buf[0];
//    buf[0] = buf[1];
//    buf[1] = tmp;
//
//    tmp = buf[2];
//    buf[2] = buf[3];
//    buf[3] = tmp;
//
//    tmp = buf[4];
//    buf[4] = buf[5];
//    buf[5] = tmp;
//    return 0;
//}
//
