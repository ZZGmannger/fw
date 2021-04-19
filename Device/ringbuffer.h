 
#ifndef RINGBUFFER_H__
#define RINGBUFFER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

/* ring buffer */
struct ringbuffer
{
    uint16_t read_mirror : 1;
    uint16_t read_index : 15;
    uint16_t write_mirror : 1;
    uint16_t write_index : 15;
    /* as we use msb of index as mirror bit, the size should be signed and
     * could only be positive. */
    int16_t buffer_size;
    uint8_t *buffer_ptr;
};

enum ringbuffer_state
{
    RINGBUFFER_EMPTY,
   	RINGBUFFER_FULL,
    /* half full is neither full nor empty */
    RINGBUFFER_HALFFULL,
};

/**
 * RingBuffer for DeviceDriver
 *
 * Please note that the ring buffer implementation of RT-Thread
 * has no thread wait or resume feature.
 */
void ringbuffer_init(struct ringbuffer *rb, uint8_t *pool, int16_t size);
void ringbuffer_reset(struct ringbuffer *rb);
uint16_t ringbuffer_put(struct ringbuffer *rb, const uint8_t *ptr, uint16_t length);
uint16_t ringbuffer_put_force(struct ringbuffer *rb, const uint8_t *ptr , uint16_t length);
uint16_t ringbuffer_putchar(struct ringbuffer *rb, const uint8_t ch);
uint16_t ringbuffer_putchar_force(struct ringbuffer *rb, const uint8_t ch);
uint16_t ringbuffer_get(struct ringbuffer *rb , uint8_t *ptr , uint16_t length);
uint16_t ringbuffer_getchar(struct ringbuffer *rb , uint8_t *ch);
uint16_t ringbuffer_data_len(struct ringbuffer *rb);

uint16_t ringbuffer_updata_put_index_force(struct ringbuffer *rb,uint16_t length);
uint16_t ringbuffer_updata_put_index(struct ringbuffer *rb , uint16_t length);
/** return the size of empty space in rb */
#define ringbuffer_space_len(rb) ((rb)->buffer_size - ringbuffer_data_len(rb))


#ifdef __cplusplus
}
#endif

#endif
