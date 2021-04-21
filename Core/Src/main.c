
#include "main.h"

#include "board.h"
#include "pin.h"
#include "serial.h"

#define LED_PIN    (50)
#define KEY_PIN    (0)

#define LOG_TAG    "UTtest"
#include <elog.h>


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
 
 void test_elog(void) ;

int main(void)
{
    HAL_Init();

    hw_board_init();

    pin_mode(LED_PIN, PIN_MODE_OUTPUT);

    pin_mode(KEY_PIN , PIN_MODE_INPUT_PULLUP);
    pin_attach_irq(KEY_PIN , PIN_IRQ_MODE_FALLING , key_press_hander , NULL);
    pin_irq_enable(KEY_PIN ,PIN_IRQ_ENABLE);  

	/* initialize EasyLogger */
    elog_init();
	
	elog_set_text_color_enabled(true);

    /* set EasyLogger log format */
    elog_set_fmt(ELOG_LVL_ERROR,  ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME|ELOG_FMT_FUNC|ELOG_FMT_LINE);
    elog_set_fmt(ELOG_LVL_WARN,   ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME|ELOG_FMT_FUNC|ELOG_FMT_LINE);
    elog_set_fmt(ELOG_LVL_INFO,   ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME|ELOG_FMT_FUNC|ELOG_FMT_LINE);
    elog_set_fmt(ELOG_LVL_DEBUG,  ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME|ELOG_FMT_FUNC|ELOG_FMT_LINE);
    elog_set_fmt(ELOG_LVL_VERBOSE,ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME|ELOG_FMT_FUNC|ELOG_FMT_LINE);
    /* start EasyLogger */
    elog_start();
	
    while (1)
    {
	   	test_elog();
        HAL_Delay(3000);
    }
}
/**
 * EasyLogger demo
 */
char nn[]={"Hello EasyLog============++++++++++++++++//////////////////QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQGGGGGGGGGGGGGG!\r\n"};
void elog_port_output(const char *log, size_t size);
void test_elog(void) {
    /* test log output for all level */
	elog_port_output(nn ,sizeof(nn) ) ;
	elog_port_output(nn ,sizeof(nn) ) ;
	elog_port_output(nn ,sizeof(nn) ) ;
//  LOG_E("Hello EasyLog============++++++++++++++++//////////////////QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQGGGGGGGGGGGGGG!");
// 	LOG_W("Hello EasyLog============++++++++++++++++//////////////////QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQGGGGGGGGGGGGGG!");
// 	LOG_I("Hello EasyLog============++++++++++++++++//////////////////QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQGGGGGGGGGGGGGG!");
// 	LOG_D("Hello EasyLog============++++++++++++++++//////////////////QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQGGGGGGGGGGGGGG!");
// 	LOG_V("Hello EasyLog============++++++++++++++++//////////////////QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQGGGGGGGGGGGGGG!");

}
