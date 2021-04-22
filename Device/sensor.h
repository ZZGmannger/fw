
#ifndef PIN_H
#define PIN_H

#include "gsi_def.h"


#define SENSOR_OBJ \
	 /*name             type                    data                             description*/ \
	X("none"  ,  SENSOR_CLASS_NONE      , s_uint8_t no_use) \
    X("acce_" ,  SENSOR_CLASS_ACCE      , struct sensor_3_axis acce)		/* Accelerometer     */ \
    X("gyro_" ,  SENSOR_CLASS_GYRO      , struct sensor_3_axis gyro)		/* Gyroscope         */ \
    X("mag_"  ,  SENSOR_CLASS_MAG       , struct sensor_3_axis magn)		/* Magnetometer      */ \
    X("temp_" ,  SENSOR_CLASS_TEMP      , s_int32_t            temp)		/* Temperature       */ \
    X("humi_" ,  SENSOR_CLASS_HUMI      , s_int32_t            humi)		/* Relative Humidity */ \
    X("baro_" ,  SENSOR_CLASS_BARO      , s_int32_t            baro)		/* Barometer         */ \
    X("li_"   ,  SENSOR_CLASS_LIGHT     , s_int32_t            light)		/* Ambient light     */ \
    X("pr_"   ,  SENSOR_CLASS_PROXIMITY , s_int32_t            proximity)	/* Proximity         */ \
    X("hr_"   ,  SENSOR_CLASS_HR        , s_int32_t            hr)		    /* Heart Rate        */ \
    X("tvoc_" ,  SENSOR_CLASS_TVOC      , s_int32_t            tvoc)		/* TVOC Level        */ \
    X("noi_"  ,  SENSOR_CLASS_NOISE     , s_int32_t            noise)		/* Noise Loudness    */ \
    X("step_" ,  SENSOR_CLASS_STEP      , s_uint32_t           step)		/* Step sensor       */ \
    X("forc_" ,  SENSOR_CLASS_FORCE     , s_int32_t            force)		/* Force sensor      */ \
    /*----------------------------------add new sensor below-------------------------------------*/ \
	  
#define X(a, b ,c)	 b,  
typedef enum
{
	SENSOR_OBJ
}SENSOR_TYPE;
#undef X

/* Sensor vendor types */
#define SENSOR_VENDOR_UNKNOWN       (0)
#define SENSOR_VENDOR_STM           (1)  /* STMicroelectronics */
#define SENSOR_VENDOR_BOSCH         (2)  /* Bosch */
#define SENSOR_VENDOR_INVENSENSE    (3)  /* Invensense */
#define SENSOR_VENDOR_SEMTECH       (4)  /* Semtech */
#define SENSOR_VENDOR_GOERTEK       (5)  /* Goertek */
#define SENSOR_VENDOR_MIRAMEMS      (6)  /* MiraMEMS */
#define SENSOR_VENDOR_DALLAS        (7)  /* Dallas */

/* Sensor unit types */
#define  SENSOR_UNIT_NONE           (0)
#define  SENSOR_UNIT_MG             (1)  /* Accelerometer           unit: mG         */
#define  SENSOR_UNIT_MDPS           (2)  /* Gyroscope               unit: mdps       */
#define  SENSOR_UNIT_MGAUSS         (3)  /* Magnetometer            unit: mGauss     */
#define  SENSOR_UNIT_LUX            (4)  /* Ambient light           unit: lux        */
#define  SENSOR_UNIT_CM             (5)  /* Distance                unit: cm         */
#define  SENSOR_UNIT_PA             (6)  /* Barometer               unit: pa         */
#define  SENSOR_UNIT_PERMILLAGE     (7)  /* Relative Humidity       unit: permillage */
#define  SENSOR_UNIT_DCELSIUS       (8)  /* Temperature             unit: dCelsius   */
#define  SENSOR_UNIT_HZ             (9)  /* Frequency               unit: HZ         */
#define  SENSOR_UNIT_ONE            (10) /* Dimensionless quantity  unit: 1          */
#define  SENSOR_UNIT_BPM            (11) /* Heart rate              unit: bpm        */
#define  SENSOR_UNIT_MM             (12) /* Distance                unit: mm         */
#define  SENSOR_UNIT_MN             (13) /* Force                   unit: mN         */

/* Sensor communication interface types */
#define  SENSOR_INTF_I2C            (1 << 0)
#define  SENSOR_INTF_SPI            (1 << 1)
#define  SENSOR_INTF_UART           (1 << 2)
#define  SENSOR_INTF_ONEWIRE        (1 << 3)
#define  SENSOR_INTF_ADC            (1 << 4)
#define  SENSOR_INTF_IO             (1 << 5)

/* Sensor power mode types */
#define  SENSOR_POWER_NONE          (0)
#define  SENSOR_POWER_DOWN          (1)  /* power down mode   */
#define  SENSOR_POWER_NORMAL        (2)  /* normal-power mode */
#define  SENSOR_POWER_LOW           (3)  /* low-power mode    */
#define  SENSOR_POWER_HIGH          (4)  /* high-power mode   */

/* Sensor mdoe types */
#define  SENSOR_MODE_NONE           (0)
#define  SENSOR_MODE_POLLING        (0)  /* One shot only read a data */
#define  SENSOR_MODE_INT            (1)  /* TODO: One shot interrupt only read a data */
#define  SENSOR_MODE_FIFO           (2)  /* TODO: One shot interrupt read all fifo data */

/* Sensor control cmd types */
#define  SENSOR_CTRL_GET_ID         (0)  /* Get device id */
#define  SENSOR_CTRL_GET_INFO       (1)  /* Get sensor info */
#define  SENSOR_CTRL_SET_RANGE      (2)  /* Set the measure range of sensor. unit is info of sensor */
#define  SENSOR_CTRL_SET_ODR        (3)  /* Set output date rate. unit is HZ */
#define  SENSOR_CTRL_SET_MODE       (4)  /* Set sensor's work mode. ex. SENSOR_MODE_POLLING,SENSOR_MODE_INT */
#define  SENSOR_CTRL_SET_POWER      (5)  /* Set power mode. args type of sensor power mode. ex. SENSOR_POWER_DOWN,SENSOR_POWER_NORMAL */
#define  SENSOR_CTRL_SELF_TEST      (6)  /* Take a self test */

struct sensor_info
{
    s_uint8_t     type;                    /* The sensor type */
    s_uint8_t     vendor;                  /* Vendor of sensors */
    const char    *name;                   /* model name of sensor */
    s_uint8_t     unit;                    /* unit of measurement */
    s_uint8_t     intf_type;               /* Communication interface type */
    s_int32_t     range_max;               /* maximum range of this sensor's value. unit is 'unit' */
    s_int32_t     range_min;               /* minimum range of this sensor's value. unit is 'unit' */
	s_int32_t     sensitivity;             /* sensitivity , out change / input change*/
	s_int32_t     precision;               /* precision   , the measured output +- precision  is actul value */
	s_int32_t     resolution;			   /* resolution  , How much input causes the output to change */
};

struct sensor_intf
{
    char                       *dev_name;   /* The name of the communication device */
    s_uint8_t                  type;       /* Communication interface type */
    void                       *user_data;  /* Private data for the sensor. ex. i2c addr,spi cs,control I/O */
};

struct sensor_config
{
    struct sensor_intf        intf;      /* sensor interface config */
 //   struct device_pin_mode    irq_pin;   /* Interrupt pin, The purpose of this pin is to notification read data */
    s_uint8_t                   mode;      /* sensor work mode */
    s_uint8_t                   power;     /* sensor power mode */
    s_uint16_t                  odr;       /* sensor out data rate */
    s_int32_t                   range;     /* sensor range of measurement */
};

struct sensor_device
{
  	struct sensor_device* next;
    struct sensor_info        info;      /* The sensor info data */
    struct sensor_config      config;    /* The sensor config data */
	
	s_uint16_t init_flag;
	s_uint16_t open_flag;
	
    void                      *data_buf;  /* The buf of the data received */
    s_size_t                   data_len;  /* The size of the data received */

    const struct sensor_ops  *ops;       /* The sensor ops */
    int (*rx_indicate)(struct sensor_device* sensor, s_uint16_t size);
    s_err_t (*irq_handle)(struct sensor_device* sensor);             /* Called when an interrupt is generated, registered by the driver */
};

/* 3-axis Data Type */
struct sensor_3_axis
{
    s_int32_t x;
    s_int32_t y;
    s_int32_t z;
};
struct sensor_data
{
    s_uint32_t         timestamp;          /* The timestamp when the data was received */
    s_uint8_t          type;               /* The sensor type of the data */
    union
    {
	  	#define X(a, b ,c)	 c; 
	  	SENSOR_OBJ
      	#undef X
    }data;
};


struct sensor_ops
{
    s_size_t (*fetch_data)(struct sensor_device *sensor, void *buf, s_size_t len);
    s_err_t (*control)(struct sensor_device *sensor, int cmd, void *arg);
};

int hw_sensor_register(struct sensor_device* sensor,
                          const char              *name,
                          s_uint32_t              flag,
                          void                    *data);

#endif