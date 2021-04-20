#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "main.h"
#include "board.h"
#include "dataqueue.h"
#include "ringbuffer.h"

#define BAUD_RATE_2400                  2400
#define BAUD_RATE_4800                  4800
#define BAUD_RATE_9600                  9600
#define BAUD_RATE_19200                 19200
#define BAUD_RATE_38400                 38400
#define BAUD_RATE_57600                 57600
#define BAUD_RATE_115200                115200
#define BAUD_RATE_230400                230400
#define BAUD_RATE_460800                460800
#define BAUD_RATE_921600                921600
#define BAUD_RATE_2000000               2000000
#define BAUD_RATE_3000000               3000000

#define DATA_BITS_5                     5
#define DATA_BITS_6                     6
#define DATA_BITS_7                     7
#define DATA_BITS_8                     8
#define DATA_BITS_9                     9

#define STOP_BITS_1                     0
#define STOP_BITS_2                     1
#define STOP_BITS_3                     2
#define STOP_BITS_4                     3

#define PARITY_NONE                     0
#define PARITY_ODD                      1
#define PARITY_EVEN                     2

#define BIT_ORDER_LSB                   0
#define BIT_ORDER_MSB                   1

#define NRZ_NORMAL                      0       /* Non Return to Zero : normal mode */
#define NRZ_INVERTED                    1       /* Non Return to Zero : inverted mode */


#define SERIAL_EVENT_RX_BYTE_IND	 0x01    /* Rx indication */
#define SERIAL_EVENT_RX_IDLE_IND	 0x02    /* Rx indication */
#define SERIAL_EVENT_TX_DONE         0x03    /* Tx complete   */
#define SERIAL_EVENT_RX_DMAHALF      0x04    /* Rx DMA transfer done */
#define SERIAL_EVENT_RX_DMADONE      0x05    /* Rx DMA transfer done */
#define SERIAL_EVENT_TX_DMADONE      0x06    /* Tx DMA transfer done */
#define SERIAL_EVENT_RX_TIMEOUT      0x07    /* Rx timeout    */

#define SERIAL_DMA_RX                0x01
#define SERIAL_DMA_TX                0x02

#define SERIAL_RX_INT                0x01
#define SERIAL_TX_INT                0x02

#define SERIAL_ERR_OVERRUN           0x01
#define SERIAL_ERR_FRAMING           0x02
#define SERIAL_ERR_PARITY            0x03

#define SERIAL_TX_DATAQUEUE_SIZE     2048
#define SERIAL_TX_DATAQUEUE_LWM      30



#define SERIAL_FLAG_INT_BYTE_RX     0x100           /**< Byte INT mode on Rx */
#define SERIAL_FLAG_INT_IDLE_RX     0x1000			/**< IDLE INT mode on Rx */
#define SERIAL_FLAG_DMA_RX        	0x200           /**< DMA mode on Rx */

#define SERIAL_FLAG_INT_TX        	0x400           /**< INT mode on Tx */
#define SERIAL_FLAG_DMA_TX        	0x800           /**< DMA mode on Tx */

#define DEVICE_CTRL_CONFIG           0x03            /**< configure device */
#define DEVICE_CTRL_SET_INT          0x10            /**< set interrupt */
#define DEVICE_CTRL_CLR_INT          0x11            /**< clear interrupt */
#define DEVICE_CTRL_GET_INT          0x12            /**< get interrupt status */

/* Default config for serial_configure structure */
#define SERIAL_RB_BUFSZ     1024
#define SERIAL_TB_BUFSZ     1024

#define SERIAL_CONFIG_DEFAULT           \
{                                          \
    BAUD_RATE_115200,   /* 115200 bits/s */  \
	SERIAL_RB_BUFSZ, 	/* Buffer size */  \
	SERIAL_TB_BUFSZ, 	/* Buffer size */  \
    DATA_BITS_8,        /* 8 databits */     \
    STOP_BITS_1,        /* 1 stopbit */      \
    PARITY_NONE,        /* No parity  */     \
    BIT_ORDER_LSB,      /* LSB first sent */ \
    NRZ_NORMAL,         /* Normal mode */    \
    0                                      \
}

struct serial_configure
{
    uint32_t baud_rate;
	
	uint16_t rxbufsz;
	uint16_t txbufsz;
	
    uint32_t data_bits               :4;
    uint32_t stop_bits               :2;
    uint32_t parity                  :2;
    uint32_t bit_order               :1;
    uint32_t invert                  :1;
    uint32_t reserved                :23;
};

struct serial_device
{
    struct serial_device* next;
    
    const char* name;    
    const struct uart_ops *ops;
    struct serial_configure   config;
    
    int (*rx_indicate)(struct serial_device* serial, uint16_t size);
    int (*tx_complete)(struct serial_device* serial, void *buffer);
    
    uint16_t init_flag;
    uint16_t open_flag;
	
    uint16_t ref_count;
    uint8_t tx_dma_activated;
    uint8_t rx_dma_activated;
	
	struct ringbuffer rx_fifo_rb;
	struct ringbuffer tx_fifo_rb;
	
	struct data_queue tx_data_queue;
	
	uint8_t rx_buffer[SERIAL_RB_BUFSZ];
    uint8_t tx_buffer[SERIAL_TB_BUFSZ];
};

/**
 * uart operators
 */
struct uart_ops
{
    int (*configure)(struct serial_device *serial, struct serial_configure *cfg);
    int (*control)(struct serial_device *serial, int cmd, void *arg);

    int (*putc)(struct serial_device *serial, char c);
    int (*getc)(struct serial_device *serial);

    uint32_t (*dma_transmit)(struct serial_device *serial, uint8_t *buf, uint32_t size, int direction);
};

/*
lowlayer API
*/
void hw_serial_isr(struct serial_device *serial, int event);

int hw_serial_register(struct serial_device            *serial,
                               const char              *name,
                               uint32_t                flag,
                               void                   *data);


					   
/*
uplayer API
*/			
struct serial_device* serial_find(const char* name);

int serial_open(struct serial_device* serial, uint16_t oflag);

int serial_control(struct serial_device* serial , int cmd , void *args);
                     
int serial_close(struct serial_device* serial);

uint32_t serial_read(struct serial_device* serial , void *buffer , uint32_t size);
                       
uint32_t serial_write( struct serial_device* serial , const void *buffer,uint32_t size);
                          
int serial_set_tx_complete(struct serial_device* serial, 
						   int (*tx_done)(struct serial_device* serial,void *buffer));
 
int serial_set_rx_indicate(struct serial_device* serial, 
						   int (*rx_ind)(struct serial_device* serial,uint16_t size));


#endif