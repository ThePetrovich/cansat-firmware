/*
 * bmp280.h
 *
 * Created: 08.06.2019 22:31:45
 *  Author: ThePetrovich
 */ 


#ifndef BMP280_H_
#define BMP280_H_

#include <stdint.h>

/* BMP280 Chip Select pin */
#define BMP280_CS_PORT				PORTE
#define BMP280_CS_DDR				DDRE
#define BMP280_CS					PE3

/* BMP280 device ID */
#define BMP280_ID					0x58

/* Calibration register definitions */
#define BMP280_REG_DIG_T1			0x88
#define BMP280_REG_DIG_T2			0x8A
#define BMP280_REG_DIG_T3			0x8C
#define BMP280_REG_DIG_P1			0x8E
#define BMP280_REG_DIG_P2			0x90
#define BMP280_REG_DIG_P3			0x92
#define BMP280_REG_DIG_P4			0x94
#define BMP280_REG_DIG_P5			0x96
#define BMP280_REG_DIG_P6			0x98
#define BMP280_REG_DIG_P7			0x9A
#define BMP280_REG_DIG_P8			0x9C
#define BMP280_REG_DIG_P9			0x9E
#define BMP280_REG_CAL26			0xE1

/* Control register definitions */
#define BMP280_REG_CHIPID			0xD0
#define BMP280_REG_VERSION			0xD1
#define BMP280_REG_SOFTRESET		0xE0
#define BMP280_REG_CONTROL			0xF4
#define BMP280_REG_CONFIG			0xF5

/* BMP280 default settings */
/* TODO: check datasheet */
#define BMP280_REG_CONTROL_VALUE	0x3F

/* Data register definitions */
#define BMP280_REG_PRESSUREDATA		0xF7
#define BMP280_REG_TEMPDATA			0xFA

/* Chip Select macros */
#define bmp280_CS_LOW() (BMP280_CS_PORT &= ~(1 << BMP280_CS))
#define bmp280_CS_HIGH() (BMP280_CS_PORT |= (1 << BMP280_CS))

int bmp280_init(void);
double bmp280_read_temperature(void);
double bmp280_read_pressure(void);
double bmp280_calc_altitude(double sea_prs);

#endif /* BMP280_H_ */