/*
 * arch.h
 * 
 * Created: 28.12.2021 01:27:13
 * Author: ThePetrovich
 */


#ifndef YKTS_BOOT_MEGA128_H_
#define YKTS_BOOT_MEGA128_H_

#include <avr/io.h>
#include <avr/iom128.h>

/* CPU frequency */
#ifndef F_CPU
#define F_CPU 16000000L
#endif

#ifndef KERNEL_ARCH_H_
	#define lo8(x) ((x)&0xff)
	#define hi8(x) ((x)>>8)

	#define arch_DISABLE_INTERRUPTS() asm volatile ("cli"::)
	#define arch_ENABLE_INTERRUPTS() asm volatile ("sei"::)
	#define arch_STATUS_REG SREG
	#define arch_NOP() asm volatile ("nop"::)
	#define arch_enterCriticalSection()	asm volatile (  "lds __tmp_reg__, __SREG__ 	\n\t"\
								"cli				\n\t"\
								"push __tmp_reg__"	    ::)
	#define arch_exitCriticalSection() 	asm volatile (  "pop __tmp_reg__		\n\t"\
								"sei				\n\t"\
								"sts __SREG__, __tmp_reg__" ::)
#endif

#endif /* YKTS_BOOT_MEGA128_H_ */