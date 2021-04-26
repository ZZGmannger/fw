
#include "pin.h"

 
static struct device_pin  _hw_pin;


void device_pin_register(const char* name ,const struct pin_ops* ops)
{
  	_hw_pin.name = name;
  	_hw_pin.ops = ops;
}

s_err_t pin_attach_irq(s_uint8_t pin , s_uint8_t mode,
                             void (*hdr)(void *args), void  *args)
{
    GSI_ASSERT(_hw_pin.ops != GSI_NULL);
    if(_hw_pin.ops->pin_attach_irq)
    {
        return _hw_pin.ops->pin_attach_irq(pin, mode, hdr, args);
    }
    return GSI_ERR_NULL;
}

s_err_t pin_detach_irq(s_uint8_t pin)
{
    GSI_ASSERT(_hw_pin.ops != GSI_NULL);
    if(_hw_pin.ops->pin_detach_irq)
    {
        return _hw_pin.ops->pin_detach_irq(pin);
    }
    return GSI_ERR_NULL;
}

s_err_t pin_irq_enable(s_uint8_t pin, s_uint8_t enabled)
{
    GSI_ASSERT(_hw_pin.ops != GSI_NULL);
    if(_hw_pin.ops->pin_irq_enable)
    {
        return _hw_pin.ops->pin_irq_enable(pin, enabled);
    }
    return GSI_ERR_NULL;
}

void pin_mode(s_uint8_t pin, s_uint8_t mode)
{
    GSI_ASSERT(_hw_pin.ops != GSI_NULL);
    _hw_pin.ops->pin_mode(pin, mode);
}

void pin_write(s_uint8_t pin, s_uint8_t value)
{
    GSI_ASSERT(_hw_pin.ops != GSI_NULL);
    _hw_pin.ops->pin_write(pin, value);
}

s_err_t pin_read(s_uint8_t pin)
{
    GSI_ASSERT(_hw_pin.ops != GSI_NULL);
    return _hw_pin.ops->pin_read(pin);
}

