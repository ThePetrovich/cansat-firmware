/*
 * nrf24l01.c
 *
 * Created: 13.02.2022 13:04:47
 * Author: ThePetrovich
 */ 


#include <drivers/mega128.h>
#include <drivers/nrf24l01.h>
#include <drivers/bus/spi.h>
#include <util/delay.h>
#include <avr/io.h>
#include <stdint.h>

static void nrf24_write_register(uint8_t addr, uint8_t data)
{
	spi_start(SPI_PRESCALER_4 | SPI_MSBFIRST | SPI_MODE0);
	
	nrf24_CS_LOW();
	
	/* Atomically write NRF register, bit mask 0x20 */
	arch_enterCriticalSection();
	spi_write((addr & ~0x20));
	spi_write(data);
	arch_exitCriticalSection();
	
	nrf24_CS_HIGH();
	
	spi_stop();
}

static void nrf24_write_register_burst(uint8_t addr, uint8_t *data, uint8_t size)
{
	spi_start(SPI_PRESCALER_4 | SPI_MSBFIRST | SPI_MODE0);
	
	nrf24_CS_LOW();
	
	/* Atomically write NRF registers, bit mask 0x20 */
	arch_enterCriticalSection();
	spi_write((addr & ~0x20));
	spi_write_burst(data, size);
	arch_exitCriticalSection();
	
	nrf24_CS_HIGH();
	
	spi_stop();
}

static void nrf24_write_command(uint8_t data)
{
	spi_start(SPI_PRESCALER_4 | SPI_MSBFIRST | SPI_MODE0);
	
	nrf24_CS_LOW();
	
	/* Atomically write NRF command, bit mask 0x20 */
	arch_enterCriticalSection();
	spi_write(data);
	arch_exitCriticalSection();
	
	nrf24_CS_HIGH();
	
	spi_stop();
}

void nrf24_init()
{
	uint8_t longcommand[5] = {0};
			
	NRF24_CS_DDR |= (1 << NRF24_CS);
	nrf24_CS_HIGH();
	
	NRF24_CE_DDR |= (1 << NRF24_CE);
	nrf24_CE_HIGH();
	_delay_ms(10);
	nrf24_CE_LOW();
	
	nrf24_write_command(NRF24_CMD_FLUSH);
	nrf24_write_register(NRF24_REG_STATUS, NRF24_REG_STATUS_VALUE);

	longcommand[0] = 0xE7;
	longcommand[1] = 0xE7;
	longcommand[2] = 0xE7;
	longcommand[3] = 0xE7;
	longcommand[4] = 0x01;
	
	nrf24_write_register_burst(NRF24_REG_TX, longcommand, 5);
	
	longcommand[0] = 0xE7;
	longcommand[1] = 0xE7;
	longcommand[2] = 0xE7;
	longcommand[3] = 0xE7;
	longcommand[4] = 0x00;
	
	nrf24_write_register_burst(NRF24_REG_RX_P0, longcommand, 5);
	
	nrf24_write_register(NRF24_REG_AA, NRF24_REG_AA_EN_VALUE);
	nrf24_write_register(NRF24_REG_RXADDR, NRF24_REG_RXADDR_VALUE);
	nrf24_write_register(NRF24_REG_CHANNEL, NRF24_REG_CHANNEL_VALUE);
	nrf24_write_register(NRF24_REG_RFSET, NRF24_REG_RFSET_VALUE);
	nrf24_write_register(NRF24_REG_CONFIG, NRF24_REG_CONFIG_VALUE);
}

void nrf24_transmit_burst(uint8_t *in, uint8_t size)
{
	spi_start(SPI_PRESCALER_4 | SPI_MSBFIRST | SPI_MODE0);
		
	nrf24_CS_LOW();
		
	arch_enterCriticalSection();
	spi_write(NRF24_CMD_TX_UPLOAD);
	spi_write_burst(in, size);
	arch_exitCriticalSection();
	
	/* Enable CE for 1 ms to transmit */
	/* TODO: read status register and wait */
	_delay_ms(2);
	nrf24_CE_HIGH();
	_delay_ms(1);
	nrf24_CE_LOW();
		
	nrf24_CS_HIGH();
		
	spi_stop();
}

void nrf24_transmit(uint8_t data)
{
	spi_start(SPI_PRESCALER_4 | SPI_MSBFIRST | SPI_MODE0);
	
	nrf24_CS_LOW();
	
	arch_enterCriticalSection();
	spi_write(NRF24_CMD_TX_UPLOAD);
	spi_write_burst(&data, 1);
	arch_exitCriticalSection();
	
	/* Enable CE for 1 ms to transmit */
	/* TODO: read status register and wait */
	_delay_ms(2);
	nrf24_CE_HIGH();
	_delay_ms(1);
	nrf24_CE_LOW();
	
	nrf24_CS_HIGH();
	
	spi_stop();
}