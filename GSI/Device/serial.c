
#include "serial.h"
 
static struct serial_device _hw_serial;

#define SER_LOG(...)

/*
 * Serial poll routines
 */
GSI_INLINE int _serial_poll_rx(struct serial_device *serial , 
                                   s_uint8_t *data , 
                                   int length)
{
    int ch;
    int size;

    GSI_ASSERT(serial != GSI_NULL);
    size = length;

    while (length)
    {
        ch = serial->ops->getc(serial);
        if (ch == -1) 
        {
            break;
        }
        *data = ch & 0xff;
        ++data; 
		--length;
    }

    return (size - length);
}

GSI_INLINE int _serial_poll_tx(struct serial_device *serial , 
                                  const s_uint8_t *data , 
                                  int length)
{
    int size;
    GSI_ASSERT(serial != GSI_NULL);

    size = length;
    while (length)
    {
        serial->ops->putc(serial, *data);

        ++data;
        --length;
    }

    return (size - length);
}

/*
 * Serial interrupt routines
 */
GSI_INLINE int _serial_int_rx(struct serial_device *serial, 
                                 s_uint8_t *data, 
                                 int length)
{
    int size;
 
    GSI_ASSERT(serial != NULL);
    size = length;

    /* read from software FIFO */
    while (length)
    {
        s_uint8_t ch;
    
        /* disable interrupt */
        interrupt_disable();

        /* there's no data: */
		if(0 == ringbuffer_data_len(&serial->rx_fifo_rb))
        {
            /* no data, enable interrupt and break out */
            interrupt_enable();
            break;
        }

        /* otherwise there's the data: */
		ringbuffer_getchar(&serial->rx_fifo_rb , &ch);
		
        /* enable interrupt */
        interrupt_enable();

        *data = ch;
        ++data; 
        --length;
    }

    return (size - length);
}

GSI_INLINE int _serial_int_tx(struct serial_device *serial, 
                                 const s_uint8_t *data, 
                                 int length)
{
    int size;
 
    GSI_ASSERT(serial != NULL);

    size = length;
    
	hw_interrupt_disable();
	if(!serial->tx_int_activated && 0 == ringbuffer_data_len(&serial->tx_fifo_rb))
	{
		serial->ops->putc(serial , *data);
        serial->tx_int_activated =1;
		--length;
		++data;
	}
    if(length)
    {
        length -= ringbuffer_put(&serial->tx_fifo_rb, data , length);
    }
 
    hw_interrupt_enable();
    
    return size - length;
}

/*
 * Serial dma routines
 */	
GSI_INLINE int _serial_dma_rx(struct serial_device *serial, s_uint8_t *data, int length)
{
    GSI_ASSERT((serial != NULL) && (data != NULL));

    hw_interrupt_disable();

    if (serial->config.rxbufsz == 0)
    {
        int result = 0;

        if (serial->rx_dma_activated != 1)
        {
            serial->rx_dma_activated = 1;
            GSI_ASSERT(serial->ops->dma_transmit != NULL);
            serial->ops->dma_transmit(serial, data, length, SERIAL_DMA_RX);
        }
        else 
        {
            result = -1;
        }
        hw_interrupt_enable();

        if (result == 0) 
        {
            return length;
        }
        return 0;
    }
    else
    {
        s_uint16_t recv_len = 0;
		s_uint16_t fifo_recved_len = ringbuffer_data_len(&serial->rx_fifo_rb);;
 
        if (length < (int)fifo_recved_len)
        {
            recv_len = length;
        }
        else
        {
            recv_len = fifo_recved_len;
        }

		recv_len = ringbuffer_get(&serial->rx_fifo_rb , data , recv_len);
 
        hw_interrupt_enable();
        return recv_len;
    }
}

GSI_INLINE int _serial_dma_tx(struct serial_device *serial, const s_uint8_t *data, int length)
{
    int result;

    result = data_queue_push(&serial->tx_data_queue, data, length);
    if (result == 0)
    {
        hw_interrupt_disable();
        if (serial->tx_dma_activated !=1)
        {
            serial->tx_dma_activated =1;
            hw_interrupt_enable();

            /* make a DMA transfer */
            serial->ops->dma_transmit(serial, (s_uint8_t *)data, length , SERIAL_DMA_TX);
        }
        else
        {
            hw_interrupt_enable();
        }

        return length;
    }
    else
    {
        SER_LOG("serial dma queue full %s",serial->name);
        return 0;
    }
}

/*
serial find
*/
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

static int serial_init(struct serial_device* serial)
{
    int result =0;
    
    GSI_ASSERT(serial);
    
    /* initialize rx/tx */
	serial->config.rxbufsz = SERIAL_RB_BUFSZ;
	serial->config.txbufsz = SERIAL_TB_BUFSZ;
	
    if(serial->ops->configure)
    {
        result = serial->ops->configure(serial, &serial->config);
    }
    
    return result;
}

int serial_open(struct serial_device* serial, s_uint16_t oflag)
{
    int result = 0;
    GSI_ASSERT(serial);
    
    result = serial_init(serial);
    if(0 != result)
    {   
        SER_LOG("initialize device:%s failed. The error code is %d\n",
                           serial->name, result);
        return result;
    }
    
    /* check device flag with the open flag */
    if ((oflag & SERIAL_FLAG_DMA_RX) && !(serial->init_flag & SERIAL_FLAG_DMA_RX))
        result = -1;
    if ((oflag & SERIAL_FLAG_DMA_TX) && !(serial->init_flag & SERIAL_FLAG_DMA_TX))
        result = -1;
    if ((oflag & SERIAL_FLAG_INT_BYTE_RX) && !(serial->init_flag & SERIAL_FLAG_INT_BYTE_RX))
        result = -1;
	if ((oflag & SERIAL_FLAG_INT_IDLE_RX) && !(serial->init_flag & SERIAL_FLAG_INT_IDLE_RX))
        result = -1;
    if ((oflag & SERIAL_FLAG_INT_TX) && !(serial->init_flag & SERIAL_FLAG_INT_TX))
        result = -1;
    
    if(0 != result)
    {   
        SER_LOG("initialize device:%s failed. The error code is %d\n",
                           serial->name, result);
        return result;
    }
    serial->open_flag = oflag & 0xff;
    
	/*rx*/
	if (oflag & SERIAL_FLAG_INT_BYTE_RX)
	{   
		ringbuffer_init(&serial->rx_fifo_rb, serial->rx_buffer , serial->config.rxbufsz);
		
		serial->open_flag |= SERIAL_FLAG_INT_BYTE_RX;
		/* configure low level device */
		serial->ops->control(serial, DEVICE_CTRL_SET_INT, (void *)SERIAL_FLAG_INT_BYTE_RX);
	}
	else if(oflag & SERIAL_FLAG_INT_IDLE_RX)
	{
		ringbuffer_init(&serial->rx_fifo_rb, serial->rx_buffer , serial->config.rxbufsz);
		
		serial->open_flag |= SERIAL_FLAG_INT_IDLE_RX;
		/* configure low level device */
		serial->ops->control(serial, DEVICE_CTRL_SET_INT, (void *)SERIAL_FLAG_INT_IDLE_RX);
	}
	else if (oflag & SERIAL_FLAG_DMA_RX)
	{
       if (serial->config.rxbufsz == 0) 
       {
            serial->rx_dma_activated = 0;
            SER_LOG("serial config rxbufsz err 0 byte!");
       }
       else
       {
            ringbuffer_init(&serial->rx_fifo_rb, serial->rx_buffer , serial->config.rxbufsz);
            /* configure low level device */
            serial->ops->control(serial , DEVICE_CTRL_CONFIG, (void *)SERIAL_FLAG_DMA_RX);
       }
        serial->open_flag |= SERIAL_FLAG_DMA_RX;
    }
    else
    {
        result = -3;
        SER_LOG("serial open err -3:%s" , serial->name);
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
	  	//TODO:dataqueue
		//data_queue_init(&serial->tx_data_queue , serial->tx_buffer , serial->config.txbufsz);  
		serial->open_flag |= SERIAL_FLAG_DMA_TX;
		/* configure low level device */
		serial->ops->control(serial, DEVICE_CTRL_CONFIG, (void *)SERIAL_FLAG_DMA_TX);
	}
    else
    {
        result = -3;
        SER_LOG("serial open err -3:%s" , serial->name);
    }
    if(result == 0)
    {
        serial->ref_count++;
    }
    
    return result;
}

int serial_close(struct serial_device* serial)
{
    GSI_ASSERT(serial);
    
    if(serial->ref_count >1)
    {
	  	serial->ref_count--;
        return 0;
    }
    
    if(serial->open_flag & SERIAL_FLAG_INT_BYTE_RX)
    {
        serial->open_flag &= ~SERIAL_FLAG_INT_BYTE_RX;
        /* configure low level device */
        serial->ops->control(serial, DEVICE_CTRL_CLR_INT, (void*)SERIAL_FLAG_INT_BYTE_RX);
    }
	else if(serial->open_flag & SERIAL_FLAG_INT_IDLE_RX)
    {
        serial->open_flag &= ~SERIAL_FLAG_INT_IDLE_RX;
        /* configure low level device */
        serial->ops->control(serial, DEVICE_CTRL_CLR_INT, (void*)SERIAL_FLAG_INT_IDLE_RX);
    }
    else if (serial->open_flag & SERIAL_FLAG_DMA_RX)
    {
        /* configure low level device */
        serial->ops->control(serial, DEVICE_CTRL_CLR_INT, (void *) SERIAL_FLAG_DMA_RX);
        serial->open_flag &= ~SERIAL_FLAG_DMA_RX;
    }
    
    if (serial->open_flag & SERIAL_FLAG_INT_TX)
    {
        /* configure low level device */
        serial->ops->control(serial, DEVICE_CTRL_CLR_INT, (void*)SERIAL_FLAG_INT_TX);
        serial->open_flag &= ~SERIAL_FLAG_INT_TX;
    }
    else if (serial->open_flag & SERIAL_FLAG_DMA_TX)
    {
        serial->open_flag &= ~SERIAL_FLAG_DMA_TX;
    }
    return 0;
}

s_uint16_t serial_read(struct serial_device* serial,void *buffer, s_uint16_t size)
{
    if (size == 0) 
    {
        return 0;
    }
    if (serial->open_flag & SERIAL_FLAG_INT_BYTE_RX || 
		serial->open_flag & SERIAL_FLAG_INT_IDLE_RX)
    {
        return _serial_int_rx(serial, (s_uint8_t *)buffer, size);
    }

    else if (serial->open_flag & SERIAL_FLAG_DMA_RX)
    {
        return _serial_dma_rx(serial, (s_uint8_t *)buffer, size);
    }

    return _serial_poll_rx(serial, (s_uint8_t *)buffer, size); 
}

s_uint16_t serial_write(struct serial_device* serial,const void *buffer,s_uint16_t size)                       
{
    if (size == 0) 
    {
        return 0;
    }
    if (serial->open_flag & SERIAL_FLAG_INT_TX)
    {
        return _serial_int_tx(serial, (const s_uint8_t *)buffer, size);
    }
    else if (serial->open_flag & SERIAL_FLAG_DMA_TX)
    {
        return _serial_dma_tx(serial, (const s_uint8_t *)buffer, size);
    }
    else
    {
        return _serial_poll_tx(serial, (const s_uint8_t *)buffer, size);
    }
}
int serial_set_tx_complete(struct serial_device* serial, 
						   int (*tx_done)(struct serial_device* serial,void *buffer))
{
	GSI_ASSERT(serial);
	
	serial->tx_complete = tx_done;
	return 0;
}


int serial_set_rx_indicate(struct serial_device* serial, 
						   int (*rx_ind)(struct serial_device* serial,s_uint16_t size))
{
	GSI_ASSERT(serial);
	serial->rx_indicate = rx_ind;
	return 0;
}

int serial_control(struct serial_device* serial,
                      int              cmd,
                      void             *args)
{
    int ret = 0;
    GSI_ASSERT(serial != GSI_NULL);
   
    switch (cmd)
    {
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
           
				/* serial device has been opened, to configure it */
				serial->ops->configure(serial, (struct serial_configure *) args);
                
            }
            break;
        default :
            /* control device */
            ret = serial->ops->control(serial, cmd, args);
            break;
    }

    return ret;
}

/*
 * serial register
 */
int hw_serial_register(struct serial_device    *serial,
                        const char              *name,
                        s_uint16_t                flag,
                        void                   *data)						   
{
    int ret = 0;
	struct serial_device*  cur_serial = &_hw_serial;
    
    serial->name = name;
	serial->init_flag = flag;
	serial->tx_complete = NULL;
	serial->rx_indicate = NULL;
	
	/*insert serial to hw_serial*/
	while(cur_serial->next != NULL)
	{
		cur_serial = cur_serial->next;
	}
	cur_serial->next = serial;
	serial->next = NULL;
	  
    return ret;
}

/* ISR for serial interrupt */
void hw_serial_isr(struct serial_device *serial, int event)
{
  	s_uint8_t dma_rx_ind = 1;
    switch (event & 0xff)
    {
        case SERIAL_EVENT_RX_BYTE_IND:
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
            if (serial->rx_indicate != NULL && 
				0 == (serial->open_flag & SERIAL_FLAG_INT_IDLE_RX))
            {
                s_uint16_t rx_length;

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
		case SERIAL_EVENT_RX_IDLE_IND:
		{
			/* invoke callback */
            if (serial->rx_indicate != NULL)
            {
                s_uint16_t rx_length;

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
		    s_uint8_t ch;
 			if(ringbuffer_getchar(&serial->tx_fifo_rb , &ch))
			{
				serial->ops->putc(serial , ch);
			}
            else
            {
               serial->tx_int_activated = 0;
            }
            break;
        }
        case SERIAL_EVENT_TX_DMADONE:
        {
//            s_uint16_t data_size;
//            s_uint8_t last_data_ptr[8];
//			s_uint8_t data_ptr[8];
			//TODO:dataqueue
            //data_queue_pop(&serial->tx_data_queue , last_data_ptr, &data_size);
            //if (data_queue_peak(&serial->tx_data_queue, data_ptr, &data_size) == 0)
//			if(1)
//            {
//                /* transmit next data node */
//                serial->tx_dma_activated = 1;
//                serial->ops->dma_transmit(serial, data_ptr+2, data_size, SERIAL_DMA_TX);
//            }
//            else
//            {
//                serial->tx_dma_activated = 0;
//            }
//
//            /* invoke callback */
//            if (serial->tx_complete != NULL)
//            {
//                serial->tx_complete(serial, (void*)last_data_ptr);
//            }
            break;
        }
		case SERIAL_EVENT_RX_DMAHALF:
		{
		  	dma_rx_ind = 0;
			/*there is no break*/
		}
	    case SERIAL_EVENT_RX_DMADONE:
        {
            int length;

            /* get DMA rx length */
            length = (event & (~0xff)) >> 8;

            if (serial->config.rxbufsz == 0)
            {
				/* invoke callback */
                if (serial->rx_indicate != NULL)
                {
                    serial->rx_indicate(serial, length);
                }
                serial->rx_dma_activated = 0;
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
                if (serial->rx_indicate != NULL && dma_rx_ind)
                {
                    serial->rx_indicate(serial, length);
                }
            }
            break;
        }
    }
}

