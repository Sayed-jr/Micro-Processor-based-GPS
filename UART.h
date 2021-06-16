#ifndef __UART_H__
#define	__UART_H__

#include "stdint.h"
void delay( uint32_t d );
void UART0_Init(void);
void UART2_init(void) ;

uint8_t UART2_Available(void);

uint8_t UART2_Read(void);

void UART2_Write(uint8_t data);
void UART0_Write(uint8_t data);


#endif // __UART_H__
