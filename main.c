/*
 * cansat-firmware.c
 *
 * Created: 10.02.2022 19:17:35
 * Author : localuser-1
 */ 


#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <drivers/bus/spi.h>
#include <drivers/bus/uart.h>
#include <drivers/bmp280.h>
#include <drivers/ds18b20.h>
#include <drivers/adxl345.h>
#include <drivers/mega128.h>
#include <drivers/nrf24l01.h>
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>

static int uart_putchar(char c, FILE *stream);
static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

static int uart_putchar(char c, FILE *stream)
{
	uart0_putc(c);
	return 0;
}

int main(void)
{
	arch_ENABLE_INTERRUPTS();
	
	spi_init(0);
	uart0_init(38400);
	
	stdout = &mystdout;
	
	printf("Hello from cansat-firmware test\r\n");
	bmp280_init();
	adxl345_init();
	nrf24_init();
	
	double tmp1 = 0;
	double prs = 0;
	double ax = 0;
	double ay = 0;
	double az = 0;
	double tmp2 = 0;
	
	ds18b20_request_temperature();
	
    while (1) 
    {
		tmp1 = bmp280_read_temperature();
		prs = bmp280_read_pressure();
		ax = adxl345_read_x();
		ay = adxl345_read_y();
		az = adxl345_read_z();
		ds18b20_request_temperature();
		_delay_ms(2000);
		tmp2 = ds18b20_read_temperature();
		
		printf("Test data: prs=%f tmp1=%f ax=%f ay=%f az=%f tmp2=%f\r\n", prs, tmp1, ax, ay, az, tmp2);
    }
}

