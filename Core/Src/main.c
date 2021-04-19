
#include "main.h"

#include "board.h"
#include "pin.h"

#define LED_PIN    (50)
#define KEY_PIN    (0)


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
 
int main(void)
{
    HAL_Init();

    board_init();

    pin_mode(LED_PIN, PIN_MODE_OUTPUT);

    pin_mode(KEY_PIN , PIN_MODE_INPUT_PULLUP);
    pin_attach_irq(KEY_PIN , PIN_IRQ_MODE_FALLING , key_press_hander , NULL);
    pin_irq_enable(KEY_PIN ,PIN_IRQ_ENABLE);                     

    while (1)
    {
        HAL_Delay(1000);
        HAL_Delay(1000);
    }
}

