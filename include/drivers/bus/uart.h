/*
 * uart.h
 * 
 * Created: 06.04.2021 05:47:48
 * Author: ThePetrovich
 */


#ifndef YKTS_BOOT_UART_H_
#define YKTS_BOOT_UART_H_

#include <stdint.h>

#ifndef F_CPU
    #warning "F_CPU is not defined"
    #define F_CPU 8000000UL
#endif

#ifndef UART_BAUD_PRESCALE
    #define UART_BAUD_PRESCALE(x) (((F_CPU / ((uint32_t)x * 16UL))) - 1)
#else
    #warning "UART_BAUD_PRESCALE already defined"
#endif

#define UART_RX_BUFFER_SIZE 32
#define UART_RX_BUFFER_MASK (UART_RX_BUFFER_SIZE - 1)

#define UART_TX_BUFFER_SIZE 32
#define UART_TX_BUFFER_MASK (UART_TX_BUFFER_SIZE - 1)

void uart0_init(uint32_t baudrate);

void uart0_putc(char c);
void uart0_puts(char *msg);

char uart0_getc(void);
char uart0_peek(void);
int uart0_available(void);

void uart_flush_rx(void);

#endif /* YKTS_BOOT_UART_H_ */