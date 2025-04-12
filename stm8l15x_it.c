#include "stm8l15x_it.h"

#include "uart_logger/uart_logger.h"

#include "pinout.h"

INTERRUPT_HANDLER(NonHandledInterrupt, 0)
{
    enable_err_led();
    log("NonHandledInterrupt occurred!\n");
}

INTERRUPT_HANDLER(DMA1_CHANNEL0_1_IRQHandler, 2)
{
    on_dma_log_transfer_complete();
}
