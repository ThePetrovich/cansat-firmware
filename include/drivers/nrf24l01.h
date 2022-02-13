/*
 * nrf24l01.h
 *
 * Created: 13.02.2022 13:12:04
 *  Author: localuser-1
 */ 


#ifndef NRF24_H_
#define NRF24_H_

#include <stdint.h>

/* NRF24L01 SPI Chip Select pin */
#define NRF24_CS_PORT				PORTE
#define NRF24_CS_DDR				DDRE
#define NRF24_CS					PE6

/* NRF24L01 Chip Enable pin */
#define NRF24_CE_PORT				PORTE
#define NRF24_CE_DDR				DDRE
#define NRF24_CE					PE5

/* Control register definitions */
#define NRF24_REG_CONFIG			0x00
#define NRF24_REG_AA				0x01
#define NRF24_REG_RXADDR			0x02
#define NRF24_REG_CHANNEL			0x05
#define NRF24_REG_RFSET				0x06
#define NRF24_REG_STATUS			0x07
#define NRF24_REG_RX_P0				0x0A
#define NRF24_REG_TX				0x10

/* NRF24 default settings */
#define NRF24_REG_CONFIG_VALUE		0x0E
#define NRF24_REG_CHANNEL_VALUE		40
#define NRF24_REG_RFSET_VALUE		0x0F
#define NRF24_REG_AA_EN_VALUE		0x01
#define NRF24_REG_RXADDR_VALUE		0x01
#define NRF24_REG_STATUS_VALUE		0x30

/* NRF24 SPI command definitions */
#define NRF24_CMD_TX_UPLOAD			0xA0
#define NRF24_CMD_FLUSH				0xE1

/* SPI Chip Select macros */
#define nrf24_CS_LOW() (NRF24_CS_PORT &= ~(1 << NRF24_CS))
#define nrf24_CS_HIGH() (NRF24_CS_PORT |= (1 << NRF24_CS))

/* NRF24 Chip Enable macros */
#define nrf24_CE_LOW() (NRF24_CE_PORT &= ~(1 << NRF24_CE))
#define nrf24_CE_HIGH() (NRF24_CE_PORT |= (1 << NRF24_CE))

void nrf24_init();
void nrf24_transmit(uint8_t data);
void nrf24_transmit_burst(uint8_t *in, uint8_t size);

#endif /* NRF24_H_ */