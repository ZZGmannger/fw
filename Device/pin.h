
#ifndef PIN_H
#define PIN_H

#include "gsi_def.h"

#define PIN_LOW                 0x00
#define PIN_HIGH                0x01

#define PIN_MODE_OUTPUT         0x00
#define PIN_MODE_INPUT          0x01
#define PIN_MODE_INPUT_PULLUP   0x02
#define PIN_MODE_INPUT_PULLDOWN 0x03
#define PIN_MODE_OUTPUT_OD      0x04

#define PIN_IRQ_MODE_RISING             0x00
#define PIN_IRQ_MODE_FALLING            0x01
#define PIN_IRQ_MODE_RISING_FALLING     0x02
#define PIN_IRQ_MODE_HIGH_LEVEL         0x03
#define PIN_IRQ_MODE_LOW_LEVEL          0x04

#define PIN_IRQ_DISABLE                 0x00
#define PIN_IRQ_ENABLE                  0x01

struct device_pin
{	
  	const char* name;
	const struct pin_ops* ops;
};

struct pin_ops
{
	void (*pin_mode)(s_uint8_t pin, s_uint8_t mode);
    void (*pin_write)(s_uint8_t pin, s_uint8_t value);
    s_err_t (*pin_read)(s_uint8_t pin);

    s_err_t (*pin_attach_irq)(s_uint8_t pin ,s_uint8_t mode, void (*hdr)(void *args), void *args);              
    s_err_t (*pin_detach_irq)(s_uint8_t pin);
    s_err_t (*pin_irq_enable)(s_uint8_t pin, s_uint8_t enabled);
};
 
struct pin_irq_hdr
{
    s_int16_t        pin;
    s_uint16_t       mode;
    void (*hdr)(void *args);
    void             *args;
};

s_err_t pin_read(s_uint8_t pin);
void pin_write(s_uint8_t pin, s_uint8_t value);
void pin_mode(s_uint8_t pin, s_uint8_t mode);
s_err_t pin_irq_enable(s_uint8_t pin, s_uint8_t enabled);
s_err_t pin_attach_irq(s_uint8_t pin, s_uint8_t mode,void (*hdr)(void *args), void  *args);
s_err_t pin_detach_irq(s_uint8_t pin);

void device_pin_register(const char* name ,const struct pin_ops* ops);

#endif