
#ifndef DATAQUEUE_H
#define DATAQUEUE_H

#include  "ringbuffer.h"

/* data queue implementation */
struct data_queue
{
	struct ringbuffer rb;
	s_uint32_t num;
};

/**
 * DataQueue for DeviceDriver
 */
s_err_t data_queue_init(struct data_queue *queue , void* buffer     ,s_uint16_t size);          
s_err_t data_queue_push(struct data_queue *queue , const void *data ,s_uint16_t size);
s_err_t data_queue_pop(struct data_queue *queue  , void *data_ptr   ,s_uint16_t *size);                   
s_err_t data_queue_peak(struct data_queue *queue , void *data_ptr   ,s_uint16_t *size);
void data_queue_reset(struct data_queue *queue);





#endif