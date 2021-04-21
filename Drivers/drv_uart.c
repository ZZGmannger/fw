
#include "drv_uart.h"

#define DRV_ASSERT(...)

/*uart1 config*/
#define UART1_CONFIG                                                \
    {                                                               \
        .name = "uart1",                                            \
        .Instance = USART1,                                         \
        .irq_type = USART1_IRQn,                                    \
    }


#define UART1_DMA_RX_CONFIG                                         \
    {                                                               \
        .Instance = UART1_RX_DMA_INSTANCE,                          \
        .dma_rcc  = UART1_RX_DMA_RCC,                               \
        .dma_irq  = UART1_RX_DMA_IRQ,                               \
    }

#define UART1_DMA_TX_CONFIG                                         \
    {                                                               \
        .Instance = UART1_TX_DMA_INSTANCE,                          \
        .dma_rcc  = UART1_TX_DMA_RCC,                               \
        .dma_irq  = UART1_TX_DMA_IRQ,                               \
    }

/*uart2 config*/
#define UART2_CONFIG                                                \
    {                                                               \
        .name = "uart2",                                            \
        .Instance = USART2,                                         \
        .irq_type = USART2_IRQn,                                    \
    }

#define UART2_DMA_RX_CONFIG                                         \
    {                                                               \
        .Instance = UART2_RX_DMA_INSTANCE,                          \
        .dma_rcc  = UART2_RX_DMA_RCC,                               \
        .dma_irq  = UART2_RX_DMA_IRQ,                               \
    }

#define UART2_DMA_TX_CONFIG                                         \
    {                                                               \
        .Instance = UART2_TX_DMA_INSTANCE,                          \
        .dma_rcc  = UART2_TX_DMA_RCC,                               \
        .dma_irq  = UART2_TX_DMA_IRQ,                               \
    }




static void stm32_dma_config(struct serial_device *serial, uint32_t flag);
enum
{
    UART1_INDEX,
    UART2_INDEX,
};

static struct stm32_uart_config uart_config[] =
{
    UART1_CONFIG,
    UART2_CONFIG,
};

static struct stm32_uart uart_obj[sizeof(uart_config) / sizeof(uart_config[0])] = {0};


static int stm32_configure(struct serial_device *serial, struct serial_configure *cfg)
{
    struct stm32_uart *uart;
    DRV_ASSERT(serial != NULL);
    DRV_ASSERT(cfg != NULL);

    uart = rt_container_of(serial, struct stm32_uart, serial);

    uart->handle.Instance          = uart->config->Instance;
    uart->handle.Init.BaudRate     = cfg->baud_rate;
    uart->handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    uart->handle.Init.Mode         = UART_MODE_TX_RX;
    uart->handle.Init.OverSampling = UART_OVERSAMPLING_16;
    switch (cfg->data_bits)
    {
    case DATA_BITS_8:
        uart->handle.Init.WordLength = UART_WORDLENGTH_8B;
        break;
    case DATA_BITS_9:
        uart->handle.Init.WordLength = UART_WORDLENGTH_9B;
        break;
    default:
        uart->handle.Init.WordLength = UART_WORDLENGTH_8B;
        break;
    }
    switch (cfg->stop_bits)
    {
    case STOP_BITS_1:
        uart->handle.Init.StopBits   = UART_STOPBITS_1;
        break;
    case STOP_BITS_2:
        uart->handle.Init.StopBits   = UART_STOPBITS_2;
        break;
    default:
        uart->handle.Init.StopBits   = UART_STOPBITS_1;
        break;
    }
    switch (cfg->parity)
    {
    case PARITY_NONE:
        uart->handle.Init.Parity     = UART_PARITY_NONE;
        break;
    case PARITY_ODD:
        uart->handle.Init.Parity     = UART_PARITY_ODD;
        break;
    case PARITY_EVEN:
        uart->handle.Init.Parity     = UART_PARITY_EVEN;
        break;
    default:
        uart->handle.Init.Parity     = UART_PARITY_NONE;
        break;
    }

    if (HAL_UART_Init(&uart->handle) != HAL_OK)
    {
        return -1;
    }

    return 0;
}


static int stm32_control(struct serial_device *serial, int cmd, void *arg)
{
    struct stm32_uart *uart;

    uint32_t ctrl_arg = (uint32_t)arg;

    DRV_ASSERT(serial != NULL);
    uart = rt_container_of(serial, struct stm32_uart, serial);

    switch (cmd)
    {
    /* disable interrupt */
    case DEVICE_CTRL_CLR_INT:
		/* disable rx irq */
		NVIC_DisableIRQ(uart->config->irq_type);
	  	if(ctrl_arg == SERIAL_FLAG_INT_BYTE_RX)
		{
			__HAL_UART_DISABLE_IT(&(uart->handle), UART_IT_RXNE);
		}
		else if(ctrl_arg == SERIAL_FLAG_INT_IDLE_RX)
		{
			__HAL_UART_DISABLE_IT(&(uart->handle), UART_IT_IDLE);
			__HAL_UART_DISABLE_IT(&(uart->handle), UART_IT_RXNE);
		}
		else if (ctrl_arg == SERIAL_FLAG_INT_TX)
		{
			__HAL_UART_DISABLE_IT(&(uart->handle), UART_IT_TC);
		}
        break;
    /* enable interrupt */
    case DEVICE_CTRL_SET_INT:
		/* enable rx irq */
        NVIC_EnableIRQ(uart->config->irq_type);
		if(ctrl_arg == SERIAL_FLAG_INT_BYTE_RX)
		{
			__HAL_UART_ENABLE_IT(&(uart->handle), UART_IT_RXNE);
		}
		else if(ctrl_arg == SERIAL_FLAG_INT_IDLE_RX)
		{
			CLEAR_BIT(uart->handle.Instance->CR3, USART_CR3_EIE);
			__HAL_UART_ENABLE_IT(&(uart->handle), UART_IT_IDLE);
			__HAL_UART_ENABLE_IT(&(uart->handle), UART_IT_RXNE);
		}
		else if (ctrl_arg == SERIAL_FLAG_INT_TX)
		{
			__HAL_UART_ENABLE_IT(&(uart->handle), UART_IT_TC);
		}
        break;

    case DEVICE_CTRL_CONFIG:
        stm32_dma_config(serial, ctrl_arg);
        break;
    }
    return 0;
}


static int stm32_putc(struct serial_device *serial, char c)
{
    struct stm32_uart *uart;
    DRV_ASSERT(serial != NULL);

    uart = rt_container_of(serial, struct stm32_uart, serial);
	
   __HAL_UART_CLEAR_FLAG(&(uart->handle), UART_FLAG_TC);
   
    uart->handle.Instance->DR = c;
	
	if((serial->open_flag & SERIAL_FLAG_INT_TX) == 0)
	{
		while (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_TC) == RESET);
	}
	 
    return 1;
}

static int stm32_getc(struct serial_device *serial)
{
    int ch;
    struct stm32_uart *uart;
    DRV_ASSERT(serial != NULL);
    uart = rt_container_of(serial, struct stm32_uart, serial);

    ch = -1;
    if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_RXNE) != RESET)
    {
        ch = uart->handle.Instance->DR & 0xff;
    }
    return ch;
}


static uint32_t stm32_dma_transmit(struct serial_device *serial, uint8_t *buf, uint32_t size, int direction)
{
    struct stm32_uart *uart;
    DRV_ASSERT(serial != RT_NULL);
    uart = rt_container_of(serial, struct stm32_uart, serial);
    
    if (size == 0)
    {
        return 0;
    }
    
    if (SERIAL_DMA_TX == direction)
    {
        if (HAL_UART_Transmit_DMA(&uart->handle, buf, size) == HAL_OK)
        {
            return size;
        }
        else
        {
            return 0;
        }
    }
    return 0;
}

static const struct uart_ops stm32_uart_ops =
{
    .configure = stm32_configure,
    .control = stm32_control,
    .putc = stm32_putc,
    .getc = stm32_getc,
    .dma_transmit = stm32_dma_transmit
};


/**
 * Uart common interrupt process. This need add to uart ISR.
 *
 * @param serial serial device
 */
static void uart_isr(struct serial_device *serial)
{
    struct stm32_uart *uart;

    uint16_t recv_total_index, recv_len;

    DRV_ASSERT(serial != RT_NULL);
    uart = rt_container_of(serial, struct stm32_uart, serial);

    /* UART in mode Receiver -------------------------------------------------*/
    if ((__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_RXNE) != RESET) &&
            (__HAL_UART_GET_IT_SOURCE(&(uart->handle), UART_IT_RXNE) != RESET))
    {
        hw_serial_isr(serial, SERIAL_EVENT_RX_BYTE_IND);
    }
	else if (__HAL_UART_GET_IT_SOURCE(&(uart->handle), UART_IT_IDLE) != RESET &&
			 __HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_IDLE) != RESET)
	{
		 if ((serial->open_flag & SERIAL_FLAG_INT_IDLE_RX) != 0)
		 {
		 	hw_serial_isr(serial, SERIAL_EVENT_RX_IDLE_IND);
		 }
		 else if((serial->open_flag & SERIAL_FLAG_DMA_RX) != 0)
		 {
			hw_interrupt_disable();
			recv_total_index = serial->config.rxbufsz - __HAL_DMA_GET_COUNTER(&(uart->dma_rx.handle));
			recv_len = recv_total_index - uart->dma_rx.last_index;
			uart->dma_rx.last_index = recv_total_index;
			hw_interrupt_enable();

			if (recv_len)
			{
				hw_serial_isr(serial, SERIAL_EVENT_RX_DMADONE | (recv_len << 8));
			}
		 }
		 __HAL_UART_CLEAR_IDLEFLAG(&uart->handle);
	}
    else if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_TC) != RESET)
    {
        if ((serial->open_flag & SERIAL_FLAG_DMA_TX) != 0)
        {
            HAL_UART_IRQHandler(&(uart->handle));
        }
        else if((serial->open_flag & SERIAL_FLAG_INT_TX) != 0)
        {
			hw_serial_isr(serial, SERIAL_EVENT_TX_DONE);
            __HAL_UART_CLEAR_FLAG(&(uart->handle), UART_FLAG_TC);
        }
    }
    else
    {
        if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_ORE) != RESET)
        {
            __HAL_UART_CLEAR_OREFLAG(&uart->handle);
        }
        if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_NE) != RESET)
        {
            __HAL_UART_CLEAR_NEFLAG(&uart->handle);
        }
        if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_FE) != RESET)
        {
            __HAL_UART_CLEAR_FEFLAG(&uart->handle);
        }
        if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_PE) != RESET)
        {
            __HAL_UART_CLEAR_PEFLAG(&uart->handle);
        }
        if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_LBD) != RESET)
        {
            __HAL_UART_CLEAR_FLAG(&(uart->handle), UART_FLAG_LBD);
        }
        if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_CTS) != RESET)
        {
            __HAL_UART_CLEAR_FLAG(&(uart->handle), UART_FLAG_CTS);
        }
        if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_TXE) != RESET)
        {
            __HAL_UART_CLEAR_FLAG(&(uart->handle), UART_FLAG_TXE);
        }
        if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_TC) != RESET)
        {
            __HAL_UART_CLEAR_FLAG(&(uart->handle), UART_FLAG_TC);
        }
        if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_RXNE) != RESET)
        {
            __HAL_UART_CLEAR_FLAG(&(uart->handle), UART_FLAG_RXNE);
        }
    }
}


static void dma_isr(struct serial_device *serial)
{
    struct stm32_uart *uart;
    uint16_t recv_total_index, recv_len;
 
    DRV_ASSERT(serial != RT_NULL);
    uart = rt_container_of(serial, struct stm32_uart, serial);

    if ((__HAL_DMA_GET_IT_SOURCE(&(uart->dma_rx.handle), DMA_IT_TC) != RESET) ||
            (__HAL_DMA_GET_IT_SOURCE(&(uart->dma_rx.handle), DMA_IT_HT) != RESET))
    {
        hw_interrupt_disable();
        recv_total_index = serial->config.rxbufsz - __HAL_DMA_GET_COUNTER(&(uart->dma_rx.handle));
        if (recv_total_index == 0)
        {
            recv_len = serial->config.rxbufsz - uart->dma_rx.last_index;
        }
        else
        {
            recv_len = recv_total_index - uart->dma_rx.last_index;
        }
        uart->dma_rx.last_index = recv_total_index;
        hw_interrupt_enable();

        if (recv_len)
        {
            hw_serial_isr(serial, SERIAL_EVENT_RX_DMAHALF | (recv_len << 8));
        }
    }
}


void USART1_IRQHandler(void)
{
    hw_interrupt_enter();
    uart_isr(&(uart_obj[UART1_INDEX].serial));
    hw_interrupt_leave();
}

void UART1_DMA_RX_IRQHandler(void)
{
    hw_interrupt_enter();
    HAL_DMA_IRQHandler(&uart_obj[UART1_INDEX].dma_rx.handle);
    hw_interrupt_leave();
}

void UART1_DMA_TX_IRQHandler(void)
{
    hw_interrupt_enter();
    HAL_DMA_IRQHandler(&uart_obj[UART1_INDEX].dma_tx.handle);
    hw_interrupt_leave();
}

void USART2_IRQHandler(void)
{
    hw_interrupt_enter();
    uart_isr(&(uart_obj[UART2_INDEX].serial));
    hw_interrupt_leave();
}

void UART2_DMA_RX_IRQHandler(void)
{
    hw_interrupt_enter();
    HAL_DMA_IRQHandler(&uart_obj[UART2_INDEX].dma_rx.handle);
    hw_interrupt_leave();
}

void UART2_DMA_TX_IRQHandler(void)
{
    hw_interrupt_enter();
    HAL_DMA_IRQHandler(&uart_obj[UART2_INDEX].dma_tx.handle);
    hw_interrupt_leave();
}

static void stm32_dma_config(struct serial_device *serial, uint32_t flag)
{
    DMA_HandleTypeDef *DMA_Handle;
    struct dma_config *dma_config;
    struct stm32_uart *uart;
    
    DRV_ASSERT(serial != NULL);
    uart = rt_container_of(serial, struct stm32_uart, serial);

    if (SERIAL_FLAG_DMA_RX == flag)
    {
        DMA_Handle = &uart->dma_rx.handle;
        dma_config = uart->config->dma_rx;
    }
    else if (SERIAL_FLAG_DMA_TX == flag)
    {
        DMA_Handle = &uart->dma_tx.handle;
        dma_config = uart->config->dma_tx;
    }
 
	uint32_t tmpreg = 0x00U;
	/* enable DMA clock && Delay after an RCC peripheral clock enabling*/
	SET_BIT(RCC->AHBENR, dma_config->dma_rcc);
	tmpreg = READ_BIT(RCC->AHBENR, dma_config->dma_rcc);
	UNUSED(tmpreg);
	
    if (SERIAL_FLAG_DMA_RX == flag)
    {
        __HAL_LINKDMA(&(uart->handle), hdmarx, uart->dma_rx.handle);
    }
    else if (SERIAL_FLAG_DMA_TX == flag)
    {
        __HAL_LINKDMA(&(uart->handle), hdmatx, uart->dma_tx.handle);
    }

    DMA_Handle->Instance                 = dma_config->Instance;
    DMA_Handle->Init.PeriphInc           = DMA_PINC_DISABLE;
    DMA_Handle->Init.MemInc              = DMA_MINC_ENABLE;
    DMA_Handle->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    DMA_Handle->Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    
    if (SERIAL_FLAG_DMA_RX == flag)
    {
        DMA_Handle->Init.Direction           = DMA_PERIPH_TO_MEMORY;
        DMA_Handle->Init.Mode                = DMA_CIRCULAR;
    }
    else if (SERIAL_FLAG_DMA_TX == flag)
    {
        DMA_Handle->Init.Direction           = DMA_MEMORY_TO_PERIPH;
        DMA_Handle->Init.Mode                = DMA_NORMAL;
    }
    DMA_Handle->Init.Priority            = DMA_PRIORITY_MEDIUM;

    if (HAL_DMA_DeInit(DMA_Handle) != HAL_OK)
    {
        DRV_ASSERT(0);
    }

    if (HAL_DMA_Init(DMA_Handle) != HAL_OK)
    {
        DRV_ASSERT(0);
    }

    /* enable interrupt */
    if (flag == SERIAL_FLAG_DMA_RX)
    {
        /* Start DMA transfer */
        if (HAL_UART_Receive_DMA(&(uart->handle), serial->rx_buffer , serial->config.rxbufsz) != HAL_OK)
        {
            /* Transfer error in reception process */
            DRV_ASSERT(0);
        }
        CLEAR_BIT(uart->handle.Instance->CR3, USART_CR3_EIE);
        __HAL_UART_ENABLE_IT(&(uart->handle), UART_IT_IDLE);
    }
 
    /* enable irq */
    HAL_NVIC_SetPriority(dma_config->dma_irq, 0, 0);
    HAL_NVIC_EnableIRQ(dma_config->dma_irq);

    HAL_NVIC_SetPriority(uart->config->irq_type, 1, 0);
    HAL_NVIC_EnableIRQ(uart->config->irq_type);
}

/**
  * @brief  UART error callbacks
  * @param  huart: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    DRV_ASSERT(huart != NULL);
    struct stm32_uart *uart = (struct stm32_uart *)huart;
//    LOG_D("%s: %s %d\n", __FUNCTION__, uart->config->name, huart->ErrorCode);
    UNUSED(uart);
}
/**
  * @brief  Rx Transfer completed callback
  * @param  huart: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    struct stm32_uart *uart;
    DRV_ASSERT(huart != NULL);
    uart = (struct stm32_uart *)huart;
    dma_isr(&uart->serial);
}
/**
  * @brief  Rx Half transfer completed callback
  * @param  huart: UART handle
  * @note   This example shows a simple way to report end of DMA Rx Half transfer, 
  *         and you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
    struct stm32_uart *uart;
    DRV_ASSERT(huart != NULL);
    uart = (struct stm32_uart *)huart;
    dma_isr(&uart->serial);
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    struct stm32_uart *uart;
    DRV_ASSERT(huart != NULL);
    uart = (struct stm32_uart *)huart;
    hw_serial_isr(&uart->serial, SERIAL_EVENT_TX_DMADONE);
}


static void stm32_uart_get_dma_config(void)
{
  	/*uart1 config*/
    uart_obj[UART1_INDEX].uart_dma_flag = 0;

    uart_obj[UART1_INDEX].uart_dma_flag |= SERIAL_FLAG_DMA_RX;
    static struct dma_config uart1_dma_rx = UART1_DMA_RX_CONFIG;
    uart_config[UART1_INDEX].dma_rx = &uart1_dma_rx;

    uart_obj[UART1_INDEX].uart_dma_flag |= SERIAL_FLAG_DMA_TX;
    static struct dma_config uart1_dma_tx = UART1_DMA_TX_CONFIG;
    uart_config[UART1_INDEX].dma_tx = &uart1_dma_tx;

	
	/*uart2 config*/
    uart_obj[UART2_INDEX].uart_dma_flag = 0;

    uart_obj[UART2_INDEX].uart_dma_flag |= SERIAL_FLAG_DMA_RX;
    static struct dma_config uart2_dma_rx = UART2_DMA_RX_CONFIG;
    uart_config[UART2_INDEX].dma_rx = &uart2_dma_rx;

    uart_obj[UART2_INDEX].uart_dma_flag |= SERIAL_FLAG_DMA_TX;
    static struct dma_config uart2_dma_tx = UART2_DMA_TX_CONFIG;
    uart_config[UART2_INDEX].dma_tx = &uart2_dma_tx;
};



int hw_usart_init(void)
{
    uint16_t obj_num = sizeof(uart_obj) / sizeof(struct stm32_uart);
    struct serial_configure config = SERIAL_CONFIG_DEFAULT;
    int result = 0;
	uint32_t flag=0;

    stm32_uart_get_dma_config();

    for (int i = 0; i < obj_num; i++)
    {
        uart_obj[i].config = &uart_config[i];
        uart_obj[i].serial.ops    = &stm32_uart_ops;
        uart_obj[i].serial.config = config;
        /* register UART device */
		flag =  SERIAL_FLAG_INT_BYTE_RX|    \
		        SERIAL_FLAG_INT_IDLE_RX|    \
				SERIAL_FLAG_INT_TX|         \
				uart_obj[i].uart_dma_flag;
		
        result = hw_serial_register(&uart_obj[i].serial, uart_obj[i].config->name,
                                  		 flag, NULL);
                                       
        DRV_ASSERT(result == 0);
    }

    return result;
}


