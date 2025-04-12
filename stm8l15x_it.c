#include "stm8l15x_it.h"

#include "uart_logger/uart_logger.h"

#include "pinout.h"

extern volatile uint16_t g_input_state;

INTERRUPT_HANDLER(NonHandledInterrupt, 0)
{
    enable_err_led();
    log("NonHandledInterrupt occurred!\n");
}

INTERRUPT_HANDLER(DMA1_CHANNEL0_1_IRQHandler, 2)
{
    on_dma_log_transfer_complete();
}

INTERRUPT_HANDLER(EXTIN_IRQHandler, 8)
{
    uint8_t new_state_low;

    // invert state (buttons pulled-up)
    new_state_low = (uint8_t)(~GPIOB->IDR);
    // pack PORTB state to lower byte
    g_input_state = new_state_low | (g_input_state & 0xFF00);

    raise_i2c_flag();
    logf("EXTIN handler: new state 0x%04hX\n", g_input_state);

    // clear pending interrupt bit
    EXTI->SR1 = 0b11111111;
}

INTERRUPT_HANDLER(EXTID_H_IRQHandler, 7)
{
    uint8_t new_state_high;

    // invert state (buttons pulled-up) and filter
    new_state_high = (uint8_t)((uint8_t)(~GPIOD->IDR) & 0b00000001);

    // pack PORTD into higher byte
    g_input_state = (new_state_high << 8) | (g_input_state & 0x00FF);

    raise_i2c_flag();
    logf("EXTID handler: new state 0x%04hX\n", g_input_state);

    // clear pending interrupt bit: PDF
    EXTI->SR2 |= (1u << 1);
}
