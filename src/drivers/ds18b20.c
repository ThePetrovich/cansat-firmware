/*
 * ds18b20.c
 *
 * Created: 17.05.2019 17:58:11
 *  Author: ThePetrovich
 */ 


#include <drivers/mega128.h>
#include <drivers/ds18b20.h>
#include <util/delay.h>
#include <avr/io.h>
#include <stdint.h>
#include <math.h>

uint8_t ds18b20_reset(void)
{
	arch_enterCriticalSection();
	
	DS18B20_PORT &= ~(1 << DS18B20_PIN_DATA);
	DS18B20_DDR |= (1 << DS18B20_PIN_DATA);
	_delay_us(480);
	
	DS18B20_DDR &= ~(1 << DS18B20_PIN_DATA);
	_delay_us(60);
	
	uint8_t i = (DS18B20_PIN & (1 << DS18B20_PIN_DATA));
	_delay_us(410);
	
	arch_exitCriticalSection();
	
	return i;
}

static void ds18b20_write_bit(uint8_t value)
{
	arch_enterCriticalSection();
	
	DS18B20_PORT &= ~(1 << DS18B20_PIN_DATA);
	DS18B20_DDR |= (1 << DS18B20_PIN_DATA);
	_delay_us(6);
	
	if(value) 
		DS18B20_DDR &= ~(1 << DS18B20_PIN_DATA);
	_delay_us(60);
	
	DS18B20_DDR &= ~(1 << DS18B20_PIN_DATA);
	
	arch_exitCriticalSection();
}

static uint8_t ds18b20_read_bit(void)
{
	uint8_t value = 0;
	
	arch_enterCriticalSection();
	
	DS18B20_PORT &= ~(1 << DS18B20_PIN_DATA);
	DS18B20_DDR |= (1 << DS18B20_PIN_DATA);
	_delay_us(6);
	
	DS18B20_DDR &= ~(1 << DS18B20_PIN_DATA);
	_delay_us(10);
	
	if(DS18B20_PIN & (1 << DS18B20_PIN_DATA)) 
		value = 1;
	_delay_us(55);
	
	arch_exitCriticalSection();
	return value;
}

static uint8_t ds18b20_read_byte(void)
{
	uint8_t n = 0;
	
	for (uint8_t i = 8; i != 0; i--) {
		n = (n >> 1);
		n |= (ds18b20_read_bit() << 7);
	}
	return n;
}

static void ds18b20_write_byte(uint8_t value)
{
	for (uint8_t i = 8; i != 0; i--) {
		ds18b20_write_bit(value & 1);
		value = (value >> 1);
	}
}

void ds18b20_request_temperature(void)
{
	ds18b20_reset();
	ds18b20_write_byte(DS_CMD_SKIPROM);
	ds18b20_write_byte(DS_CMD_CONVERTTEMP);
}
	
double ds18b20_read_temperature(void)
{
	uint8_t raw[2] = {0};
	int8_t digit = 0;
	uint16_t decimal = 0;
	double value = 0;
	
	ds18b20_reset();
	ds18b20_write_byte(DS_CMD_SKIPROM);
	ds18b20_write_byte(DS_CMD_RSCRATCHPAD);
	
	raw[0] = ds18b20_read_byte();
	raw[1] = ds18b20_read_byte();
	ds18b20_reset();
	
	digit = raw[0] >> 4;
	digit |= (raw[1] & 0x7) << 4;
	
	decimal = raw[0] & 0xF;
	decimal *= 625;
	
	value = (double)digit;
	
	if (digit < 0) {
		value -= ((double)decimal / 1000.0);
	}
	else {
		value += ((double)decimal / 1000.0);
	}
	
	return value;
}