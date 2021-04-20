
#ifndef DRV_BOARD_H
#define DRV_BOARD_H

#include "stm32f1xx_hal.h"


#define rt_container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))



void board_init(void);

void hw_interrupt_enable(void);
void hw_interrupt_disable(void);
void interrupt_enter(void);
void interrupt_leave(void);


#endif