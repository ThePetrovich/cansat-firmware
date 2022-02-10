/*
 * ds18b20.h
 *
 * Created: 17.05.2019 17:58:22
 *  Author: ThePetrovich
 */ 


#ifndef DS18B20_H_
#define DS18B20_H_

#include <stdint.h>

/* BMP280 Chip Select pin */
#define DS18B20_PORT		PORTE
#define DS18B20_DDR			DDRE
#define DS18B20_PIN			PINE
#define DS18B20_PIN_DATA	PE2

#define DS_RESOLUTION_12BIT 625

/* DS18B20 command definitions */
#define DS_CMD_CONVERTTEMP 0x44
#define DS_CMD_RSCRATCHPAD 0xBE
#define DS_CMD_WSCRATCHPAD 0x4E
#define DS_CMD_CPYSCRATCHPAD 0x48
#define DS_CMD_RECEEPROM 0xB8
#define DS_CMD_RPWRSUPPLY 0xB4
#define DS_CMD_SEARCHROM 0xF0
#define DS_CMD_READROM 0x33
#define DS_CMD_MATCHROM 0x55
#define DS_CMD_SKIPROM 0xCC
#define DS_CMD_ALARMSEARCH 0xEC

void ds18b20_request_temperature(void);
double ds18b20_read_temperature(void);

#endif /* DS18B20_H_ */