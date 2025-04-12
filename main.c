#include "stm8l15x.h"

#include "uart_logger/uart_logger.h"

#include "pinout.h"

void main(void)
{
    // enable pull-up for unused pins: PA4, PC3, PC4
    GPIOA->CR1 |= 1u << 4;
    GPIOC->CR1 |= 1u << 3;
    GPIOC->CR1 |= 1u << 4;

    // configure pins as output for status LEDs: PA5, PA6
    GPIOA->DDR |= LED_ERR_MASK;
    GPIOA->DDR |= LED_OK_MASK;

    // configure clock to 12 MHz fed from HSE
    // set system clock presclaer to 1
    CLK->CKDIVR = 0;
    // indicate progress by turning LED_ERR on
    GPIOA->ODR &= (uint8_t)(~LED_ERR_MASK);
    // HSE selected as system clock source
    CLK->SWR = 0x04;
    // wait until clock ready
    while (!(CLK->ECKCR & CLK_ECKCR_HSERDY))
        ;
    // indicate progress by turning LED_OK on
    GPIOA->ODR &= (uint8_t)(~LED_OK_MASK);
    // enable clock switch execution
    CLK->SWCR |= CLK_SWCR_SWEN;
    // wait until clock switched
    while (CLK->SWCR & CLK_SWCR_SWBSY)
        ;
    // indicate progress by turning LED_ERR off
    GPIOA->ODR |= LED_ERR_MASK;

    // set UART BaudRate to 115200, so divider 12000000/115200 = 104 = 0x68
    configure_logger_peripheral(0x68u);

    enableInterrupts();

    log("device configured\n");

    while (1)
    {
        process_buffered_logs();

        wfi();
    }
}
