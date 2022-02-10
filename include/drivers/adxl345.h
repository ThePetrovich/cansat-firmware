/*
 * adxl345.h
 *
 * Created: 23.03.2019 17:28:15
 *  Author: ThePetrovich
 */ 


#ifndef ADXL345_H_
#define ADXL345_H_

#include <stdint.h>

/* ADXL345 Chip Select pin */
#define ADXL345_PORT		PORTE
#define ADXL345_DDR		 	DDRE
#define ADXL345_PIN_CS		PE4

/* ADXL device ID */
#define ADXL345_ID 0xE5

/* Control register definitions */
#define ADXL345_REG_DEVID 0x00
#define ADXL345_REG_POWERCTL 0x2D
#define ADXL345_REG_FORMAT 0x31

/* ADXL default settings */
/* TODO: check datasheet */
#define ADXL345_REG_POWERCTL_VALUE 0x08
#define ADXL345_REG_FORMAT_VALUE 0x03

/* Data register definitions */
#define ADXL345_REG_X0 0x32
#define ADXL345_REG_X1 0x33
#define ADXL345_REG_Y0 0x34
#define ADXL345_REG_Y1 0x35
#define ADXL345_REG_Z0 0x36
#define ADXL345_REG_Z1 0x37

/* Chip Select macros */
#define adxl345_CSLOW() (ADXL345_PORT &= ~(1 << ADXL345_PIN_CS))
#define adxl345_CSHIGH() (ADXL345_PORT |= (1 << ADXL345_PIN_CS))

int	adxl345_init(void);
double adxl345_read_x(void);
double adxl345_read_y(void);
double adxl345_read_z(void);

#endif /* ADXL345_H_ */