
#ifndef DATAQUEUE_H
#define DATAQUEUE_H

#include  "gsi_def.h"

/* data queue implementation */
#define DATAQUEUE_EVENT_UNKNOWN   0x00
#define DATAQUEUE_EVENT_POP       0x01
#define DATAQUEUE_EVENT_PUSH      0x02
#define DATAQUEUE_EVENT_LWM       0x03

struct data_item;
#define DATAQUEUE_SIZE(dq)        ((dq)->put_index - (dq)->get_index)
#define DATAQUEUE_EMPTY(dq)       ((dq)->size - DATAQUEUE_SIZE(dq))
/* data queue implementation */
struct data_queue
{
    s_uint16_t size;

    s_uint16_t get_index;
    s_uint16_t put_index;

    struct data_item *queue;

    /* event notify */
    void (*evt_notify)(struct data_queue *queue, s_uint32_t event);
};

/**
 * DataQueue for DeviceDriver
 */
s_err_t data_queue_init(struct data_queue *queue,
                            s_uint16_t           size,
                            void (*evt_notify)(struct data_queue *queue, s_uint32_t event));
s_err_t data_queue_push(struct data_queue *queue,
                            const void           *data_ptr,
                            s_size_t             data_size);
                            
s_err_t data_queue_pop(struct data_queue *queue,
                           const void          **data_ptr,
                           s_size_t            *size);
                           
s_err_t data_queue_peak(struct data_queue *queue,
                            const void          **data_ptr,
                            s_size_t            *size);
 

#endif