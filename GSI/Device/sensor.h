
#ifndef PIN_H
#define PIN_H

#include "gsi_def.h"


#define SENSOR_PHY_TYPE \
	 /*name             type                  physical  data                     description*/ \
	X("none" ,  SENSOR_CLASS_NONE      , s_uint8_t no_use) \
    X("acce" ,  SENSOR_CLASS_ACCE      , struct sensor_3_axis acce)		/* Accelerometer     */ \
    X("temp" ,  SENSOR_CLASS_TEMP      , s_int32_t            temp)		/* Temperature       */ \
    X("humi" ,  SENSOR_CLASS_HUMI      , s_int32_t            humi)		/* Relative Humidity */ \
    X("baro" ,  SENSOR_CLASS_BARO      , s_int32_t            baro)		/* Barometer         */ \
    X("li"   ,  SENSOR_CLASS_LIGHT     , s_int32_t            light)	/* Ambient light     */ \
    X("pr"   ,  SENSOR_CLASS_PROXIMITY , s_int32_t            proximity)/* Proximity         */ \
    X("tvoc" ,  SENSOR_CLASS_TVOC      , s_int32_t            tvoc)		/* TVOC Level        */ \
    X("noi"  ,  SENSOR_CLASS_NOISE     , s_int32_t            noise)	/* Noise Loudness    */ \
    X("step" ,  SENSOR_CLASS_STEP      , s_uint32_t           step)		/* Step sensor       */ \
    X("forc" ,  SENSOR_CLASS_FORCE     , s_int32_t            force)	/* Force sensor      */ \
    /*----------------------------------add new sensor below-------------------------------------*/ \
	X("door" ,  SENSOR_CLASS_IO_DOOR   , s_uint32_t             door)	    /* door sensor   */ \
	X("water" , SENSOR_CLASS_IO_WATER  , s_uint32_t             water)    /* water sensor    */ \
 
/* Sensor vendor types */	  
#define SENSOR_VENDOR	\
	X("unknow"		, SENSOR_VENDOR_UNKNOWN        )  \
	X("STM" 		, SENSOR_VENDOR_STM            )  		/* STMicroelectronics */ \
	X("Bosch" 		, SENSOR_VENDOR_BOSCH          )  	/* Bosch */ 			 \
	X("Invensense" 	, SENSOR_VENDOR_INVENSENSE      )	/* Invensense */ 		 \
	X("Semtech" 	, SENSOR_VENDOR_SEMTECH        )  	/* Semtech */ 			 \
	X("Goertek" 	, SENSOR_VENDOR_GOERTEK          )  	/* Goertek */ 			 \
	X("MiraMEMS" 	, SENSOR_VENDOR_MIRAMEMS      )  	/* MiraMEMS */ 			 \
	X("Dallas" 		, SENSOR_VENDOR_DALLAS         )  	/* Dallas */ 			 \
    /*----------------------add new vendor below----------------------*/ \
	  
	  
#define SENSOR_INTF \
	X("unknow" 		, SENSOR_INTF_UNKNOWN      )	\
	X("I2C" 		, SENSOR_INTF_I2C         ) 		\
	X("SPI" 		, SENSOR_INTF_SPI         ) 		\
	X("UART" 		, SENSOR_INTF_UART        ) 	\
	X("CAN" 		, SENSOR_INTF_CAN           ) 	\
	X("ONEWIRE" 	, SENSOR_INTF_ONEWIRE        ) 	\
	X("ADC" 		, SENSOR_INTF_ADC          )		\
	X("IO" 			, SENSOR_INTF_IO          )		\
	 /*------------------add new interface below----------------------*/ \

	     
/*enum sensor type*/	  
#define X(a, b ,c)	 b,  
typedef enum
{
	SENSOR_PHY_TYPE
	SENSOR_PHY_TYPE_MAX
}SENSOR_TYPE_t;
#undef X
extern char *const sensor_type_str[];    
#define SENSOR_TYPE_INFO(x)   sensor_type_str[((x)<SENSOR_PHY_TYPE_MAX ? (x):SENSOR_CLASS_NONE)] 

/*enum sensor vendor*/	
#define X(a,b) b,
typedef enum
{
	SENSOR_VENDOR
	SENSOR_VENDOR_MAX
}SENSOR_VENDOR_t;
#undef X
extern char *const sensor_vendor_str[];
#define SENSOR_VENDOR_INFO(x)   sensor_vendor_str[((x)<SENSOR_VENDOR_MAX ? (x):SENSOR_VENDOR_UNKNOWN)] 

/*enum sensor interface*/	
#define X(a,b)   b,
typedef enum
{
	SENSOR_INTF
	SENSOR_INTF_MAX
}SENSOR_INTF_t;
#undef X
extern char *const sensor_intf_str[];
#define SENSOR_INTF_INFO(x)   sensor_intf_str[(x)<SENSOR_INTF_MAX ?(x):SENSOR_INTF_UNKNOWN]



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

#define  SENSOR_CTRL_SET_RESET      (2)  /* Get device id */
#define  SENSOR_CTRL_SET_ARGC       (2)  /* Get device id */
#define  SENSOR_CTRL_SET_RANGE      (3)  /* Set the measure range of sensor. unit is info of sensor */
#define  SENSOR_CTRL_SET_ODR        (4)  /* Set output date rate. unit is HZ */
#define  SENSOR_CTRL_SET_MODE       (5)  /* Set sensor's work mode. ex. SENSOR_MODE_POLLING,SENSOR_MODE_INT */
#define  SENSOR_CTRL_SET_POWER      (6)  /* Set power mode. args type of sensor power mode. ex. SENSOR_POWER_DOWN,SENSOR_POWER_NORMAL */
#define  SENSOR_CTRL_SELF_TEST      (7)  /* Take a self test */
#define  SENSOR_CTRL_SET_IND        (8)

/* Sensor open flag types */
#define SENSOR_FLAG_RDONLY     (0x0001)
#define SENSOR_FLAG_INT_RX     (0x0002)
#define SENSOR_FLAG_FIFO_RX    (0x0004)

struct sensor_parameter
{
	s_uint8_t	  type;                    /* The sensor type*/
	s_uint8_t     unit;                    /* unit of measurement */
	s_int32_t     range_max;               /* maximum range of this sensor's value. unit is 'unit' */
	s_int32_t     range_min;               /* minimum range of this sensor's value. unit is 'unit' */
	s_int32_t     sensitivity;             /* sensitivity , out change / input change*/
	s_int32_t     precision;               /* precision   , the measured output +- precision  is actul value */
	s_int32_t     resolution;			   /* resolution  , How much input causes the output to change */
};

struct sensor_info
{
	const char    *name;                   /* model name of sensor */
	s_uint8_t     vendor;                  /* Vendor of sensors */
    s_uint8_t     intf_type;               /* Communication interface type */	
	s_uint8_t     argc;                    /*indicate the sensor param num*/
	struct sensor_parameter* param;
};

struct sensor_config
{
	// struct sensor_intf         intf;      /* sensor interface config */
    s_uint8_t                   argc;
    s_uint8_t                   mode;      /* sensor work mode */
    s_uint8_t                   power;     /* sensor power mode */
    s_uint16_t                  odr;       /* sensor out data rate */
    s_int32_t                   range;     /* sensor range of measurement */
};

struct sensor_device
{
  	struct sensor_device*     next;
    struct sensor_info        info;      /* The sensor info data */
    struct sensor_config      config;    /* The sensor config data */
	
	s_uint32_t status;
	
	s_uint16_t init_flag;
	s_uint16_t open_flag;
	
    const struct sensor_ops  *ops;       /* The sensor ops */
    void (*rx_indicate)(struct sensor_device* sensor);             /* Called when an interrupt is generated, registered by the driver */
};

typedef void (*rx_indicate_func_t)(struct sensor_device* sensor);  

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
	  	SENSOR_PHY_TYPE
      	#undef X
    }data;
};


struct sensor_ops
{
    s_size_t (*fetch_data)(struct sensor_device *sensor, void *buf, s_size_t len);
    s_err_t (*control)(struct sensor_device *sensor, int cmd, void *arg);
};


struct sensor_device* sensor_find(const char* name);
struct sensor_device* sensor_list(void);
s_err_t sensor_open(struct sensor_device* sensor , s_uint16_t oflag);
s_err_t sensor_close(struct sensor_device* sensor);
s_size_t sensor_read(struct sensor_device* sensor , void *buf , s_size_t len);
s_err_t sensor_control(struct sensor_device* sensor, int cmd, void *args);



int hw_sensor_register(struct sensor_device* sensor,
                          const char              *name,
                          s_uint32_t              flag,
                          void                    *data);



#endif