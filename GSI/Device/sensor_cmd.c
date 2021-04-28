#include "sensor.h"
#include "shell.h"
#define LOG_TAG    "SensorCmd"
#include <elog.h>
 

__weak void sensor_show_data(struct sensor_data* data)
{
	switch(data->type)
	{
		case SENSOR_CLASS_ACCE :
		  	LOG_D("acce x:%d",data->data.acce.x);
		  	LOG_D("acce y:%d",data->data.acce.y);
		  	LOG_D("acce z:%d",data->data.acce.z);break;
		case SENSOR_CLASS_TEMP :
		  	LOG_D("temp: %d" ,data->data.temp);	break;
		case SENSOR_CLASS_HUMI :
		  	LOG_D("humi: %d%" ,data->data.humi);break;
		case SENSOR_CLASS_BARO :
			LOG_D("");break;
		case SENSOR_CLASS_LIGHT :
			LOG_D("");break;
		case SENSOR_CLASS_PROXIMITY :
			LOG_D("");break;
		case SENSOR_CLASS_TVOC :
			  LOG_D(""); break;
		case SENSOR_CLASS_NOISE :
			LOG_D("");break;
		case  SENSOR_CLASS_STEP :
			LOG_D("");break;
		case SENSOR_CLASS_FORCE :
			LOG_D("");break;
	}
}

static void sensor_info_log(struct sensor_device* sensor)
{
	if(sensor != NULL)
	{
		LOG_I("sensor: %s" , sensor->info.name);
		LOG_I("	  vendor: %s" ,SENSOR_VENDOR_INFO(sensor->info.vendor));
		LOG_I("	  intf: %s" , SENSOR_INTF_INFO(sensor->info.intf_type));
		LOG_I("	  property num: %d" , sensor->info.argc);
		for(uint8_t i=0;i<sensor->info.argc;i++)
		{
			LOG_I("	  property(%d):",i);
			LOG_I("	      type: %s" ,  SENSOR_TYPE_INFO(sensor->info.param[i].type));
			LOG_I("	      unit: %d" , sensor->info.param[i].unit);
			LOG_I("	      max: %d" ,  sensor->info.param[i].range_max);
			LOG_I("	      min: %d" ,  sensor->info.param[i].range_min);
			LOG_I("	      sensitivity: %d" , sensor->info.param[i].sensitivity);
			LOG_I("	      precision: %d"   , sensor->info.param[i].precision);
			LOG_I("	      resolution: %d"  , sensor->info.param[i].resolution);
		}
	}
}

static void sensor_read_param(struct sensor_device* sensor)		
{
	struct sensor_data sdata = {0};
	for(uint8_t i=0;i<sensor->info.argc;i++)
	{
	  	sdata.timestamp = HAL_GetTick();
		sdata.type = sensor->info.param[i].type;
		
		sensor_control(sensor, SENSOR_CTRL_SET_ARGC, (void *)i);
		sensor_read(sensor , &sdata , 1);
		sensor_show_data(&sdata);
	}
}

void sensor_list_all_cmd(void)
{
	struct sensor_device* cur_sensor;
	cur_sensor = sensor_list();
	while(NULL != cur_sensor)
	{
		LOG_I(" %s ",cur_sensor->info.name);
		cur_sensor = cur_sensor->next;
	}
}
SHELL_EXPORT_CMD(SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC)|SHELL_CMD_DISABLE_RETURN ,sensor_list, sensor_list_all_cmd, list all sensor name);


void sensor_info_cmd(const char* name)
{
	struct sensor_device* cur_sensor;
	cur_sensor = sensor_find(name);
	if(cur_sensor != NULL)
	{
		sensor_info_log(cur_sensor);
	}
	else
	{
		LOG_W("sensor \"%s\" was not found",name);	
	}
}
SHELL_EXPORT_CMD(SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC)|SHELL_CMD_DISABLE_RETURN ,sensor_info, sensor_info_cmd,  sensor information);


static void sensor_init_cmd(const char* name)
{
  	struct sensor_device* sensor = sensor_find(name);
 
	if(sensor == NULL)
	{
		LOG_E("sensor \"%s\" was not found",name);
		return;
	}
	sensor_open(sensor , SENSOR_FLAG_RDONLY);
}
SHELL_EXPORT_CMD(SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC)|SHELL_CMD_DISABLE_RETURN ,sensor_init, sensor_init_cmd,  sensor init);


static void sensor_read_cmd(const char* name)
{
	struct sensor_device* cur_sensor;
	cur_sensor = sensor_find(name);
	if(cur_sensor != NULL)
	{
		sensor_read_param(cur_sensor);
	}
	else
	{
		LOG_W("sensor \"%s\" was not found",name);	
	}
}
SHELL_EXPORT_CMD(SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC)|SHELL_CMD_DISABLE_RETURN ,sensor_read, sensor_read_cmd, read single sensor);


static void sensor_read_all_cmd(void)
{
  	struct sensor_device* cur_sensor;
	cur_sensor = sensor_list();
	while(NULL != cur_sensor)
	{
		LOG_I(" %s ",cur_sensor->info.name);
		sensor_read_param(cur_sensor);
		cur_sensor = cur_sensor->next;
	}
}
SHELL_EXPORT_CMD(SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC)|SHELL_CMD_DISABLE_RETURN ,sensor_read_all, sensor_read_all_cmd, read all sensor);


