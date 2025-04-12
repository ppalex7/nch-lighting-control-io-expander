#include "stm8l15x.h"

#define LED_ERR_MASK ((uint8_t)(1u << 5))
#define LED_OK_MASK ((uint8_t)(1u << 6))

void main(void)
{
    // enable pull-up for unused pins: PA4, PC3, PC4
    GPIOA->CR1 |= 1u << 4;
    GPIOC->CR1 |= 1u << 3;
    GPIOC->CR1 |= 1u << 4;

    // configure pins as output for status LEDs: PA5, PA6
    GPIOA->DDR |= LED_ERR_MASK;
    GPIOA->DDR |= LED_OK_MASK;

    while (1)
    {
    }
}
