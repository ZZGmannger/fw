
#ifndef DRV_UART
#define DRV_UART

#include "stm32f1xx_hal.h"
#include "board.h"
#include "serial.h"

#define UART1_DMA_RX_IRQHandler         DMA1_Channel5_IRQHandler
#define UART1_RX_DMA_RCC                RCC_AHBENR_DMA1EN
#define UART1_RX_DMA_INSTANCE           DMA1_Channel5
#define UART1_RX_DMA_IRQ                DMA1_Channel5_IRQn

#define UART1_DMA_TX_IRQHandler         DMA1_Channel4_IRQHandler
#define UART1_TX_DMA_RCC                RCC_AHBENR_DMA1EN
#define UART1_TX_DMA_INSTANCE           DMA1_Channel4
#define UART1_TX_DMA_IRQ                DMA1_Channel4_IRQn

#define UART2_DMA_RX_IRQHandler         DMA1_Channel6_IRQHandler
#define UART2_RX_DMA_RCC                RCC_AHBENR_DMA1EN
#define UART2_RX_DMA_INSTANCE           DMA1_Channel6
#define UART2_RX_DMA_IRQ                DMA1_Channel6_IRQn

#define UART2_DMA_TX_IRQHandler         DMA1_Channel7_IRQHandler
#define UART2_TX_DMA_RCC                RCC_AHBENR_DMA1EN
#define UART2_TX_DMA_INSTANCE           DMA1_Channel7
#define UART2_TX_DMA_IRQ                DMA1_Channel7_IRQn



struct dma_config 
{
    DMA_Channel_TypeDef *Instance;
    uint32_t dma_rcc;
    IRQn_Type dma_irq;
    uint32_t channel;
};

/* stm32 config class */
struct stm32_uart_config
{
    const char *name;
    USART_TypeDef *Instance;
    IRQn_Type irq_type;
    struct dma_config *dma_rx;
    struct dma_config *dma_tx;
};

/* stm32 uart dirver class */
struct stm32_uart
{
    UART_HandleTypeDef handle;
    struct stm32_uart_config *config;
    
    struct
    {
        DMA_HandleTypeDef handle;
        uint16_t last_index;
    } dma_rx;
    struct
    {
        DMA_HandleTypeDef handle;
    } dma_tx;
 
    uint16_t uart_dma_flag;
    struct serial_device serial;
};


int hw_usart_init(void);


#endif