#ifndef __STM8L15x_IT_H
#define __STM8L15x_IT_H

#include "stm8l15x.h"

void _stext(void); /* RESET startup routine */
INTERRUPT void NonHandledInterrupt(void);
INTERRUPT void DMA1_CHANNEL0_1_IRQHandler(void); /* DMA1 Channel0/1*/
INTERRUPT void EXTIN_IRQHandler(void); /* EXTI PIN[0:7] */
INTERRUPT void EXTID_H_IRQHandler(void); /* EXTI PORTD / EXTI PORTH*/
INTERRUPT void I2C1_SPI2_IRQHandler(void); /* I2C1 / SPI2 */

#endif /* __STM8L15x_IT_H */
