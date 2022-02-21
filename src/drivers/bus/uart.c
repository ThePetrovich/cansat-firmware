/*
 * uart.c
 * 
 * Created: 09.02.2022 04:14:05
 * Author: ThePetrovich
 */


#include <drivers/bus/uart.h>
#include <drivers/mega128.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

static char buf_rx[UART_RX_BUFFER_SIZE];
static uint8_t buf_rx_head;
static uint8_t buf_rx_tail;

void uart0_init(uint32_t baudrate)
{
	uint32_t ubrr = UART_BAUD_PRESCALE(baudrate);

	arch_enterCriticalSection();
	buf_rx_head = 0;
	buf_rx_tail = 0;
	arch_exitCriticalSection();

	/* Splitting UBRR into UBRRH & UBRRL */
	UBRR0L = (uint8_t)(ubrr & (uint32_t) 0xFF); /* Lowest 8 bits */
	ubrr >>= 8;
	UBRR0H = (uint8_t)(ubrr & (uint32_t) 0xFF); /* Upper 8 bits (out of 16) */

	/* UART setup: RXEN & TXEN = 1, RX interrupt enabled, 8 bits, 1 stop bit, no parity - default */
	arch_enterCriticalSection();
	UCSR0A = 0;
	UCSR0B = (uint8_t)((1 << RXEN) | (1 << TXEN) | (1 << RXCIE) | (0 << TXCIE) | (0 << UDRIE));
	UCSR0C = (uint8_t)((0 << USBS) | (1 << UCSZ00) | (1 << UCSZ01) | (0 << UCSZ02) | (0 << UPM00) | (0 << UPM01) | (0 << UMSEL0));
	arch_exitCriticalSection();
}

void uart0_putc(char c)
{
	/* Send char & wait */
	while (!((UCSR0A) & (1 << UDRE)));
	UDR0 = c;
}

void uart0_puts(char *msg)
{
	while (*msg) {
		uart0_putc(*msg++);
	}
}

char uart0_getc(void)
{
	uint8_t rx_tail = 0;
	char data = '\0';

	if (buf_rx_head == buf_rx_tail) {
		return '\0';   /* RX buffer empty */
	}

	/* Calculate buffer index, wrap on overflow */
	rx_tail = (buf_rx_tail + 1) & UART_RX_BUFFER_MASK;
	buf_rx_tail = rx_tail;

	/* Get char from RX buffer */
	data = buf_rx[rx_tail];

	return data;
}

char uart0_peek(void)
{
	uint8_t rx_tail = 0;
	char data = '\0';

	if (buf_rx_head == buf_rx_tail) {
		return '\0';   /* RX buffer empty */
	}

	/* Calculate buffer index, wrap on overflow */
	rx_tail = (buf_rx_tail + 1) & UART_RX_BUFFER_MASK;
	/* Proceed without updating buf_rx_tail */

	/* Get char from RX buffer */
	data = buf_rx[rx_tail];

	return data;
}

int uart0_available(void)
{
	int available = 0;

	arch_enterCriticalSection();
	available = (UART_RX_BUFFER_SIZE + buf_rx_head - buf_rx_tail) & UART_RX_BUFFER_MASK;
	arch_exitCriticalSection();

	return available;
}

void uart0_flush_rx(void)
{
	buf_rx_head = buf_rx_tail;
}

ISR(USART0_RX_vect)
{
	char data = '\0';
	uint8_t rx_head = 0;

	data = UDR0;

	/* Calculate buffer index, wrap on overflow */
	rx_head = (buf_rx_head + 1) & UART_RX_BUFFER_MASK;

	/* Store data if buffer is not full */
	if (rx_head != buf_rx_tail) {
		buf_rx_head = rx_head;
		buf_rx[rx_head] = data;
	}
}