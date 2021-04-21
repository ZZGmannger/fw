
#ifndef DRV_BOARD_H
#define DRV_BOARD_H

#include "stm32f1xx_hal.h"





void hw_board_init(void);

void hw_interrupt_enable(void);
void hw_interrupt_disable(void);
void hw_interrupt_enter(void);
void hw_interrupt_leave(void);


#endif