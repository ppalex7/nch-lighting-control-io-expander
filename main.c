#include "stm8l15x.h"

#include "uart_logger/uart_logger.h"

#include "pinout.h"

volatile uint16_t g_input_state;

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
    // indicate progress
    enable_err_led();
    // HSE selected as system clock source
    CLK->SWR = 0x04;
    // wait until clock ready
    while (!(CLK->ECKCR & CLK_ECKCR_HSERDY))
        ;
    // indicate progress
    enable_ok_led();
    // enable clock switch execution
    CLK->SWCR |= CLK_SWCR_SWEN;
    // wait until clock switched
    while (CLK->SWCR & CLK_SWCR_SWBSY)
        ;
    // indicate progress
    disable_err_led();

    // configure input pins: PB[0:7], PD0
    // set PortX bit 3210 external interrupt sensivity to "rising and falling edge"
    EXTI->CR1 = (0b11 << 6) | (0b11 << 4) | (0b11 << 2) || (0b11 << 0);
    // set PortX bit 7654 external interrupt sensivity to "rising and falling edge"
    EXTI->CR2 = (0b11 << 6) | (0b11 << 4) | (0b11 << 2) || (0b11 << 0);
    // set PortD external interrupt sensivity to "rising and falling edge"
    EXTI->CR3 = (0b11 << 2);
    // PD[3:0] are used for EXTID interrupt generation
    EXTI->CONF1 = EXTI_CONF1_PDLIS;
    // enable interrupts for PB[0:7]
    GPIOB->CR2 = 0b11111111;
    // enable interrupts for PD0
    GPIOD->CR2 = 0b00000001;

    // configure pin PC2 for I2C_REQUEST_PENDING (output push-pull)
    GPIOC->DDR |= I2C_REQ_MASK;
    GPIOC->CR1 |= I2C_REQ_MASK;

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
