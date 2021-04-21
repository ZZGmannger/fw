
#include "dataqueue.h"


s_err_t data_queue_init(struct data_queue *queue , void* buffer , s_uint16_t size)
{
	ringbuffer_init(&queue->rb , (s_uint8_t*)buffer, size);
	return 0;
}
s_err_t data_queue_push(struct data_queue *queue , const void *data, s_uint16_t size)
{
  	s_uint16_t remain;
	s_uint16_t length;
	s_uint8_t ch;
	
	remain = queue->rb.buffer_size - ringbuffer_data_len(&queue->rb);
	
	while(size > remain)
	{
		ringbuffer_get(&queue->rb , (s_uint8_t *)(&length) , 2);
		while(length--)
		{
			ringbuffer_getchar(&queue->rb , &ch);
		}
		remain = queue->rb.buffer_size - ringbuffer_data_len(&queue->rb);
	}
	
	ringbuffer_put(&queue->rb, (s_uint8_t *)&size, 2);
	ringbuffer_put(&queue->rb, (s_uint8_t *)data, size);
	
	queue->num++;
	return 0;
}

s_err_t data_queue_pop(struct data_queue *queue , void *data_ptr , s_uint16_t *size)
{
  	if(queue->num)
	{
		ringbuffer_get(&queue->rb , (s_uint8_t *)(size) , 2);
		ringbuffer_get(&queue->rb , (s_uint8_t *)data_ptr , *size);
		queue->num--;
		return 0;
	}
	return -1;
}

s_err_t data_queue_peak(struct data_queue *queue, void *data_ptr , s_uint16_t *size)
{
    s_uint16_t length;
	if(queue->num)
	{
		ringbuffer_peak(&queue->rb , (s_uint8_t *)&length , 2);
		ringbuffer_peak(&queue->rb , (s_uint8_t *)data_ptr , length+2);
        *size = length;
		return 0;
	}
	return -1;
}

void data_queue_reset(struct data_queue *queue)
{
	
}
