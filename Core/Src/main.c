
#include "main.h"

#include "board.h"
#include "pin.h"
#include "serial.h"

#define LED_PIN    (50)
#define KEY_PIN    (0)


#define TUART     "uart2"

struct serial_device*  uart1;

void key_press_hander(void *args)
{
    static uint8_t flag;
    
    flag = !flag;
    
    if(flag)
    {
		pin_write(LED_PIN, PIN_LOW);
    }
    else
    {
		pin_write(LED_PIN ,PIN_HIGH);
    }
}
 

uint8_t len;
int uart1_rx_ind(struct serial_device* serial , uint16_t size)
{
  	uint8_t buf[128];
	len = size;
  	serial_read(serial , buf, len);
    serial_write(serial , buf, size);
	return 0;
}

int main(void)
{
    HAL_Init();

    board_init();

    pin_mode(LED_PIN, PIN_MODE_OUTPUT);

    pin_mode(KEY_PIN , PIN_MODE_INPUT_PULLUP);
    pin_attach_irq(KEY_PIN , PIN_IRQ_MODE_FALLING , key_press_hander , NULL);
    pin_irq_enable(KEY_PIN ,PIN_IRQ_ENABLE);  
	
	uart1 = serial_find(TUART);
	if(uart1 != NULL)
	{
	  	struct serial_configure cfg = SERIAL_CONFIG_DEFAULT;
	    cfg.baud_rate =9600;
		serial_control(uart1  , DEVICE_CTRL_CONFIG , &cfg);
		serial_open(uart1 ,   SERIAL_FLAG_DMA_RX|SERIAL_FLAG_INT_TX);
	    serial_set_rx_indicate(uart1 , uart1_rx_ind);
	}
	
    while (1)
    {
        HAL_Delay(1000);
    }
}

