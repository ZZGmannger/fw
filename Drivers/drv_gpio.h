#ifndef DRV_GPIO_H
#define DRV_GPIO_H

#include "stm32f1xx_hal.h"
#include "board.h"
#include "pin.h"

struct pin_index
{
    int index;
    GPIO_TypeDef *gpio;
    uint32_t pin;
};

struct pin_irq_map
{
    uint16_t pinbit;
    IRQn_Type irqno;
};

int hw_pin_init(void);

#endif