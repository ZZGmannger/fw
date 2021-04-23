
#include "main.h"
#define LOG_TAG    "UTtest"
#include <elog.h>

#include "board.h"
#include "pin.h"
#include "serial.h"

#include "export.h"

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
 
 void test_elog(void) ;

int main(void)
{
    HAL_Init();

    hw_board_init();

    pin_mode(LED_PIN, PIN_MODE_OUTPUT);

    pin_mode(KEY_PIN , PIN_MODE_INPUT_PULLUP);
    pin_attach_irq(KEY_PIN , PIN_IRQ_MODE_FALLING , key_press_hander , NULL);
    pin_irq_enable(KEY_PIN ,PIN_IRQ_ENABLE);  
	
    while (1)
    {
	   	test_auto_run();
    }
}
/**
 * EasyLogger demo
 */
int test_elog_init(void)
{
	/* initialize EasyLogger */
    elog_init();
	
	elog_set_text_color_enabled(true);

    /* set EasyLogger log format */
    elog_set_fmt(ELOG_LVL_ERROR,  ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME|ELOG_FMT_FUNC|ELOG_FMT_LINE);
    elog_set_fmt(ELOG_LVL_WARN,   ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME|ELOG_FMT_FUNC|ELOG_FMT_LINE);
    elog_set_fmt(ELOG_LVL_INFO,   ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_DEBUG,  ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_VERBOSE,ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    /* start EasyLogger */
   // elog_set_filter_tag_lvl(LOG_TAG, ELOG_LVL_WARN);
    
    elog_start();
	return 0;
}
INIT_COMPONENT_EXPORT(test_elog_init);

void test_elog(void) {
  void elog_port_output(const char *log, size_t size);
//
//  	hw_interrupt_disable();
//    elog_port_output("EasyLog1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111\r\n" , 103);
//    hw_interrupt_enable();
//	
//	hw_interrupt_disable();
//	elog_port_output("EasyLog2222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222\r\n" , 103);
//	hw_interrupt_enable();
//	
//	hw_interrupt_disable();
//	elog_port_output("EasyLog3333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333\r\n" , 103);
//	hw_interrupt_enable();
//    elog_port_output("Hello EasyLog4\r\n" , 16);
//    elog_port_output("Hello EasyLog5\r\n" , 16);
//    elog_port_output("Hello EasyLog6\r\n" , 16);
//	
//    elog_port_output("Hello EasyLog7\r\n" , 16);
//    elog_port_output("Hello EasyLog8\r\n" , 16);
//    elog_port_output("Hello EasyLog9\r\n" , 16);
//    LOG_E("Hello EasyLog1");
//    LOG_E("Hello EasyLog2");
//    LOG_E("Hello EasyLog3");
 	LOG_E("EasyLog1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111");
 	LOG_W("EasyLog2222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222");
 	LOG_I("EasyLog3333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333");
 	LOG_D("EasyLog4444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444");
 	LOG_V("EasyLog5555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555");
	HAL_Delay(3000);
}
//RUN_TEST_EXPORT(test_elog);
