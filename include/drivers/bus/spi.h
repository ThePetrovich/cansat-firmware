/*
 * spi.h
 * 
 * Created: 06.04.2021 05:47:38
 * Author: ThePetrovich
 */


#ifndef YKTS_BOOT_SPI_H_
#define YKTS_BOOT_SPI_H_

#include <stdint.h>
#include <drivers/mega128.h>

/* ATmega128 SPI port is on PORTB */
#define SPI_PORT				PORTB
#define SPI_DDR		 			DDRB

/* SPI PORTB pins */
#define SPI_PIN_MISO			PB3
#define SPI_PIN_MOSI			PB2
#define SPI_PIN_SCK				PB1
#define SPI_PIN_SS				PB0

/* SPI bit order predefines */
#define SPI_MSBFIRST			(0)
#define SPI_LSBFIRST			(1)
#define SPI_MSB_VAL(x)			((uint8_t)(x & (1)))

/* SPI speed predefines */
#define SPI_PRESCALER_4 		(0 << 1)
#define SPI_PRESCALER_16 		(1 << 1)
#define SPI_PRESCALER_64 		(2 << 1)
#define SPI_PRESCALER_128 		(3 << 1)
#define SPI_PRESCALER_VAL(x)	((uint8_t)((x & (3 << 1)) >> 1))

/* SPI speed multipliers */
#define SPI_2X					(1 << 3)
#define SPI_1X					(0 << 3)
#define SPI_XX_VAL(x)			((uint8_t)((x & (1 << 3)) >> 3))

/* SPI mode predefines */
#define SPI_MODE0 				(0 << 4)
#define SPI_MODE1 				(1 << 4)
#define SPI_MODE2				(2 << 4)
#define SPI_MODE3				(3 << 4)
#define SPI_MODE_VAL(x)			((uint8_t)((x & (3 << 4)) >> 4))

/*
 *	SPI modes:
 *	0 - 0b00 - CPOL=0, CPHA=0 - mode 0
 *	1 - 0b01 - CPOL=0, CPHA=1 - mode 1
 *	2 - 0b10 - CPOL=1, CPHA=0 - mode 2
 *	3 - 0b11 - CPOL=1, CPHA=1 - mode 3
 */

#define SPI_RX_BUFFER_SIZE 32
#define SPI_RX_BUFFER_MASK (SPI_RX_BUFFER_SIZE - 1)

#define spi_CS_LOW() SPI_PORT &= ~(1 << SPI_PIN_SS)
#define spi_CS_HIGH() SPI_PORT |= (1 << SPI_PIN_SS)

void spi_init(uint16_t flags);

void spi_start(uint16_t flags);
void spi_stop(void);

void spi_write(uint8_t data);
uint8_t spi_read(void);
uint8_t spi_read_write(uint8_t data);

void spi_write_burst(uint8_t *in, uint8_t size);
void spi_read_write_burst(uint8_t *in, uint8_t *out, uint8_t size);

#endif /* YKTS_BOOT_SPI_H_ */