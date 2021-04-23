
#include "sensor.h"
#define LOG_TAG    "SensorApp"
#include <elog.h>
#include "main.h"

#define  SHTXX_NAME      "sht21_1"
 
struct sensor_device*  shtxx_sensor;
struct sensor_data sdata;

int SensorAppTest_init(void)
{
  	shtxx_sensor = sensor_find(SHTXX_NAME);
 
	if(shtxx_sensor == NULL)
	{
		LOG_E("no shtxx_sensor found!");
		return -1;
	}
	sensor_open(shtxx_sensor , SENSOR_FLAG_RDONLY);
	
	LOG_D("sensor info name: %s" , shtxx_sensor->info.name);
	LOG_D("sensor info vendor: %d" , shtxx_sensor->info.vendor);
	LOG_D("sensor info intf_type: %d" , shtxx_sensor->info.intf_type);
	LOG_D("sensor info argc: %d" , shtxx_sensor->info.argc);
	
	LOG_D("sensor info param1 type: %d" , shtxx_sensor->info.param[0].type);
	LOG_D("sensor info param2 type: %d" , shtxx_sensor->info.param[1].type);

	return 0;
}
INIT_TEST_EXPORT(SensorAppTest_init);

void SensorAppTest_poll(void)
{
	sensor_control(shtxx_sensor, SENSOR_CTRL_SET_ARGC, (void *)0);
	sensor_read(shtxx_sensor , &sdata , 1);
	LOG_D("humi is %d%_" , sdata.data.humi);
  
	sensor_control(shtxx_sensor, SENSOR_CTRL_SET_ARGC, (void *)1);
	sensor_read(shtxx_sensor , &sdata , 1);
	LOG_D("temp is %d c" , sdata.data.temp);

	HAL_Delay(1000);
}
RUN_TEST_EXPORT(SensorAppTest_poll);