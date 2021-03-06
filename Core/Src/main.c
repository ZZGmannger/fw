
#include "main.h"
#define LOG_TAG    "UTtest"
#include <elog.h>

#include "board.h"
#include "pin.h"
#include "serial.h"

#include "export.h"
#include "shell.h"

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
    elog_set_fmt(ELOG_LVL_WARN,   ELOG_FMT_LVL);
    elog_set_fmt(ELOG_LVL_INFO,   ELOG_FMT_LVL);
    elog_set_fmt(ELOG_LVL_DEBUG,  ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_VERBOSE,ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    /* start EasyLogger */
   // elog_set_filter_tag_lvl(LOG_TAG, ELOG_LVL_WARN);
    
    elog_start();
	return 0;
}
INIT_COMPONENT_EXPORT(test_elog_init);

void test_elog(void) {

 	LOG_E("EasyLog111111111111111111111111111111111111111111111111111111111111111111111111");
 	LOG_W("EasyLog22222222222222222222222222222222222222222222222222222222222222222222222");
 	LOG_I("EasyLog3333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333");
 	LOG_D("EasyLog4444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444");
 	LOG_V("EasyLog5555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555");
}
SHELL_EXPORT_CMD(SHELL_CMD_DISABLE_RETURN ,test_log, test_elog, test log);

