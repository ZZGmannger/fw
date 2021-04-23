
#include "dataqueue.h"


struct data_item
{
    const void *data_ptr;
    s_size_t data_size;
};


s_err_t data_queue_init(struct data_queue *queue,
                          s_uint16_t size,
                          void (*evt_notify)(struct data_queue *queue, s_uint32_t event))
{
    GSI_ASSERT(queue != GSI_NULL);

    queue->evt_notify = evt_notify;

    queue->size = size;

    queue->get_index = 0;
    queue->put_index = 0;

    queue->queue = (struct data_item *)malloc(sizeof(struct data_item) * size);
    if (queue->queue == GSI_NULL)
    {
        return -1;
    }

    return 0;
}

s_err_t data_queue_push(struct data_queue *queue,
                            const void *data_ptr,
                            s_size_t data_size )
                           
{
    s_err_t    result;
    
    GSI_ASSERT(queue != GSI_NULL);

    result = 0;

    hw_interrupt_disable();
    while (queue->put_index - queue->get_index == queue->size)
    {
       
    }

    queue->queue[queue->put_index % queue->size].data_ptr  = data_ptr;
    queue->queue[queue->put_index % queue->size].data_size = data_size;
    queue->put_index += 1;
    
    hw_interrupt_enable();
    if (queue->evt_notify != GSI_NULL)
    {
        queue->evt_notify(queue, DATAQUEUE_EVENT_PUSH);
    }

    return result;
}

s_err_t data_queue_pop(struct data_queue *queue,
                           const void** data_ptr,
                           s_size_t *size)
                          
{
    s_err_t    result;

    GSI_ASSERT(queue != GSI_NULL);
    GSI_ASSERT(data_ptr != GSI_NULL);
    GSI_ASSERT(size != GSI_NULL);

    result = 0;
    hw_interrupt_disable();
    while (queue->get_index == queue->put_index)
    {
        /* queue is empty */
    }

    *data_ptr = queue->queue[queue->get_index % queue->size].data_ptr;
    *size     = queue->queue[queue->get_index % queue->size].data_size;

    queue->get_index += 1;

    hw_interrupt_enable();
    if (queue->evt_notify != GSI_NULL)
    {
        queue->evt_notify(queue, DATAQUEUE_EVENT_POP);
    }

    return result;
}

s_err_t data_queue_peak(struct data_queue *queue,
                            const void** data_ptr,
                            s_size_t *size)
{
    GSI_ASSERT(queue != GSI_NULL);

    hw_interrupt_disable();

    if (queue->get_index == queue->put_index) 
    {
        hw_interrupt_enable();
        
        return -1;
    }

    *data_ptr = queue->queue[queue->get_index % queue->size].data_ptr;
    *size     = queue->queue[queue->get_index % queue->size].data_size;

    hw_interrupt_enable();

    return 0;
}


