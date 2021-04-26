#include "shell.h"
#include "shell_port.h"
#include  "export.h"
#include "serial.h"


#define USE_SHELL

#define SHELL_PORT   "uart1"

static struct serial_device*  shell_uart;
static Shell shell;
static char shell_buffer[512];

void shell_put(const char* buf , s_uint16_t len)
{
   serial_write(shell_uart , buf ,len);
}

static void shell_write(const char ch)
{
 	serial_write(shell_uart , &ch ,1);
}

static int shell_received_byte(struct serial_device* serial , s_uint16_t size)
{
  	s_uint8_t ch;
	while(size--)
	{
		serial_read(serial , &ch , 1);
		shellHandler(&shell, ch);
	}
	return 0;
}

int shell_init(void)
{
  	shell_uart = serial_find(SHELL_PORT);
	if(shell_uart != NULL)
	{
	  	struct serial_configure cfg = SERIAL_CONFIG_DEFAULT;
	    cfg.baud_rate = 115200;
		serial_control(shell_uart  , DEVICE_CTRL_CONFIG , &cfg);
		serial_open(shell_uart ,    SERIAL_FLAG_INT_BYTE_RX|SERIAL_FLAG_INT_TX);
		serial_set_rx_indicate(shell_uart , shell_received_byte );
	}
    
	shell.write = shell_write;
	shellInit(&shell , shell_buffer , sizeof(shell_buffer));
	return 0;
}
#ifdef USE_SHELL
INIT_COMPONENT_EXPORT(shell_init);
#endif

#define LOG_TAG    "Shell"
#include <elog.h>   

int hello()
{
  LOG_E("Hello World\r\n");
 return 0;
}

SHELL_EXPORT_CMD(
SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC)|SHELL_CMD_DISABLE_RETURN,
hello, hello, print hello);