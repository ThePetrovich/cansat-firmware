/*
 * spi.c
 * 
 * Created: 09.02.2022 04:14:05
 * Author: ThePetrovich
 */


#include <drivers/mega128.h>
#include <drivers/bus/spi.h>
#include <avr/io.h>
#include <stdint.h>
#include <stddef.h>

void spi_init(uint16_t flags)
{
	arch_enterCriticalSection();

	/* Setting up SPI pins - data directions to out, MISO pullup enabled */
	/* Note: to use master mode you MUST set HW SS pin to output high (DDR = 1, PORT = 1) */
	/* Otherwise SPIF bit is never set and nothing is transmitted */
	SPI_DDR |= (1 << SPI_PIN_MOSI) | (1 << SPI_PIN_SCK) | (1 << SPI_PIN_SS);
	SPI_DDR &= ~(1 << SPI_PIN_MISO);
	SPI_PORT |= (1 << SPI_PIN_MISO) | (1 << SPI_PIN_SS);

	arch_exitCriticalSection();

	spi_start(flags);
}

void spi_start(uint16_t flags)
{
	arch_enterCriticalSection();
	/* Set 1X / 2X speed */
	SPSR = (SPI_XX_VAL(flags) << SPI2X);

	/* Set SPCR - bit order, clock idle state & polarity, prescaler, interrupts, master mode */
	SPCR = ((SPI_MSB_VAL(flags) << DORD) | (SPI_MODE_VAL(flags) << CPHA) | (SPI_PRESCALER_VAL(flags) << SPR0) | (1 << MSTR) | (1 << SPE));

	arch_exitCriticalSection();
}

void spi_stop(void)
{
	arch_enterCriticalSection();

	/* Disable SPI */
	SPCR = 0;
	SPSR = 0;

	arch_exitCriticalSection();
}

void spi_write(uint8_t data)
{
	/* Send data and wait for transmission */
	SPDR = data;
	while(!(SPSR & (1 << SPIF)));
}

void spi_write_burst(uint8_t *in, uint8_t size)
{
	/* Write multiple bytes from in */
	if (in != NULL && size != 0) {
		for (uint8_t i = 0; i < size; i++) {
			spi_write(in[i]);
		}
	}
}

uint8_t spi_read_write(uint8_t data)
{
	/* Send data and wait for transmission */
	SPDR = data;
	while(!(SPSR & (1 << SPIF)));
	/* Read SPDR and return */
	return SPDR;
}

void spi_read_write_burst(uint8_t *in, uint8_t *out, uint8_t size)
{
	/* Write multiple bytes from in, put response into out */
	if (in != NULL && out != NULL && size != 0) {
		for (uint8_t i = 0; i < size; i++) {
			out[i] = spi_read_write(in[i]);
		}
	}
}

uint8_t spi_read(void)
{
	/* Read SPDR and return */
	return SPDR;
}
