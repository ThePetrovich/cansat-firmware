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
	adxl345_init();
	bmp280_init();
	
    while (1) 
    {
		ds18b20_request_temperature();
		_delay_ms(1000);
		
		double tmp1 = bmp280_read_temperature();
		double prs = bmp280_read_pressure();
		double ax = adxl345_read_x();
		double ay = adxl345_read_y();
		double az = adxl345_read_z();
		double tmp2 = ds18b20_read_temperature();
		
		printf("Test data: prs=%f tmp1=%f ax=%f ay=%f az=%f tmp2=%f\r\n", prs, tmp1, ax, ay, az, tmp2);
    }
}

