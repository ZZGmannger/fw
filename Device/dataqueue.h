
#ifndef DATAQUEUE_H
#define DATAQUEUE_H


#include "main.h"
#include  "ringbuffer.h"

/* data queue implementation */
struct data_queue
{
	struct ringbuffer rb;
	uint32_t num;
};

/**
 * DataQueue for DeviceDriver
 */
int data_queue_init(struct data_queue *queue , void* buffer , uint16_t size);          
int data_queue_push(struct data_queue *queue , const void *data, uint16_t size);
int data_queue_pop(struct data_queue *queue ,  void *data_ptr , uint16_t *size);                   
int data_queue_peak(struct data_queue *queue,  void *data_ptr , uint16_t *size);
void data_queue_reset(struct data_queue *queue);





#endif