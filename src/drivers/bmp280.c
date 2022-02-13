/*
 * bmp280.c
 *
 * Created: 08.06.2019 22:31:31
 *  Author: ThePetrovich
 */ 


#include <drivers/mega128.h>
#include <drivers/bmp280.h>
#include <drivers/bus/spi.h>
#include <avr/io.h>
#include <stdint.h>
#include <math.h>

static uint16_t dig_T1;
static int16_t  dig_T2;
static int16_t  dig_T3;

static uint16_t dig_P1;
static int16_t  dig_P2;
static int16_t  dig_P3;
static int16_t  dig_P4;
static int16_t  dig_P5;
static int16_t  dig_P6;
static int16_t  dig_P7;
static int16_t  dig_P8;
static int16_t  dig_P9;

static int32_t tempCalibrated;

static uint8_t bmp280_read_register_1(uint8_t addr)
{
	uint8_t response = 0;
	
	spi_start(SPI_PRESCALER_16 | SPI_MSBFIRST | SPI_MODE3);
	
	bmp280_CS_LOW();
	
	/* Atomically read BMP280 1-byte register, MSB must be 1 */
	arch_enterCriticalSection();
	spi_write(addr | 0x80);
	response = spi_read_write(0x00);
	arch_exitCriticalSection();
	
	bmp280_CS_HIGH();
	
	spi_stop();
	
	return response;
}

static uint16_t bmp280_read_register_2(uint8_t addr)
{
	uint16_t response = 0;
	
	spi_start(SPI_PRESCALER_16 | SPI_MSBFIRST | SPI_MODE3);
	
	bmp280_CS_LOW();
	
	/* Atomically read BMP280 2-byte register, MSB must be 1 */
	arch_enterCriticalSection();
	spi_write(addr | 0x80);
	response = spi_read_write(0x00);
	response |= (spi_read_write(0x00) << 8);
	arch_exitCriticalSection();
	
	bmp280_CS_HIGH();
	
	spi_stop();
	
	return response;
}

static uint32_t bmp280_read_register_4(uint8_t addr)
{
	uint32_t response = 0;
	
	spi_start(SPI_PRESCALER_16 | SPI_MSBFIRST | SPI_MODE3);
	
	bmp280_CS_LOW();
	
	/* Atomically read BMP280 4-byte register, MSB must be 1 */
	arch_enterCriticalSection();
	spi_write(addr | 0x80);
	response = spi_read_write(0x00);
	response <<= 8;
	response |= spi_read_write(0x00);
	response <<= 8;
	response |= spi_read_write(0x00);
	arch_exitCriticalSection();
	
	bmp280_CS_HIGH();
	
	spi_stop();
	
	return response;
}

static void bmp280_write_register(uint8_t addr, uint8_t data)
{
	spi_start(SPI_PRESCALER_16 | SPI_MSBFIRST | SPI_MODE3);
	
	bmp280_CS_LOW();
	
	/* Atomically write BMP280 register, MSB must be 0 */
	arch_enterCriticalSection();
	spi_write(addr & ~0x80);
	spi_write(data & ~0x80);
	arch_exitCriticalSection();
	
	bmp280_CS_HIGH();
	spi_stop();
}

static void bmp280_read_calibration(void)
{
	dig_T1 = bmp280_read_register_2(BMP280_REG_DIG_T1);
	dig_T2 = (int16_t)bmp280_read_register_2(BMP280_REG_DIG_T2);
	dig_T3 = (int16_t)bmp280_read_register_2(BMP280_REG_DIG_T3);

	dig_P1 = bmp280_read_register_2(BMP280_REG_DIG_P1);
	dig_P2 = (int16_t)bmp280_read_register_2(BMP280_REG_DIG_P2);
	dig_P3 = (int16_t)bmp280_read_register_2(BMP280_REG_DIG_P3);
	dig_P4 = (int16_t)bmp280_read_register_2(BMP280_REG_DIG_P4);
	dig_P5 = (int16_t)bmp280_read_register_2(BMP280_REG_DIG_P5);
	dig_P6 = (int16_t)bmp280_read_register_2(BMP280_REG_DIG_P6);
	dig_P7 = (int16_t)bmp280_read_register_2(BMP280_REG_DIG_P7);
	dig_P8 = (int16_t)bmp280_read_register_2(BMP280_REG_DIG_P8);
	dig_P9 = (int16_t)bmp280_read_register_2(BMP280_REG_DIG_P9);
}

int bmp280_init(void)
{
	/* Init BMP280 Chip Select pin */
	BMP280_CS_DDR |= (1 << BMP280_CS);
	bmp280_CS_HIGH();
	
	/* Check BMP280 device ID, quit on mismatch */
	if (bmp280_read_register_1(0x00) != BMP280_ID) {
		return -1;
	}
	
	/* Write BMP280 configuration registers */
	bmp280_write_register(BMP280_REG_CONTROL, BMP280_REG_CONTROL_VALUE);
	bmp280_read_calibration();
	
	return 0;
}

double bmp280_read_temperature(void)
{
	double var1 = 0;
	double var2 = 0; 
	double t = 0;
	
	int32_t adc_T = (int32_t)bmp280_read_register_4(BMP280_REG_TEMPDATA);
	adc_T >>= 4;
	
	/* Some forkery from the datasheet, no idea what is going on here */
	var1 = (((double)adc_T) / 16384.0 - (double)dig_T1 / 1024.0) * ((double)dig_T2);
	var2 = ((((double)adc_T) / 131072.0 - ((double)dig_T1) / 8192.0 * ((double)adc_T) / 131072.0 \
			- ((double)dig_T1) / 8192.0)) * ((double)dig_T3);
		 
	tempCalibrated = (int32_t)(var1 + var2);
	
	t = (var1 + var2) / 5120.0;
	
	return t;
}

double bmp280_read_pressure(void)
{
	double var1 = 0;
	double var2 = 0;
	double p = 0;
	
	int32_t adc_P = (int32_t)bmp280_read_register_4(BMP280_REG_PRESSUREDATA);
	adc_P >>= 4;
	
	/* The fork is this lmao */
	var1 = ((double)tempCalibrated / 2.0) - 64000.0;
	var2 = var1 * var1 * ((double)dig_P6) / 32768.0;
	var2 = var2 + var1 * ((double)dig_P5) / 2.0;
	var2 = (var2 / 4.0) + (((double)dig_P4) * 65536.0);
	var1 = (((double)dig_P3) * var1 * var1 / 524288.0 + ((double)dig_P2) * var1) / 524288.0;
	var1 = (1.0 + var1 / 32768.0) * ((double)dig_P1);
	p = 1048576.0 - (double)adc_P;
	p = (p - (var2 / 4096.0)) * 6250.0 / var1;
	var1 = ((double)dig_P9) * p * p / 2147483648.0;
	var2 = p*((double)dig_P8) / 32768.0;
	
	p = p + (var1 + var2 + ((double)dig_P7)) / 16.0;
	
	return p;
}

double bmp280_calc_altitude(double sea_prs)
{
	double prs = bmp280_read_pressure();

	double altitude = 44330.0 * (1.0 - pow(prs / sea_prs, 0.1903));

	return altitude;
}
