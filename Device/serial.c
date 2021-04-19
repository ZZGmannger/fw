
#include "serial.h"
 

static struct serial_device _hw_serial;

#define SER_ASSERT(...)

/*---------------------------------------------------------------------------*/
/*
 * Serial poll routines
 */
static inline int _serial_poll_rx(struct serial_device *serial, uint8_t *data, int length)
{
    int ch;
    int size;

    SER_ASSERT(serial != RT_NULL);
    size = length;

    while (length)
    {
        ch = serial->ops->getc(serial);
        if (ch == -1) break;

        *data = ch;
        data ++; 
		length --;
    }

    return size - length;
}

static inline int _serial_poll_tx(struct serial_device *serial, const uint8_t *data, int length)
{
    int size;
    SER_ASSERT(serial != RT_NULL);

    size = length;
    while (length)
    {
        serial->ops->putc(serial, *data);

        ++ data;
        -- length;
    }

    return size - length;
}

/*
 * Serial interrupt routines
 */
static inline int _serial_int_rx(struct serial_device *serial, uint8_t *data, int length)
{
    int size;
 
    SER_ASSERT(serial != NULL);
    size = length;

    /* read from software FIFO */
    while (length)
    {
        uint8_t ch;
    
        /* disable interrupt */
        hw_interrupt_disable();

        /* there's no data: */
		if(0 == ringbuffer_data_len(&serial->rx_fifo_rb))
        {
            /* no data, enable interrupt and break out */
            hw_interrupt_enable();
            break;
        }

        /* otherwise there's the data: */
		ringbuffer_getchar(&serial->rx_fifo_rb , &ch);
		
        /* enable interrupt */
        hw_interrupt_enable();

        *data = ch & 0xff;
        data ++; length --;
    }

    return size - length;
}

static inline int _serial_int_tx(struct serial_device *serial, const uint8_t *data, int length)
{
    int size;
 
    SER_ASSERT(serial != RT_NULL);

    size = length;
	
	if(0 == ringbuffer_data_len(&serial->tx_fifo_rb))
	{
		serial->ops->putc(serial , *data);
		length--;
		data++;
	}
 	length -= ringbuffer_put(&serial->tx_fifo_rb, data , length);
	
    return size - length;
}


/*
 * Serial dma routines
 */	
static inline int _serial_dma_rx(struct serial_device *serial, uint8_t *data, int length)
{
    SER_ASSERT((serial != NULL) && (data != NULL));

    hw_interrupt_disable();

    if (serial->config.rxbufsz == 0)
    {
        int result = 0;
        //struct rt_serial_rx_dma *rx_dma;

        //rx_dma = (struct rt_serial_rx_dma*)serial->serial_rx;
        SER_ASSERT(rx_dma != RT_NULL);

        if (1)//rx_dma->activated != RT_TRUE)
        {
            //rx_dma->activated = RT_TRUE;
            SER_ASSERT(serial->ops->dma_transmit != RT_NULL);
            serial->ops->dma_transmit(serial, data, length, SERIAL_DMA_RX);
        }
        else 
		  result = -1;
        hw_interrupt_enable();

        if (result == 0) return length;

        return 0;
    }
    else
    {
        uint16_t recv_len = 0;
		uint16_t fifo_recved_len = ringbuffer_data_len(&serial->rx_fifo_rb);;
 
        if (length < (int)fifo_recved_len)
            recv_len = length;
        else
            recv_len = fifo_recved_len;

		recv_len = ringbuffer_get(&serial->rx_fifo_rb , data , recv_len);
 
        hw_interrupt_enable();
        return recv_len;
    }
}

static inline int _serial_dma_tx(struct serial_device *serial, const uint8_t *data, int length)
{
    int result;

    result = data_queue_push(&serial->tx_data_queue, data, length);
    if (result == 0)
    {
        hw_interrupt_disable();
        if (1)//tx_dma->activated != RT_TRUE)
        {
//            tx_dma->activated = RT_TRUE;
            hw_interrupt_enable();

            /* make a DMA transfer */
            serial->ops->dma_transmit(serial, (uint8_t *)data, length , SERIAL_DMA_TX);
        }
        else
        {
            hw_interrupt_enable();
        }

        return length;
    }
    else
    {
        return 0;
    }
}


/*---------------------------------------------------------------------------*/
/*
 * serial register
 */
int hw_serial_register(struct serial_device            *serial,
                               const char              *name,
							   const struct uart_ops   *ops,
                               uint32_t                flag,
                               void                   *data)						   
{
    int ret = 0;
	struct serial_device*  cur_serial = &_hw_serial;
    
    serial->name = name;
	serial->init_flag = flag;
	serial->tx_complete = NULL;
	serial->rx_indicate = NULL;
	serial->ops = ops;
	
	/*insert serial to hw_serial*/
	while(cur_serial->next != NULL)
	{
		cur_serial = cur_serial->next;
	}
	cur_serial->next = serial;
	serial->next = NULL;
	  
    return ret;
}

struct serial_device* serial_find(const char* name)
{
    struct serial_device*  cur_serial = &_hw_serial;
	
	while(cur_serial->next != NULL)
	{
		cur_serial = cur_serial->next;
		if(0 == strcmp(cur_serial->name , name))
		{
			return cur_serial;
		}
	}
	
	return NULL;
}

int serial_init(struct serial_device* serial)
{
    int result =0;
    
    SER_ASSERT(serial);
    
    /* initialize rx/tx */
	serial->rx_buffer = NULL;
    serial->tx_buffer = NULL;
	 
    if(serial->ops->configure)
    {
        result = serial->ops->configure(serial, &serial->config);
    }
    
    return result;
}

int serial_open(struct serial_device* serial, uint16_t oflag)
{
    SER_ASSERT(serial);
    
    /* check device flag with the open flag */
    if ((oflag & SERIAL_FLAG_DMA_RX) && !(serial->init_flag & SERIAL_FLAG_DMA_RX))
        return -1;
    if ((oflag & SERIAL_FLAG_DMA_TX) && !(serial->init_flag & SERIAL_FLAG_DMA_TX))
        return -1;
    if ((oflag & SERIAL_FLAG_INT_RX) && !(serial->init_flag & SERIAL_FLAG_INT_RX))
        return -1;
    if ((oflag & SERIAL_FLAG_INT_TX) && !(serial->init_flag & SERIAL_FLAG_INT_TX))
        return -1;
    
    serial->open_flag = oflag & 0xff;
    
	/*rx*/
	if (oflag & SERIAL_FLAG_INT_RX)
	{   
		ringbuffer_init(&serial->rx_fifo_rb, serial->rx_buffer , serial->config.rxbufsz);
		
		serial->open_flag |= SERIAL_FLAG_INT_RX;
		/* configure low level device */
		serial->ops->control(serial, DEVICE_CTRL_SET_INT, (void *)SERIAL_FLAG_INT_RX);
	}
	else if (oflag & SERIAL_FLAG_DMA_RX)
	{
		ringbuffer_init(&serial->rx_fifo_rb, serial->rx_buffer , serial->config.rxbufsz);

		serial->open_flag |= SERIAL_FLAG_DMA_RX;
		/* configure low level device */
		serial->ops->control(serial , DEVICE_CTRL_CONFIG, (void *)SERIAL_FLAG_DMA_RX);
	}

	/*tx*/
	if (oflag & SERIAL_FLAG_INT_TX)
	{
		ringbuffer_init(&serial->tx_fifo_rb, serial->tx_buffer , serial->config.txbufsz);
		
		serial->open_flag |= SERIAL_FLAG_INT_TX;
		/* configure low level device */
		serial->ops->control(serial, DEVICE_CTRL_SET_INT, (void *)SERIAL_FLAG_INT_TX);
	}
	else if (oflag & SERIAL_FLAG_DMA_TX)
	{
		data_queue_init(&serial->tx_data_queue , serial->tx_buffer , serial->config.txbufsz);  
		serial->open_flag |= SERIAL_FLAG_DMA_TX;
		/* configure low level device */
		serial->ops->control(serial, DEVICE_CTRL_CONFIG, (void *)SERIAL_FLAG_DMA_TX);
	}
    return 0;
}

int serial_control(struct serial_device* serial,
                      int              cmd,
                      void             *args)
{
    int ret = 0;
    SER_ASSERT(serial != RT_NULL);
   

    switch (cmd)
    {
        case DEVICE_CTRL_SUSPEND:
            /* suspend device */
            serial->init_flag |= DEVICE_FLAG_SUSPENDED;
            break;

        case DEVICE_CTRL_RESUME:
            /* resume device */
            serial->init_flag &= ~DEVICE_FLAG_SUSPENDED;
            break;

        case DEVICE_CTRL_CONFIG:
            if (args)
            {
                struct serial_configure *pconfig = (struct serial_configure *) args;
                if (pconfig->rxbufsz != serial->config.rxbufsz && serial->ref_count)
                {
                    /*can not change buffer size*/
                    return -1;
                }
                /* set serial configure */
                serial->config = *pconfig;
                if (serial->ref_count)
                {
                    /* serial device has been opened, to configure it */
                    serial->ops->configure(serial, (struct serial_configure *) args);
                }
            }

            break;
 
        default :
            /* control device */
            ret = serial->ops->control(serial, cmd, args);
            break;
    }

    return ret;
}
int serial_close(struct serial_device* serial)
{
    SER_ASSERT(serial);
    if (serial->open_flag & SERIAL_FLAG_INT_RX)
    {
      serial->open_flag &= ~SERIAL_FLAG_INT_RX;
    }
    else if (serial->open_flag & SERIAL_FLAG_DMA_RX)
    {
        serial->open_flag &= ~SERIAL_FLAG_DMA_RX;
    }
    
    if (serial->open_flag & SERIAL_FLAG_INT_TX)
    {
        serial->open_flag &= ~SERIAL_FLAG_INT_TX;
    }
    else if (serial->open_flag & SERIAL_FLAG_DMA_TX)
    {
        serial->open_flag &= ~SERIAL_FLAG_DMA_TX;
    }
    return 0;
}
uint32_t serial_read(struct serial_device* serial,void *buffer, uint32_t size)
{
    if (size == 0) return 0;
    
    if (serial->open_flag & SERIAL_FLAG_INT_RX)
    {
        return _serial_int_rx(serial, (uint8_t *)buffer, size);
    }
 
    else if (serial->open_flag & SERIAL_FLAG_DMA_RX)
    {
        return _serial_dma_rx(serial, (uint8_t *)buffer, size);
    }
   
    return _serial_poll_rx(serial, (uint8_t *)buffer, size); 
}

uint32_t serial_write(struct serial_device* serial,const void *buffer,uint32_t size)                       
{
   
    if (size == 0) return 0;

    if (serial->open_flag & SERIAL_FLAG_INT_TX)
    {
        return _serial_int_tx(serial, (const uint8_t *)buffer, size);
    }
  
    else if (serial->open_flag & SERIAL_FLAG_DMA_TX)
    {
        return _serial_dma_tx(serial, (const uint8_t *)buffer, size);
    }
    else
    {
        return _serial_poll_tx(serial, (const uint8_t *)buffer, size);
    }
}
int serial_set_tx_complete(struct serial_device* serial, 
						   int (*tx_done)(struct serial_device* serial,void *buffer))
{
	SER_ASSERT(serial);
	
	serial->tx_complete = tx_done;
	return 0;
}


int serial_set_rx_indicate(struct serial_device* serial, 
						   int (*rx_ind)(struct serial_device* serial,uint16_t size))
{
	SER_ASSERT(serial);
	serial->rx_indicate = rx_ind;
	return 0;
}

/* ISR for serial interrupt */
void hw_serial_isr(struct serial_device *serial, int event)
{
    switch (event & 0xff)
    {
        case SERIAL_EVENT_RX_IND:
        {
            int ch = -1;
           
            while (1)
            {
                ch = serial->ops->getc(serial);
                if (ch == -1) 
				    break;

                /* disable interrupt */
                hw_interrupt_disable();
				
				ringbuffer_putchar_force(&serial->rx_fifo_rb , ch);
		
                /* enable interrupt */
                hw_interrupt_enable();
            }

            /* invoke callback */
            if (serial->rx_indicate != NULL)
            {
                uint16_t rx_length;

                /* get rx length */
                hw_interrupt_disable();
                rx_length = ringbuffer_data_len(&serial->rx_fifo_rb);
                hw_interrupt_enable();

                if (rx_length)
                {
                    serial->rx_indicate(serial, rx_length);
                }
            }
            break;
        }
        case SERIAL_EVENT_TX_DONE:
        {
		    uint8_t ch;
 			if(ringbuffer_getchar(&serial->tx_fifo_rb , &ch))
			{
				serial->ops->putc(serial , ch);
			}
            break;
        }
        case SERIAL_EVENT_TX_DMADONE:
        {
            const void *data_ptr;
            uint16_t data_size;
            const void *last_data_ptr;

            data_queue_pop(&serial->tx_data_queue , &last_data_ptr, &data_size);
            if (data_queue_peak(&serial->tx_data_queue, &data_ptr, &data_size) == 0)
            {
                /* transmit next data node */
                //tx_dma->activated = RT_TRUE;
                serial->ops->dma_transmit(serial, (uint8_t *)data_ptr, data_size, SERIAL_DMA_TX);
            }
            else
            {
               //tx_dma->activated = RT_FALSE;
            }

            /* invoke callback */
            if (serial->tx_complete != NULL)
            {
                serial->tx_complete(serial, (void*)last_data_ptr);
            }
            break;
        }
        case SERIAL_EVENT_RX_DMADONE:
        {
            int length;

            /* get DMA rx length */
            length = (event & (~0xff)) >> 8;

            if (serial->config.rxbufsz == 0)
            {
                //struct rt_serial_rx_dma* rx_dma;

               // rx_dma = (struct rt_serial_rx_dma*) serial->serial_rx;
                SER_ASSERT(rx_dma != RT_NULL);

                SER_ASSERT(serial->rx_indicate != NULL);
                serial->rx_indicate(serial, length);
                //rx_dma->activated = RT_FALSE;
            }
            else
            {
                /* disable interrupt */
                hw_interrupt_disable();
                /* update fifo put index */
				
				ringbuffer_updata_put_index_force(&serial->rx_fifo_rb , length);
				
                /* calculate received total length */
                length = ringbuffer_data_len(&serial->rx_fifo_rb);
                /* enable interrupt */
                hw_interrupt_enable();
                /* invoke callback */
                if (serial->rx_indicate != NULL)
                {
                    serial->rx_indicate(serial, length);
                }
            }
            break;
        }
    }
}

