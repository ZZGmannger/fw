#include "sensor.h"
#include "shell.h"
#define LOG_TAG    "SensorCmd"
#include <elog.h>
 

void sensor_show_data(struct sensor_data* data);

int sensor_init_cmd(const char* name)
{
  	struct sensor_device* sensor = sensor_find(name);
 
	if(sensor == NULL)
	{
		LOG_E("sensor \"%s\" was not found",name);
		return -1;
	}
	sensor_open(sensor , SENSOR_FLAG_RDONLY);
	return 0;
}
 
void sensor_info_cmd(struct sensor_device* sensor)
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

void sensor_read_cmd(struct sensor_device* sensor)		
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

__weak void sensor_show_data(struct sensor_data* data)
{
	switch(data->type)
	{
		case SENSOR_CLASS_ACCE :
			LOG_D("");
			break;
		case SENSOR_CLASS_TEMP :
		    //connect to lap
		  	LOG_D("temp: %d" ,data->data.temp);
		  	break;
		case SENSOR_CLASS_HUMI :
		   //connect to lap
		  	LOG_D("humi: %d%" ,data->data.humi);
		  	break;
		case SENSOR_CLASS_BARO :
			LOG_D("");
			break;
		case SENSOR_CLASS_LIGHT :
			LOG_D("");
			break;
		case SENSOR_CLASS_PROXIMITY :
			LOG_D("");
			break;
		case SENSOR_CLASS_TVOC :
			  LOG_D("");
		  break;
		case SENSOR_CLASS_NOISE :
			LOG_D("");
			break;
		case  SENSOR_CLASS_STEP :
			LOG_D("");
			break;
		case SENSOR_CLASS_FORCE :
			LOG_D("");
			break;
	}
}

static void sensor(int argc, char* sensor_name  , char *argv)
{
	struct sensor_device* cur_sensor;
	if(argc==3)
	{
		cur_sensor = sensor_list();
		while(NULL != cur_sensor)
		{
			LOG_I(" %s ",cur_sensor->info.name);
		  	cur_sensor = cur_sensor->next;
		}
	}
	else if(argc ==4)
	{
		if(0==strcmp(argv , "list"))
		{
			cur_sensor = sensor_list();
			while(NULL != cur_sensor)
			{
				LOG_I(" %s ",cur_sensor->info.name);
				cur_sensor = cur_sensor->next;
			}
		}
	}
	else if(argc==5)
	{
		if(0==strcmp(argv , "info"))
		{
			cur_sensor = sensor_find(sensor_name);
			if(cur_sensor != NULL)
			{
				sensor_info_cmd(cur_sensor);
			}
			else
			{
				LOG_W("sensor \"%s\" was not found",sensor_name);	
			}
		}
		else if(0==strcmp(argv , "read"))
		{
			cur_sensor = sensor_find(sensor_name);
			sensor_init_cmd(sensor_name);
			sensor_read_cmd(cur_sensor);
		}
	}
}

SHELL_EXPORT_CMD(SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC)|SHELL_CMD_DISABLE_RETURN ,sensor, sensor, sensor);