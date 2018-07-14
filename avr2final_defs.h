#ifndef AVR2FINAL_DEFS_H_
#define AVR2FINAL_DEFS_H_

#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "HDK128.h"

/*MANEJO DE BITS BASICO*/

#define set_bit(reg,bit) 		reg|=(1<<bit)
#define clr_bit(reg,bit) 		reg&=~(1<<bit)
#define tog_bit(reg,bit) 		reg^=(1<<bit)
#define get_bit(reg,bit) 		(reg&(1<<bit))
#define clr_reg(reg) 			reg&=0
#define set_output(ddr,bit) 	ddr|=(1<<bit)
#define set_input(ddr,bit) 		ddr&=~(1<<bit)

/*HABILITACION Y MANEJO DE I/O*/

#define PortB4_Output 			set_output(DDRB,4)
#define PinE4_Input 			set_input(DDRE,4)
#define PinE5_Input 			set_input(DDRE,5)

#define Get_PINE4				get_bit(PINE,4)
#define Get_PINE5				get_bit(PINE,5)
#define PORTB4_off				clr_bit(PORTB,4)

/*TIMER 0*/

#define timer0_mode_fastpwm		set_bit(TCCR0,WGM00);\
								set_bit(TCCR0,WGM01)
#define timer0_prescaler_8		TCCR0&=~((1<<CS02)|(1<<CS00)); \
	                       	   	TCCR0|=(1<<CS01)
#define timer0_comp_value		OCR0
#define timer0_comp_clear		set_bit(TCCR0,COM01);\
								clr_bit(TCCR0,COM00)
#define timer0_comp_disc		clr_bit(TCCR0,COM01);\
								clr_bit(TCCR0,COM00)
#define timer0_stop 			TCCR0&=~((1<<CS02)|(1<<CS01)|(1<<CS00))

/*TIMER 2*/

#define timer2_mode_compare 	TCCR2|=(1<<WGM21);\
								TCCR2&=~(1<<WGM20)
#define timer2_prescaler_8 		TCCR2&=~((1<<CS22)|(1<<CS20)); \
								TCCR2|=(1<<CS21)
#define timer2_compare_value 	OCR2

/*CONFIGURACIÓN UART1*/

#define FOSC 8000000
#define BAUD 9600
#define MYUBRR FOSC/BAUD/16-1
void UART1_init(unsigned int ubrr);
#define UART1_REG			UDR1
#define enable_uart1_RX		UCSR1B|=(1<<RXEN1)
#define enable_uart1_TX		UCSR1B|=(1<<TXEN1)
#define off_uart1_RX		UCSR1B&=~(1<<RXEN1)
#define off_uart1_TX		UCSR1B&=~(1<<TXEN1)

/*INTERRUPCIONES*/

#define enable_INT4				set_bit(EIMSK,INT4)
#define enable_INT5				set_bit(EIMSK,INT5)
#define off_INT4				clr_bit(EIMSK,INT4)
#define off_INT5				clr_bit(EIMSK,INT5)
#define INT4_falling_edge		set_bit(EICRB,ISC41)
#define INT5_falling_edge		set_bit(EICRB,ISC51)
#define enable_int_tmr0cmp 		TIMSK|=(1<<OCIE0)
#define enable_int_tmr2cmp 		TIMSK|=(1<<OCIE2)
#define enable_int_uart1_TX		UCSR1B&=~(1<<RXCIE1);\
								UCSR1B|=(1<<TXCIE1)
#define enable_int_uart1_RX		UCSR1B&=~(1<<TXCIE1);\
								UCSR1B|=(1<<RXCIE1)
#define off_int_uart1_RX		UCSR1B&=~(1<<RXCIE1)

/*FUNCIONES Y FLAGS*/

#define FACTOR 65,535

extern volatile bit_t flags;
#define flag_man	flags.B0
#define flag_auto	flags.B1
#define flag_lux	flags.B2
#define flag_bot	flags.B3
#define flag_modo	flags.B4
#define flag_est	flags.B5
#define flag_bri	flags.B6
#define flag_s		flags.B7

void automatico(void);
void manual(void);
void show_LCD(char modo, char estado[4], char brillo, unsigned int lux);
void show_UART1_modo(char modo);
void show_UART1_estado(char estado[4], char brillo);
void show_UART1_lux(unsigned int lux);

#endif /* AVR2FINAL_DEFS_H_ */
