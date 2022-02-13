/*
 * ds18b20.h
 *
 * Created: 17.05.2019 17:58:22
 *  Author: ThePetrovich
 */ 


#ifndef DS18B20_H_
#define DS18B20_H_

#include <stdint.h>

/* DS18B20 Data pin */
#define DS18_DQ_PORT				PORTE
#define DS18_DQ_DDR					DDRE
#define DS18_DQ_PIN					PINE
#define DS18_DQ						PE7

#define DS_RESOLUTION_12BIT			625

/* DS18B20 command definitions */
#define DS18_CMD_CONVERTTEMP		0x44
#define DS18_CMD_RSCRATCHPAD		0xBE
#define DS18_CMD_WSCRATCHPAD		0x4E
#define DS18_CMD_CPYSCRATCHPAD		0x48
#define DS18_CMD_RECEEPROM			0xB8
#define DS18_CMD_RPWRSUPPLY			0xB4
#define DS18_CMD_SEARCHROM			0xF0
#define DS18_CMD_READROM			0x33
#define DS18_CMD_MATCHROM			0x55
#define DS18_CMD_SKIPROM			0xCC
#define DS18_CMD_ALARMSEARCH		0xEC

void ds18b20_request_temperature(void);
double ds18b20_read_temperature(void);

#endif /* DS18B20_H_ */