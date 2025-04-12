#include "stm8l15x.h"

void main(void)
{
    // enable pull-up for unused pins: PA4, PC3, PC4
    GPIOA->CR1 |= 1u << 4;
    GPIOC->CR1 |= 1u << 3;
    GPIOC->CR1 |= 1u << 4;

    while (1)
    {
    }
}
