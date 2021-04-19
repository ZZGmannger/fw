#include "drv_gpio.h"

#define __STM32_PIN(index, gpio, gpio_index)                                \
	{                                                                       \
		index, GPIO##gpio, GPIO_PIN_##gpio_index                            \
	}

static const struct pin_index pins[] = 
{
#if defined(GPIOA)
    __STM32_PIN(0 ,  A, 0 ),
    __STM32_PIN(1 ,  A, 1 ),
    __STM32_PIN(2 ,  A, 2 ),
    __STM32_PIN(3 ,  A, 3 ),
    __STM32_PIN(4 ,  A, 4 ),
    __STM32_PIN(5 ,  A, 5 ),
    __STM32_PIN(6 ,  A, 6 ),
    __STM32_PIN(7 ,  A, 7 ),
    __STM32_PIN(8 ,  A, 8 ),
    __STM32_PIN(9 ,  A, 9 ),
    __STM32_PIN(10,  A, 10),
    __STM32_PIN(11,  A, 11),
    __STM32_PIN(12,  A, 12),
    __STM32_PIN(13,  A, 13),
    __STM32_PIN(14,  A, 14),
    __STM32_PIN(15,  A, 15),
#if defined(GPIOB)
    __STM32_PIN(16,  B, 0),
    __STM32_PIN(17,  B, 1),
    __STM32_PIN(18,  B, 2),
    __STM32_PIN(19,  B, 3),
    __STM32_PIN(20,  B, 4),
    __STM32_PIN(21,  B, 5),
    __STM32_PIN(22,  B, 6),
    __STM32_PIN(23,  B, 7),
    __STM32_PIN(24,  B, 8),
    __STM32_PIN(25,  B, 9),
    __STM32_PIN(26,  B, 10),
    __STM32_PIN(27,  B, 11),
    __STM32_PIN(28,  B, 12),
    __STM32_PIN(29,  B, 13),
    __STM32_PIN(30,  B, 14),
    __STM32_PIN(31,  B, 15),
#if defined(GPIOC)
    __STM32_PIN(32,  C, 0),
    __STM32_PIN(33,  C, 1),
    __STM32_PIN(34,  C, 2),
    __STM32_PIN(35,  C, 3),
    __STM32_PIN(36,  C, 4),
    __STM32_PIN(37,  C, 5),
    __STM32_PIN(38,  C, 6),
    __STM32_PIN(39,  C, 7),
    __STM32_PIN(40,  C, 8),
    __STM32_PIN(41,  C, 9),
    __STM32_PIN(42,  C, 10),
    __STM32_PIN(43,  C, 11),
    __STM32_PIN(44,  C, 12),
    __STM32_PIN(45,  C, 13),
    __STM32_PIN(46,  C, 14),
    __STM32_PIN(47,  C, 15),
#if defined(GPIOD)
    __STM32_PIN(48,  D, 0),
    __STM32_PIN(49,  D, 1),
    __STM32_PIN(50,  D, 2),
    __STM32_PIN(51,  D, 3),
    __STM32_PIN(52,  D, 4),
    __STM32_PIN(53,  D, 5),
    __STM32_PIN(54,  D, 6),
    __STM32_PIN(55,  D, 7),
    __STM32_PIN(56,  D, 8),
    __STM32_PIN(57,  D, 9),
    __STM32_PIN(58,  D, 10),
    __STM32_PIN(59,  D, 11),
    __STM32_PIN(60,  D, 12),
    __STM32_PIN(61,  D, 13),
    __STM32_PIN(62,  D, 14),
    __STM32_PIN(63,  D, 15),
#if defined(GPIOE)
    __STM32_PIN(64,  E, 0),
    __STM32_PIN(65,  E, 1),
    __STM32_PIN(66,  E, 2),
    __STM32_PIN(67,  E, 3),
    __STM32_PIN(68,  E, 4),
    __STM32_PIN(69,  E, 5),
    __STM32_PIN(70,  E, 6),
    __STM32_PIN(71,  E, 7),
    __STM32_PIN(72,  E, 8),
    __STM32_PIN(73,  E, 9),
    __STM32_PIN(74,  E, 10),
    __STM32_PIN(75,  E, 11),
    __STM32_PIN(76,  E, 12),
    __STM32_PIN(77,  E, 13),
    __STM32_PIN(78,  E, 14),
    __STM32_PIN(79,  E, 15),
#if defined(GPIOF)
    __STM32_PIN(80,  F, 0),
    __STM32_PIN(81,  F, 1),
    __STM32_PIN(82,  F, 2),
    __STM32_PIN(83,  F, 3),
    __STM32_PIN(84,  F, 4),
    __STM32_PIN(85,  F, 5),
    __STM32_PIN(86,  F, 6),
    __STM32_PIN(87,  F, 7),
    __STM32_PIN(88,  F, 8),
    __STM32_PIN(89,  F, 9),
    __STM32_PIN(90,  F, 10),
    __STM32_PIN(91,  F, 11),
    __STM32_PIN(92,  F, 12),
    __STM32_PIN(93,  F, 13),
    __STM32_PIN(94,  F, 14),
    __STM32_PIN(95,  F, 15),
#if defined(GPIOG)
    __STM32_PIN(96,  G, 0),
    __STM32_PIN(97,  G, 1),
    __STM32_PIN(98,  G, 2),
    __STM32_PIN(99,  G, 3),
    __STM32_PIN(100, G, 4),
    __STM32_PIN(101, G, 5),
    __STM32_PIN(102, G, 6),
    __STM32_PIN(103, G, 7),
    __STM32_PIN(104, G, 8),
    __STM32_PIN(105, G, 9),
    __STM32_PIN(106, G, 10),
    __STM32_PIN(107, G, 11),
    __STM32_PIN(108, G, 12),
    __STM32_PIN(109, G, 13),
    __STM32_PIN(110, G, 14),
    __STM32_PIN(111, G, 15),
#if defined(GPIOH)
    __STM32_PIN(112, H, 0),
    __STM32_PIN(113, H, 1),
    __STM32_PIN(114, H, 2),
    __STM32_PIN(115, H, 3),
    __STM32_PIN(116, H, 4),
    __STM32_PIN(117, H, 5),
    __STM32_PIN(118, H, 6),
    __STM32_PIN(119, H, 7),
    __STM32_PIN(120, H, 8),
    __STM32_PIN(121, H, 9),
    __STM32_PIN(122, H, 10),
    __STM32_PIN(123, H, 11),
    __STM32_PIN(124, H, 12),
    __STM32_PIN(125, H, 13),
    __STM32_PIN(126, H, 14),
    __STM32_PIN(127, H, 15),
#endif /* defined(GPIOH) */
#endif /* defined(GPIOG) */
#endif /* defined(GPIOF) */
#endif /* defined(GPIOE) */
#endif /* defined(GPIOD) */
#endif /* defined(GPIOC) */
#endif /* defined(GPIOB) */
#endif /* defined(GPIOA) */
};


static const struct pin_irq_map pin_irq_map[] =
{
    {GPIO_PIN_0, EXTI0_IRQn},
    {GPIO_PIN_1, EXTI1_IRQn},
    {GPIO_PIN_2, EXTI2_IRQn},
    {GPIO_PIN_3, EXTI3_IRQn},
    {GPIO_PIN_4, EXTI4_IRQn},
    {GPIO_PIN_5, EXTI9_5_IRQn},
    {GPIO_PIN_6, EXTI9_5_IRQn},
    {GPIO_PIN_7, EXTI9_5_IRQn},
    {GPIO_PIN_8, EXTI9_5_IRQn},
    {GPIO_PIN_9, EXTI9_5_IRQn},
    {GPIO_PIN_10, EXTI15_10_IRQn},
    {GPIO_PIN_11, EXTI15_10_IRQn},
    {GPIO_PIN_12, EXTI15_10_IRQn},
    {GPIO_PIN_13, EXTI15_10_IRQn},
    {GPIO_PIN_14, EXTI15_10_IRQn},
    {GPIO_PIN_15, EXTI15_10_IRQn},
};


static struct pin_irq_hdr pin_irq_hdr_tab[] =
{
    {-1, 0, NULL, NULL},
    {-1, 0, NULL, NULL},
    {-1, 0, NULL, NULL},
    {-1, 0, NULL, NULL},
    {-1, 0, NULL, NULL},
    {-1, 0, NULL, NULL},
    {-1, 0, NULL, NULL},
    {-1, 0, NULL, NULL},
    {-1, 0, NULL, NULL},
    {-1, 0, NULL, NULL},
    {-1, 0, NULL, NULL},
    {-1, 0, NULL, NULL},
    {-1, 0, NULL, NULL},
    {-1, 0, NULL, NULL},
    {-1, 0, NULL, NULL},
    {-1, 0, NULL, NULL},
};
static uint32_t pin_irq_enable_mask=0;


#define ITEM_NUM(items) sizeof(items) / sizeof(items[0])
static const struct pin_index *get_pin(uint8_t pin)
{
    const struct pin_index *index;

    if (pin < ITEM_NUM(pins))
    {
        index = &pins[pin];
        if (index->index == -1)
            index = NULL;
    }
    else
    {
        index = NULL;
    }

    return index;
};

static void stm32_pin_write(uint8_t pin, uint8_t value)
{
    const struct pin_index *index;

    index = get_pin(pin);
    if (index == NULL)
    {
        return;
    }

    HAL_GPIO_WritePin(index->gpio, index->pin, (GPIO_PinState)value);
}

static int stm32_pin_read(uint8_t pin)
{
    int value;
    const struct pin_index *index;

    value = PIN_LOW;

    index = get_pin(pin);
    if (index == NULL)
    {
        return value;
    }

    value = HAL_GPIO_ReadPin(index->gpio, index->pin);

    return value;
}


static void stm32_pin_mode(uint8_t pin, uint8_t mode)
{
    const struct pin_index *index;
    GPIO_InitTypeDef GPIO_InitStruct;

    index = get_pin(pin);
    if (index == NULL)
    {
        return;
    }

    /* Configure GPIO_InitStructure */
    GPIO_InitStruct.Pin = index->pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    if (mode == PIN_MODE_OUTPUT)
    {
        /* output setting */
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
    }
    else if (mode == PIN_MODE_INPUT)
    {
        /* input setting: not pull. */
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
    }
    else if (mode == PIN_MODE_INPUT_PULLUP)
    {
        /* input setting: pull up. */
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
    }
    else if (mode == PIN_MODE_INPUT_PULLDOWN)
    {
        /* input setting: pull down. */
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    }
    else if (mode == PIN_MODE_OUTPUT_OD)
    {
        /* output setting: od. */
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
    }

    HAL_GPIO_Init(index->gpio, &GPIO_InitStruct);
}

/*irq--------------------------------------------------------------------------*/
static inline int bit2bitno(uint32_t bit)
{
    int i;
    for (i = 0; i < 32; i++)
    {
        if ((0x01 << i) == bit)
        {
            return i;
        }
    }
    return -1;
}
static inline const struct pin_irq_map *get_pin_irq_map(uint32_t pinbit)
{
    int32_t mapindex = bit2bitno(pinbit);
    if (mapindex < 0 || mapindex >= ITEM_NUM(pin_irq_map))
    {
        return NULL;
    }
    return &pin_irq_map[mapindex];
};


static int stm32_pin_attach_irq(uint8_t pin,uint8_t mode,
                                void (*hdr)(void *args), void *args)
{
    const struct pin_index *index;
    int irqindex = -1;

    index = get_pin(pin);
    if (index == NULL)
    {
        return -1;
    }
    irqindex = bit2bitno(index->pin);
    if (irqindex < 0 || irqindex >= ITEM_NUM(pin_irq_map))
    {
        return -1;
    }

    hw_interrupt_disable();
	
    if (pin_irq_hdr_tab[irqindex].pin == pin &&
            pin_irq_hdr_tab[irqindex].hdr == hdr &&
            pin_irq_hdr_tab[irqindex].mode == mode &&
            pin_irq_hdr_tab[irqindex].args == args)
    {
        hw_interrupt_enable();
        return 0;
    }
    if (pin_irq_hdr_tab[irqindex].pin != -1)
    {
        hw_interrupt_enable();
        return -2;
    }
    pin_irq_hdr_tab[irqindex].pin = pin;
    pin_irq_hdr_tab[irqindex].hdr = hdr;
    pin_irq_hdr_tab[irqindex].mode = mode;
    pin_irq_hdr_tab[irqindex].args = args;
    hw_interrupt_enable();

    return 0;
}

static int stm32_pin_dettach_irq(uint8_t pin)
{
    const struct pin_index *index;

    int irqindex = -1;

    index = get_pin(pin);
    if (index == NULL)
    {
        return -1;
    }
    irqindex = bit2bitno(index->pin);
    if (irqindex < 0 || irqindex >= ITEM_NUM(pin_irq_map))
    {
        return -1;
    }

    hw_interrupt_disable();
    if (pin_irq_hdr_tab[irqindex].pin == -1)
    {
        hw_interrupt_enable();
        return 0;
    }
    pin_irq_hdr_tab[irqindex].pin = -1;
    pin_irq_hdr_tab[irqindex].hdr = NULL;
    pin_irq_hdr_tab[irqindex].mode = 0;
    pin_irq_hdr_tab[irqindex].args = NULL;
    hw_interrupt_enable();

    return 0;
}


static int stm32_pin_irq_enable(uint8_t pin,uint8_t enabled)
                                     
{
    const struct pin_index *index;
    const struct pin_irq_map *irqmap;
   
    int irqindex = -1;
    GPIO_InitTypeDef GPIO_InitStruct;

    index = get_pin(pin);
    if (index == NULL)
    {
        return -1;
    }

    if (enabled == PIN_IRQ_ENABLE)
    {
        irqindex = bit2bitno(index->pin);
        if (irqindex < 0 || irqindex >= ITEM_NUM(pin_irq_map))
        {
            return -1;
        }

        hw_interrupt_disable();

        if (pin_irq_hdr_tab[irqindex].pin == -1)
        {
            hw_interrupt_enable();
            return -1;
        }

        irqmap = &pin_irq_map[irqindex];

        /* Configure GPIO_InitStructure */
        GPIO_InitStruct.Pin = index->pin;        
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        switch (pin_irq_hdr_tab[irqindex].mode)
        {
        case PIN_IRQ_MODE_RISING:
            GPIO_InitStruct.Pull = GPIO_PULLDOWN;
            GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
            break;
        case PIN_IRQ_MODE_FALLING:
            GPIO_InitStruct.Pull = GPIO_PULLUP;
            GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
            break;
        case PIN_IRQ_MODE_RISING_FALLING:
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
            break;
        }
        HAL_GPIO_Init(index->gpio, &GPIO_InitStruct);

        HAL_NVIC_SetPriority(irqmap->irqno, 5, 0);
        HAL_NVIC_EnableIRQ(irqmap->irqno);
        pin_irq_enable_mask |= irqmap->pinbit;

        hw_interrupt_enable();
    }
    else if (enabled == PIN_IRQ_DISABLE)
    {
        irqmap = get_pin_irq_map(index->pin);
        if (irqmap == NULL)
        {
            return -1;
        }

        hw_interrupt_disable();

        HAL_GPIO_DeInit(index->gpio, index->pin);

        pin_irq_enable_mask &= ~irqmap->pinbit;
   
        if (( irqmap->pinbit>=GPIO_PIN_5 )&&( irqmap->pinbit<=GPIO_PIN_9 ))
        {
            if(!(pin_irq_enable_mask&(GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9)))
            {    
                HAL_NVIC_DisableIRQ(irqmap->irqno);
            }
        }
        else if (( irqmap->pinbit>=GPIO_PIN_10 )&&( irqmap->pinbit<=GPIO_PIN_15 ))
        {
            if(!(pin_irq_enable_mask&(GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15)))
            {    
                HAL_NVIC_DisableIRQ(irqmap->irqno);
            }
        }
        else
        {
            HAL_NVIC_DisableIRQ(irqmap->irqno);
        }               
        hw_interrupt_enable();  
    }
    else
    {
        return -2;
    }

    return 0;
}

const static struct pin_ops _stm32_pin_ops =
{
    stm32_pin_mode,
    stm32_pin_write,
    stm32_pin_read,
    stm32_pin_attach_irq,
    stm32_pin_dettach_irq,
    stm32_pin_irq_enable,
};

static inline void pin_irq_hdr(int irqno)
{
    if (pin_irq_hdr_tab[irqno].hdr)
    {
        pin_irq_hdr_tab[irqno].hdr(pin_irq_hdr_tab[irqno].args);
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    pin_irq_hdr(bit2bitno(GPIO_Pin));
}


void EXTI0_IRQHandler(void)
{
    interrupt_enter();
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
    interrupt_leave();
}

void EXTI1_IRQHandler(void)
{
    interrupt_enter();
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
    interrupt_leave();
}

void EXTI2_IRQHandler(void)
{
    interrupt_enter();
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
    interrupt_leave();
}

void EXTI3_IRQHandler(void)
{
    interrupt_enter();
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
    interrupt_leave();
}

void EXTI4_IRQHandler(void)
{
    interrupt_enter();
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
    interrupt_leave();
}

void EXTI9_5_IRQHandler(void)
{
    interrupt_enter();
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
    interrupt_leave();
}

void EXTI15_10_IRQHandler(void)
{
    interrupt_enter();
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
    interrupt_leave();
}


int hw_pin_init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

	device_pin_register("pin" ,  &_stm32_pin_ops);
	return 0;
}


