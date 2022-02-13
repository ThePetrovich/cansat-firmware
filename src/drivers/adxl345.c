/*
 * adxl345.c
 *
 * Created: 23.03.2019 17:28:05
 * Author: ThePetrovich
 */ 


#include <drivers/mega128.h>
#include <drivers/adxl345.h>
#include <drivers/bus/spi.h>
#include <avr/io.h>
#include <stdint.h>

static uint8_t adxl345_read_register_1(uint8_t addr)
{
	uint8_t response = 0;
	
	spi_start(SPI_PRESCALER_16 | SPI_MSBFIRST | SPI_MODE3);
	
	adxl345_CS_LOW();
	
	/* Atomically read ADXL register, MSB must be 1 */
	arch_enterCriticalSection();
	spi_write(addr | 0x80);
	response = spi_read_write(0xFF);
	arch_exitCriticalSection();
	
	adxl345_CS_HIGH();
	
	spi_stop();
	
	return response;
}

static void adxl345_write_register(uint8_t addr, uint8_t data)
{
	spi_start(SPI_PRESCALER_16 | SPI_MSBFIRST | SPI_MODE3);
	
	adxl345_CS_LOW();
	
	/* Atomically write ADXL register, MSB must be 0 */
	arch_enterCriticalSection();
	spi_write((addr & ~0x80));
	spi_write(data);
	arch_exitCriticalSection();
	
	adxl345_CS_HIGH();
	
	spi_stop();
}

int adxl345_init()
{
	/* Init ADXL Chip Select pin */
	ADXL345_CS_DDR |= (1 << ADXL345_CS);
	adxl345_CS_HIGH();
	
	/* Check ADXL device ID, quit on mismatch */
	if (adxl345_read_register_1(ADXL345_REG_DEVID) != ADXL345_ID) {
		return -1;
	}
	
	/* Write ADXL configuration registers */
	adxl345_write_register(ADXL345_REG_POWERCTL, ADXL345_REG_POWERCTL_VALUE);
	adxl345_write_register(ADXL345_REG_FORMAT, ADXL345_REG_FORMAT_VALUE);
	
	return 0;
}

static double adxl345_read_acceleration(uint8_t addr0, uint8_t addr1)
{
	/* Read acceleration high and low registers */
	int16_t r0 = adxl345_read_register_1(addr0);
	int16_t r1 = adxl345_read_register_1(addr1);
		
	r1 = r1 << 8;
	r1 |= r0;
		
	return ((double)r1 / 32.0);
}

double adxl345_read_x()
{
	return adxl345_read_acceleration(ADXL345_REG_X0, ADXL345_REG_X1);
}

double adxl345_read_y()
{
	return adxl345_read_acceleration(ADXL345_REG_Y0, ADXL345_REG_Y1);
}

double adxl345_read_z()
{
	return adxl345_read_acceleration(ADXL345_REG_Z0, ADXL345_REG_Z1);
}