#ifndef __STM8L15x_IT_H
#define __STM8L15x_IT_H

#include "stm8l15x.h"

void _stext(void); /* RESET startup routine */
INTERRUPT void NonHandledInterrupt(void);
INTERRUPT void DMA1_CHANNEL0_1_IRQHandler(void); /* DMA1 Channel0/1*/

#endif /* __STM8L15x_IT_H */
