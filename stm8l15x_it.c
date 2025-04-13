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

INTERRUPT_HANDLER(I2C1_SPI2_IRQHandler, 29)
{
    static uint16_t tx;
    static uint16_t sending_input_state;
    static uint8_t bytes_sent;

    uint8_t sr1;
    uint8_t sr2;
    uint8_t sr3;

    // Check for errors
    sr2 = I2C1->SR2;
    if (sr2)
    {
        // Reset error
        I2C1->SR2 = 0;

        if (sr2 & I2C_SR2_AF && bytes_sent == sizeof(tx))
        {
            // EV3_2
            // not a bug
            return;
        }
        else
        {
            logf("I2C error, SR2: 0x%02hX\n", sr2);
        }
    }

    // read status registers
    sr1 = I2C1->SR1;
    sr3 = I2C1->SR3;
    logf("I2C event, SR1_SR3: 0x%04hX\n", (sr1 << 8) | sr3);

    // ignore SR3 BUSY bit
    sr3 &= (uint8_t)(~I2C_SR3_BUSY);

    if (sr3 == I2C_SR3_TRA)
    {
        // transmitter
        if (sr1 & I2C_SR1_ADDR)
        {
            // address matched
            tx = g_input_state;
            sending_input_state = tx;
            bytes_sent = 0;
        }
        if (sr1 & I2C_SR1_TXE)
        {
            // data register empty
            if (bytes_sent < sizeof(tx))
            {
                I2C1->DR = (uint8_t)tx;
                tx = tx >> 8;
                bytes_sent++;
                logf("I2C: %d bytes sent\n", bytes_sent);
            }
            else
            {
                if (sending_input_state == g_input_state)
                {
                    // actual data was sent, clear flag
                    log("I2C: transmitted state is actual, turn off \"pending request\" flag\n");
                    lower_i2c_flag();
                }
                else
                {
                    log("I2C: transmitted state is stale, keep \"pending request\" flag on\n");
                }
            }
        }
    }
}
