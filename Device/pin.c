
#include "pin.h"

#define PIN_ASSERT(...)

static struct device_pin  _hw_pin;


void device_pin_register(const char* name ,const struct pin_ops* ops)
{
  	_hw_pin.name = name;
  	_hw_pin.ops = ops;
}

int pin_attach_irq(uint8_t pin, uint8_t mode,
                             void (*hdr)(void *args), void  *args)
{
    PIN_ASSERT(_hw_pin.ops != RT_NULL);
    if(_hw_pin.ops->pin_attach_irq)
    {
        return _hw_pin.ops->pin_attach_irq(pin, mode, hdr, args);
    }
    return -1;
}

int pin_detach_irq(uint8_t pin)
{
    PIN_ASSERT(_hw_pin.ops != RT_NULL);
    if(_hw_pin.ops->pin_detach_irq)
    {
        return _hw_pin.ops->pin_detach_irq(pin);
    }
    return -1;
}

int pin_irq_enable(uint8_t pin, uint8_t enabled)
{
    PIN_ASSERT(_hw_pin.ops != RT_NULL);
    if(_hw_pin.ops->pin_irq_enable)
    {
        return _hw_pin.ops->pin_irq_enable(pin, enabled);
    }
    return -1;
}

void pin_mode(uint8_t pin, uint8_t mode)
{
    PIN_ASSERT(_hw_pin.ops != RT_NULL);
    _hw_pin.ops->pin_mode(pin, mode);
}

void pin_write(uint8_t pin, uint8_t value)
{
    PIN_ASSERT(_hw_pin.ops != RT_NULL);
    _hw_pin.ops->pin_write(pin, value);
}

int pin_read(uint8_t pin)
{
    PIN_ASSERT(_hw_pin.ops != RT_NULL);
    return _hw_pin.ops->pin_read(pin);
}

