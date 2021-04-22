
#include "sensor.h"

#define LOG_TAG    "sensor"
#include <elog.h>

#define X(a, b ,c)	 a,  
static char *const sensor_name_str[] =
{
    SENSOR_OBJ
};
#undef X


 
static struct sensor_device _hw_sensor;

struct sensor_device* sensor_find(const char* name)
{
	struct sensor_device*  cur_sensor = &_hw_sensor;
	
	while(cur_sensor->next != NULL)
	{
		cur_sensor = cur_sensor->next;
		if(GSI_NULL != strstr(cur_sensor->info.name , name))
		{
			return cur_sensor;
		}
	}
	
	return NULL;
}

/* RT-Thread Device Interface */
s_err_t sensor_open(struct sensor_device* sensor , uint16_t oflag)
{
    GSI_ASSERT(sensor !=  GSI_NULL);
    s_err_t res = 0;


    if (oflag & SENSOR_FLAG_RDONLY && sensor->init_flag & SENSOR_FLAG_RDONLY)
    {
        if (sensor->ops->control != GSI_NULL)
        {
            /* If polling mode is supported, configure it to polling mode */
            sensor->ops->control(sensor, SENSOR_CTRL_SET_MODE, (void *)SENSOR_MODE_POLLING);
        }
        sensor->config.mode = SENSOR_MODE_POLLING;
    }
    else if (oflag & SENSOR_FLAG_INT_RX && sensor->init_flag & SENSOR_FLAG_INT_RX)
    {
        if (sensor->ops->control != GSI_NULL)
        {
            /* If interrupt mode is supported, configure it to interrupt mode */
            sensor->ops->control(sensor, SENSOR_CTRL_SET_MODE, (void *)SENSOR_MODE_INT);
        }
        /* Initialization sensor interrupt */
        rt_sensor_irq_init(sensor);
        sensor->config.mode =  SENSOR_MODE_INT;
    }
    else if (oflag & SENSOR_FLAG_FIFO_RX && sensor->init_flag & SENSOR_FLAG_FIFO_RX)
    {
        if (sensor->ops->control != NULL)
        {
            /* If fifo mode is supported, configure it to fifo mode */
            sensor->ops->control(sensor, SENSOR_CTRL_SET_MODE, (void *)SENSOR_MODE_FIFO);
        }
        /* Initialization sensor interrupt */
        rt_sensor_irq_init(sensor);
        sensor->config.mode = SENSOR_MODE_FIFO;
    }
    else
    {
        res = -EINVAL;
        goto __exit;
    }

    /* Configure power mode to normal mode */
    if (sensor->ops->control(sensor, SENSOR_CTRL_SET_POWER, (void *)SENSOR_POWER_NORMAL) == EOK)
    {
        sensor->config.power = SENSOR_POWER_NORMAL;
    }

__exit:

    return res;
}

s_err_t sensor_close(struct sensor_device* sensor)
{
 
    GSI_ASSERT(sensor != NULL);
    //diasble irq
   
    /* Configure power mode to power down mode */
    if (sensor->ops->control(sensor, SENSOR_CTRL_SET_POWER, (void *)SENSOR_POWER_DOWN) == 0)
    {
        sensor->config.power = SENSOR_POWER_DOWN;
    }

    /* Sensor disable interrupt */
//    if (sensor->config.irq_pin.pin != PIN_NONE)
//    {
//        rt_pin_irq_enable(sensor->config.irq_pin.pin, FALSE);
//    }
 
 
    return 0;
}

s_size_t sensor_read(struct sensor_device* sensor , void *buf , s_size_t len)
{
    s_size_t result = 0;
   	GSI_ASSERT(sensor != NULL);

    if (buf == NULL || len == 0)
    {
        return 0;
    }

    /* The buffer is not empty. Read the data in the buffer first */
    if (sensor->data_len > 0)
    {
        if (len > sensor->data_len / sizeof(struct sensor_data))
        {
            len = sensor->data_len / sizeof(struct sensor_data);
        }

        s_memcpy(buf, sensor->data_buf, len * sizeof(struct sensor_data));

        /* Clear the buffer */
        sensor->data_len = 0;
        result = len;
    }
    else
    {
        /* If the buffer is empty read the data */
        result = sensor->ops->fetch_data(sensor, buf, len);
    }
    return result;
}

static s_err_t sensor_control(struct sensor_device* sensor, int cmd, void *args)
{
    s_err_t result = 0;
    GSI_ASSERT(sensor != GSI_NULL);

 	//disable irq
    switch (cmd)
    {
    case SENSOR_CTRL_GET_ID:
        if (args)
        {
            sensor->ops->control(sensor, SENSOR_CTRL_GET_ID, args);
        }
        break;
    case SENSOR_CTRL_GET_INFO:
        if (args)
        {
            s_memcpy(args, &sensor->info, sizeof(struct sensor_info));
        }
        break;
    case SENSOR_CTRL_SET_RANGE:

        /* Configuration measurement range */
        result = sensor->ops->control(sensor, SENSOR_CTRL_SET_RANGE, args);
        if (result == 0)
        {
            sensor->config.range = (s_int32_t)args;
            LOG_D("set range %d", sensor->config.range);
        }
        break;
    case SENSOR_CTRL_SET_ODR:

        /* Configuration data output rate */
        result = sensor->ops->control(sensor, SENSOR_CTRL_SET_ODR, args);
        if (result == 0)
        {
            sensor->config.odr = (s_uint32_t)args & 0xFFFF;
            LOG_D("set odr %d", sensor->config.odr);
        }
        break;
    case SENSOR_CTRL_SET_POWER:

        /* Configuration sensor power mode */
        result = sensor->ops->control(sensor, SENSOR_CTRL_SET_POWER, args);
        if (result == 0)
        {
            sensor->config.power = (s_uint32_t)args & 0xFF;
            LOG_D("set power mode code:", sensor->config.power);
        }
        break;
    case SENSOR_CTRL_SELF_TEST:

        /* Device self-test */
        result = sensor->ops->control(sensor, SENSOR_CTRL_SELF_TEST, args);
        break;
    default:
        return -1;
    }

 	//enable irq
    return result;
}


/*
 * sensor register
 */
int hw_sensor_register(struct sensor_device* sensor,
                          const char              *name,
                          s_uint32_t              flag,
                          void                    *data)
{
    s_int8_t result = 0;
 
    GSI_ASSERT(sensor != NULL);

    char *sensor_name = NULL, *device_name = NULL;

    /* Add a type name for the sensor device */
    sensor_name = sensor_name_str[sensor->info.type];

    s_memcpy(device_name, sensor_name,  strlen(sensor_name) + 1);
    strcat(device_name, name);




    LOG_I("rt_sensor init success");
    return result;
}
