#ifndef __PINOUT_H
#define __PINOUT_H

#include "stm8l15x.h"

#define LED_ERR_MASK ((uint8_t)(1u << 5))
#define LED_OK_MASK ((uint8_t)(1u << 6))
#define I2C_REQ_MASK ((uint8_t)(1u << 2))
#define PORTD_OUTPUT_MASK 0b11111110

@ inline void enable_ok_led(void)
{
    GPIOA->ODR &= (uint8_t)(~LED_OK_MASK);
}

@ inline void disable_ok_led(void)
{
    GPIOA->ODR |= LED_OK_MASK;
}

@ inline void enable_err_led(void)
{
    GPIOA->ODR &= (uint8_t)(~LED_ERR_MASK);
}

@ inline void disable_err_led(void)
{
    GPIOA->ODR |= LED_ERR_MASK;
}

@ inline void raise_i2c_flag(void)
{
    GPIOC->ODR |= I2C_REQ_MASK;
}

@ inline void lower_i2c_flag(void)
{
    GPIOC->ODR &= (uint8_t)(~I2C_REQ_MASK);
}

#endif
