
#ifndef DATAQUEUE_H
#define DATAQUEUE_H


#include "main.h"


 
#define RT_DATAQUEUE_SIZE(dq)        ((dq)->put_index - (dq)->get_index)
#define RT_DATAQUEUE_EMPTY(dq)       ((dq)->size - RT_DATAQUEUE_SIZE(dq))
/* data queue implementation */
struct data_queue
{
    uint16_t size;

    uint16_t get_index;
    uint16_t put_index;
};

/**
 * DataQueue for DeviceDriver
 */
int data_queue_init(struct data_queue *queue , void* buffer , uint16_t size);          
int data_queue_push(struct data_queue *queue , const void *data, uint16_t size);
int data_queue_pop(struct data_queue *queue , const void **data_ptr , uint16_t *size);                   
int data_queue_peak(struct data_queue *queue,const void **data_ptr , uint16_t *size);
void data_queue_reset(struct data_queue *queue);





#endif