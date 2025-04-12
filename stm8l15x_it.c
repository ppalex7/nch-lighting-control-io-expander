#include "stm8l15x_it.h"

#include "uart_logger/uart_logger.h"

#include "pinout.h"

INTERRUPT_HANDLER(NonHandledInterrupt, 0)
{
    GPIOA->ODR &= (uint8_t)(~LED_ERR_MASK);
    log("NonHandledInterrupt occurred!\n");
}

INTERRUPT_HANDLER(DMA1_CHANNEL0_1_IRQHandler, 2)
{
    on_dma_log_transfer_complete();
}
